#include "evideorender.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "defThread.h"
#include "base/misctool.h"

const char* kFrameBuffer = R"(
    attribute vec3 vPosition;
    attribute vec2 txcod;

    varying  vec2 TexCoord;

    void main()
    {
        gl_Position = vec4(vPosition, 1.0);
        TexCoord = vec2(txcod.x, txcod.y);
    }
)";

const char* kFS = R"(
    precision mediump float;
    varying  vec2 TexCoord;
    uniform sampler2D ourTexture;
    void main()
    {
        vec4 texColor = texture2D(ourTexture, TexCoord);
        gl_FragColor = texColor;
    }
)";

const char* kVS0 = R"(
    attribute vec3 vPosition;
    attribute vec3 vColor;
    attribute vec2 txcod;
    //uniform mat4 transform;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    varying  vec4 vertexColor; // 为片段着色器指定一个颜色输出
    varying  vec2 v_texcoord;

    void main()
    {
        gl_Position = projection * view * model * vec4(vPosition, 1.0);
        //gl_Position =  view * model * vec4(vPosition, 1.0);
        //gl_Position = model * vec4(vPosition, 1.0);
        vertexColor = vec4(vColor, 1.0);
        //v_texcoord = vec2(txcod.x, txcod.y);
        v_texcoord = txcod;
    }
)";

//fragment shader
//yuv片元着色器
const char* kFS0_yuv = R"(
    //precision mediump float;
    precision highp float;
    varying  vec4 vertexColor; // 为片段着色器指定一个颜色输出
    varying  vec2 v_texcoord;
    uniform sampler2D ourTexture;

    uniform lowp sampler2D s_textureY;
    uniform lowp sampler2D s_textureU;
    uniform lowp sampler2D s_textureV;
    void main() {
        float y, u, v, r, g, b;
        y = texture2D(s_textureY, v_texcoord).r;
        u = texture2D(s_textureU, v_texcoord).r;
        v = texture2D(s_textureV, v_texcoord).r;
        u = u - 0.5;
        v = v - 0.5;
        r = y + 1.403 * v;
        g = y - 0.344 * u - 0.714 * v;
        b = y + 1.770 * u;
        vec3 objectColor = vec3(r,g,b);
        gl_FragColor = vec4(objectColor, 1.0);
    }
)";

//nv12片元着色器
const char* kFS0_nv = R"(
    //precision mediump float;
    precision highp float;
    varying  vec4 vertexColor; // 为片段着色器指定一个颜色输出
    varying  vec2 v_texcoord;
    uniform sampler2D ourTexture;

    uniform lowp sampler2D s_textureY;
    uniform lowp sampler2D s_textureU;
    uniform lowp sampler2D s_textureV;
    void main() {
        vec3 yuv;
        vec3 rgb;
        yuv.x = texture2D(s_textureY, v_texcoord).r -16./256.;
        yuv.y = texture2D(s_textureU, v_texcoord).r - 128./256.;
        yuv.z = texture2D(s_textureU, v_texcoord).g - 128./256.;
        rgb = mat3( 1,       1,         1,
                    0,       -0.39465,  2.03211,
                    1.13983, -0.58060,  0) * yuv;
        gl_FragColor = vec4(rgb, 1.0);
    }
)";

bool create_glsl_shade(const char* vec, const char* frag, GLuint& program) {
   bool ret = false;
   unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vec, NULL);
   glCompileShader(vertexShader);
   // check for shader compile errors
   int success = 0;
   char infoLog[512];
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
       glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
       std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
       return ret;
   }
   // fragment shader
   unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &frag, NULL);
   glCompileShader(fragmentShader);
   // check for shader compile errors
   success = 0;
   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
       glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
       std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
       return ret;
   }
   program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glAttachShader(program, fragmentShader);
   glLinkProgram(program);

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
   // check for linking errors
   success = 0;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success) {
       glGetProgramInfoLog(program, 512, NULL, infoLog);
       std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
       return ret;
   }
   ret = true;
   return ret;
}

extern bool IsWindow(uint64_t id);

void eVideoRender::greate_yuv_texture(unsigned int& texture, int width, int height, GLint gltype) {
    if (texture) {
        //return;
        eglReleaseTexImage(hDisp_, surfRet_, texture);
        glDeleteTextures(1, &texture);
        texture = 0;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // 纹理过滤函数, GL_TEXTURE_MIN_FILTER 缩小过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    // 纹理过滤函数, GL_TEXTURE_MAG_FILTER 放大过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // 纹理过滤函数, GL_TEXTURE_WRAP_S S方向上的贴图模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // 纹理过滤函数, GL_TEXTURE_WRAP_T T方向上的贴图模式
    glTexImage2D(GL_TEXTURE_2D, 0, gltype, width, height, 0, gltype, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<eVideoRender> eVideoRender::Create(const void* hwnd, size_t width,
   size_t height, float model_ang) {
   std::shared_ptr<eVideoRender> sp_renderer;
   eVideoRender* gl_renderer = new eVideoRender(width, height, model_ang);
   if (!gl_renderer->Init(hwnd)) {
       delete gl_renderer;
       fprintf(stderr, "fail init render!!!\n");
       return sp_renderer;
   }
   //unsigned long dw = neosmart::WaitForMultipleEvents( gl_renderer->_hInitEvents, 2, false, neosmart::WAIT_INFINITE);
   /*if (dw == 0) {
       sp_renderer.reset(gl_renderer); // std::make_shared<eGLRender>(width, height);
   }
   else {
       delete gl_renderer;
   }*/
   sp_renderer.reset(gl_renderer);
   return sp_renderer;
}

eVideoRender::~eVideoRender() {
   done_ = true;
   //Destroy();
   /*if (_hCloseEvent) {
       CloseHandle(_hCloseEvent);
   }*/
}

eVideoRender::eVideoRender(size_t width, size_t height, float model_ang) : vbo_(0), ebo_(0), framebuffer_(0), rbo_(0),
           textureY_(0), textureU_(0), textureV_(0){
   width_ = width;
   height_ = height;
   model_ang_ = model_ang;
   //pic_w_ = width;
   //pic_h_ = height;
   //pix_buf_ = new unsigned char[PIX_BUF_SIZE];
   //_hInitEvents[0] = 0;
   //_hInitEvents[1] = 0;
}

void eVideoRender::CleanContext() {
   if (hDisp_) {
       eglMakeCurrent(hDisp_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
       if (surfRet_) {
           eglDestroySurface(hDisp_, surfRet_);
           surfRet_ = EGL_NO_SURFACE;
       }
       if (context_) {
           eglDestroyContext(hDisp_, context_);
           context_ = EGL_NO_CONTEXT;
       }
       eglTerminate(hDisp_);
       hDisp_ = EGL_NO_DISPLAY;
       eglWaitClient();
   }
}

int angle_back[] = {
   0,
   EGL_PLATFORM_ANGLE_TYPE_OPENGLES_ANGLE,
   EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE,
   EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
};

EGLDisplay GetDisplay(int idx) {
   EGLDisplay disp = nullptr;
   int engtype = angle_back[idx];
   if (engtype == 0) {
       disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
   }
   else {
       /*EGLint displayAttribs[] = {EGL_PLATFORM_ANGLE_TYPE_ANGLE,
                              EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE,
                              EGL_NONE };*/
       std::vector<EGLAttrib> displayAttribs;
       displayAttribs.push_back(EGL_PLATFORM_ANGLE_TYPE_ANGLE);
       displayAttribs.push_back(engtype);
       displayAttribs.push_back(EGL_NONE);

       /*auto eglGetPlatformDisplayEXT = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
           eglGetProcAddress("eglGetPlatformDisplayEXT"));
       if (eglGetPlatformDisplayEXT) {
           disp = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT,
               reinterpret_cast<void*>(hdc),
               &(displayAttribs[0]));
       }*/
       eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, EGL_DEFAULT_DISPLAY, &displayAttribs[0]);
   }

   return disp;
}

bool eVideoRender::InitSurface() {
   int try_times = 0;
   int try_sum = sizeof(angle_back) / sizeof(int);

   bool bwin = IsWindow(hwnd_);
   fprintf(stdout, "####---- hwnd: %p, iswin: %d\n", hwnd_, bwin);

   while (try_times < try_sum) {
       EGLConfig windowConfig;
       EGLBoolean r = 0;
       fprintf(stdout, "####---- try InitSurface: %d\n", try_times);

       hDisp_ = GetDisplay(try_times++);
       if (hDisp_) {
           EGLint minor, major;
           r = eglInitialize(hDisp_, &major, &minor);
           fprintf(stdout, "#####----hDisp_: %d\n", hDisp_);
       }
       else {
           fprintf(stdout, "#####----hDisp_: %d fail\n", hDisp_);
           continue;
       }

       eglBindAPI(EGL_OPENGL_ES_API);

       const EGLint configAttributes[] =
       {
           EGL_BUFFER_SIZE, 0,
           EGL_RED_SIZE, 5,
           EGL_GREEN_SIZE, 6,
           EGL_BLUE_SIZE, 5,
           EGL_ALPHA_SIZE, 0,
           EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
           EGL_DEPTH_SIZE, 24,
           EGL_LEVEL, 0,
           EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
           EGL_SAMPLE_BUFFERS, 0,
           EGL_SAMPLES, 0,
           EGL_STENCIL_SIZE, 0,
           EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
           EGL_TRANSPARENT_TYPE, EGL_NONE,
           EGL_TRANSPARENT_RED_VALUE, EGL_DONT_CARE,
           EGL_TRANSPARENT_GREEN_VALUE, EGL_DONT_CARE,
           EGL_TRANSPARENT_BLUE_VALUE, EGL_DONT_CARE,
           EGL_CONFIG_CAVEAT, EGL_DONT_CARE,
           EGL_CONFIG_ID, EGL_DONT_CARE,
           EGL_MAX_SWAP_INTERVAL, EGL_DONT_CARE,
           EGL_MIN_SWAP_INTERVAL, EGL_DONT_CARE,
           EGL_NATIVE_RENDERABLE, EGL_DONT_CARE,
           EGL_NATIVE_VISUAL_TYPE, EGL_DONT_CARE,
           EGL_NONE
       };

       EGLint numConfigs;

       //if (!eglGetConfigs(_hDisp, NULL, 0, &iConfigs)) {
       //    return;
       //}
       fprintf(stdout, "####----do eglChooseConfig\n");
       if (!eglChooseConfig(hDisp_, configAttributes, &windowConfig, 1, &numConfigs) || (numConfigs != 1)) {
           EGLint err = eglGetError();
           fprintf(stdout, "####----fail eglChooseConfig, err:%d\n", err);
           CleanContext();
           continue;
       }

       fprintf(stdout, "####----do create surface\n");
       EGLint surfaceAttributes[] = { EGL_NONE };
       surfRet_ = eglCreateWindowSurface(hDisp_, windowConfig, hwnd_, surfaceAttributes);
       if (surfRet_ == EGL_NO_SURFACE) {
           EGLint err = eglGetError();
           fprintf(stdout, "####----fail eglCreateWindowSurface, err:%d\n", err);
           CleanContext();
           continue;
       }

       static const EGLint ctx_attribs[] = {
           EGL_CONTEXT_CLIENT_VERSION, 2,
           EGL_NONE
       };

       fprintf(stdout, "####----do eglCreateContext\n");
       context_ = eglCreateContext(hDisp_, windowConfig, EGL_NO_CONTEXT, ctx_attribs);
       if (context_ == EGL_NO_CONTEXT) {
           EGLint err = eglGetError();
           fprintf(stdout, "####---fail eglCreateContext, err:%d\n", err);
           CleanContext();
           continue;
       }

       EGLint val, width, height;
       eglQuerySurface(hDisp_, surfRet_, EGL_WIDTH, &width);
       eglQuerySurface(hDisp_, surfRet_, EGL_HEIGHT, &height);
       fprintf(stdout, "####---gl surface w:%d, h:%d\n", width, height);
       eglGetConfigAttrib(hDisp_, windowConfig, EGL_SURFACE_TYPE, &val);
       EGLint test = val & EGL_WINDOW_BIT;
       break;
   }
   fprintf(stdout, "surfRet_: %p\n", surfRet_);
   return surfRet_ != EGL_NO_SURFACE;
}



void eVideoRender::Generate_Square_array(int line, int col, std::vector<Signle_Square>& out, std::vector<Draw_Indices>& ind_array) {
   out.clear();
   ind_array.clear();
   constexpr float h_margin = 0.f;
   constexpr float v_margin = 0.f;
   constexpr float square_size = 2.f;
   float init_tr_x, init_tr_y, init_br_x, init_br_y, init_bl_x, init_bl_y, init_tl_x, init_tl_y;
   init_tr_x = init_tr_y = 1.f;
   init_br_x = 1.f; init_br_y = -1.f;
   init_bl_x = init_bl_y = -1.f;
   init_tl_x = -1.f; init_tl_y = 1.f;
   for (int i = 0; i < line; ++i) {
       init_tr_y = 1.f + (-(square_size * i));
       init_tl_y = 1.f + (-(square_size * i));
       init_br_y = init_tr_y - square_size;
       init_bl_y = init_tl_y - square_size;
       init_tr_x = init_br_x = 1.f;
       init_bl_x = init_tl_x = -1.f;
       for (int j = 0; j < col; ++j) {
           init_bl_x = -1.f + (square_size * j);
           init_tl_x = -1.f + (square_size * j);
           init_tr_x = init_tl_x + square_size;
           init_br_x = init_bl_x + square_size;
           Signle_Square item;
           item.tr[0] = init_tr_x;	item.tr[1] = init_tr_y;
           item.br[0] = init_br_x;	item.br[1] = init_br_y;
           item.bl[0] = init_bl_x;	item.bl[1] = init_bl_y;
           item.tl[0] = init_tl_x;	item.tl[1] = init_tl_y;
           out.push_back(item);
       }
   }

   int sum = line * col;
   constexpr int step = 4;
   for (int i = 0; i < sum; ++i) {
       Draw_Indices indices;
       indices.tri_frist[0] = i * step;
       indices.tri_frist[1] = indices.tri_frist[0] + 1;
       indices.tri_frist[2] = indices.tri_frist[0] + 3;
       indices.tri_second[0] = indices.tri_frist[1];
       indices.tri_second[1] = indices.tri_second[0] + 1;
       indices.tri_second[2] = indices.tri_frist[2];
       ind_array.push_back(indices);
   }
}

bool eVideoRender::Init(const void* hwnd) {
   hwnd_ = (uint64_t)hwnd;
   _hInitEvents[0] = neosmart::CreateEvent();
   _hInitEvents[1] = neosmart::CreateEvent();
   nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=] {
       bool ret = false;
       bool enter_face = false;
   while (true)
   {
       if (!::IsWindow(hwnd_)) {
           fprintf(stderr, "fail hwnd!\n");
           break;
       }
       if (!InitSurface()) {
           fprintf(stderr, "fail InitSurface!\n");
           break;
       }
       eglMakeCurrent(hDisp_, surfRet_, surfRet_, context_);
       enter_face = true;
       if (!CreateFBO(width_, height_)) {
           fprintf(stderr, "fail CreateFBO!\n");
           break;
       }

       // build and compile our shader program
   // ------------------------------------
   // vertex shader
       unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
       glShaderSource(vertexShader, 1, &kVS0, NULL);
       glCompileShader(vertexShader);
       // check for shader compile errors
       int success = 0;
       char infoLog[512];
       glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
       if (!success)
       {
           glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
           std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
           fprintf(stderr, "fail vertexShader!\n");
           break;
       }
       // fragment shader
       unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
       glShaderSource(fragmentShader, 1, &kFS0_yuv, NULL);
       glCompileShader(fragmentShader);
       // check for shader compile errors
       success = 0;
       glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
       if (!success)
       {
           glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
           std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
           fprintf(stderr, "fail fragmentShader!\n");
           break;
       }
       program_ = glCreateProgram();
       glAttachShader(program_, vertexShader);
       glAttachShader(program_, fragmentShader);
       glLinkProgram(program_);

       glDeleteShader(vertexShader);
       glDeleteShader(fragmentShader);
       // check for linking errors
       success = 0;
       glGetProgramiv(program_, GL_LINK_STATUS, &success);
       if (!success) {
           glGetProgramInfoLog(program_, 512, NULL, infoLog);
           std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
           fprintf(stderr, "fail link program!\n");
           break;
       }
       {
           //build nv12 program
           unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
           glShaderSource(vertexShader, 1, &kVS0, NULL);
           glCompileShader(vertexShader);
           // check for shader compile errors
           int success = 0;
           char infoLog[512];
           glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
           if (!success)
           {
               glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
               std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
               fprintf(stderr, "fail vertexShader!\n");
               break;
           }
           // fragment shader
           unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
           glShaderSource(fragmentShader, 1, &kFS0_nv, NULL);
           glCompileShader(fragmentShader);
           // check for shader compile errors
           success = 0;
           glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
           if (!success)
           {
               glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
               std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
               fprintf(stderr, "fail fragmentShader!\n");
               break;
           }
           program_nv12_ = glCreateProgram();
           glAttachShader(program_nv12_, vertexShader);
           glAttachShader(program_nv12_, fragmentShader);
           glLinkProgram(program_nv12_);

           glDeleteShader(vertexShader);
           glDeleteShader(fragmentShader);
           // check for linking errors
           success = 0;
           glGetProgramiv(program_nv12_, GL_LINK_STATUS, &success);
           if (!success) {
               glGetProgramInfoLog(program_nv12_, 512, NULL, infoLog);
               std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
               fprintf(stderr, "fail link program!\n");
               break;
           }
           //build nv12 program done.
       }

       float vertices[] = {
           // top right
            1.f,  1.f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0, 0.0,
            // bottom right
            1.f, -1.f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 1.0,
            // bottom left
           -1.f, -1.f, 0.0f, 0.0f, 0.0f, 1.0f, 0., 1.,
           // top left
           -1.f,  1.f, 0.0f, 1.0f, 0.0f, 0.0f, 0., 0.0
       };

       float square_vertices[] = {
           // top right
            1.f,  1.f, 0.0f, 1.0, 1.0,
            // bottom right
            1.f, -1.f, 0.0f, 1.0, 0.0,
            // bottom left
           -1.f, -1.f, 0.0f, 0., 0.,
           // top left
           -1.f,  1.f, 0.0f, 0., 1.
       };

       unsigned int indices[] = {  // note that we start from 0!
           0, 1, 3,  // first Triangle
           1, 2, 3   // second Triangle
       };

       //像格子一样,渲染出不同的方块(行,列)
       Generate_Square_array(1, 1, _elearray, _indarray);

       glGenVertexArrays(sizeof(VAO_) / sizeof(VAO_[0]), VAO_);

       glGenBuffers(1, &vbo_);
       glGenBuffers(1, &ebo_);

       // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
       glBindVertexArray(VAO_[0]);

       glBindBuffer(GL_ARRAY_BUFFER, vbo_);
       //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
       int ele_type_size = sizeof(Signle_Square);
       glBufferData(GL_ARRAY_BUFFER, _elearray.size() * ele_type_size, &(_elearray[0]), GL_STATIC_DRAW);

       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
       //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
       _scene_draw_indice_sum = _indarray.size() * 6;
       int ind_type_size = sizeof(Draw_Indices);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indarray.size() * ind_type_size, &(_indarray[0]), GL_STATIC_DRAW);

       GLint nvPos = glGetAttribLocation(program_, "vPosition");
       glVertexAttribPointer(nvPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
       glEnableVertexAttribArray(nvPos);

       GLint nvCol = glGetAttribLocation(program_, "vColor");
       glVertexAttribPointer(nvCol, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
       glEnableVertexAttribArray(nvCol);

       GLint nvTxtcod = glGetAttribLocation(program_, "txcod");
       glVertexAttribPointer(nvTxtcod, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
       glEnableVertexAttribArray(nvTxtcod);
       {
           GLint nvPos = glGetAttribLocation(program_nv12_, "vPosition");
           glVertexAttribPointer(nvPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
           glEnableVertexAttribArray(nvPos);

           GLint nvCol = glGetAttribLocation(program_nv12_, "vColor");
           glVertexAttribPointer(nvCol, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
           glEnableVertexAttribArray(nvCol);

           GLint nvTxtcod = glGetAttribLocation(program_nv12_, "txcod");
           glVertexAttribPointer(nvTxtcod, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
           glEnableVertexAttribArray(nvTxtcod);
       }

       //ResizeTexture(pic_w_, pic_h_);

       //创建fb
       if (create_glsl_shade(kFrameBuffer, kFS, fb_program_)) {
           glBindVertexArray(VAO_[1]);
           glGenBuffers(1, &v_square_bo_);
           glBindBuffer(GL_ARRAY_BUFFER, v_square_bo_);
           glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

           GLint nvPos = glGetAttribLocation(fb_program_, "vPosition");
           glVertexAttribPointer(nvPos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
           glEnableVertexAttribArray(nvPos);

           GLint nvTxtcod = glGetAttribLocation(fb_program_, "txcod");
           glVertexAttribPointer(nvTxtcod, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
           glEnableVertexAttribArray(nvTxtcod);

           glGenBuffers(1, &febo_);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, febo_);
           glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
       }
       else {
           fprintf(stderr, "fail create_glsl_shade!\n");
           break;
       }

       ret = true;
       break;
   }
   if (enter_face) {
       eglMakeCurrent(hDisp_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   }
   if (ret) {
       neosmart::SetEvent(_hInitEvents[0]);
       if (!_b_single_res) {
           _b_single_res = true;
           pix_buf_array_.clear();
           for (int i = 0; i < _elearray.size(); ++i) {
               pix_buf_array_.push_back(std::make_unique<Pix_Ele>());
           }
           //_hCloseEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
           _hCloseEvent = neosmart::CreateEvent();

       }
   }
   else {
       neosmart::SetEvent(_hInitEvents[1]);
   }
   }));

   return nbase::ThreadManager::PostTask((int)ThreadId::kThreadGL, fn);
}

void eVideoRender::OnFrame(const unsigned char* buf, int buf_len, int width, int height) {
   if (done_ || pix_buf_array_.size() == 0 || hDisp_ == EGL_NO_DISPLAY || surfRet_ == EGL_NO_SURFACE || context_ == EGL_NO_CONTEXT) {
       return;
   }
   if (framebuffer_ == 0 || texColorBuffer_ == 0 || rbo_ == 0) {
       return;
   }

   if (static_cast<size_t>(width) != pic_w_ ||
       static_cast<size_t>(height) != pic_h_) {
       ResizeTexture(static_cast<size_t>(width),
           static_cast<size_t>(height));
   }

   EGLBoolean r = eglMakeCurrent(hDisp_, surfRet_, surfRet_, context_);
   if (r == false) {
       EGLint err = eglGetError();
       //if (err == EGL_NOT_INITIALIZED)
       {
           Destroy();
           Init((void*)hwnd_);
       }
       return;
   }
   //memcpy_s(pix_buf_, sizeof(pix_buf_), buf, buf_len);
   GLuint draw_program = 0;
   if (_pix_fmt == PIXEL_FORMAT::RENDER_YUV420P) {
       draw_program = program_;
   }
   else if (_pix_fmt == PIXEL_FORMAT::RENDER_NV12) {
       draw_program = program_nv12_;
   }

   glUseProgram(draw_program);
   //使用帧缓存 glBindFramebuffer
   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
   glClearColor(0., 0., 0., 1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   //配置0号纹理
   //glActiveTexture(GL_TEXTURE0);
   //glBindTexture(GL_TEXTURE_2D, texture1_);
   //memset(pix_buf_, 128, 33177600);
   int w, h, nrChannels;
   //stbi_set_flip_vertically_on_load(true);
   //unsigned char* data = stbi_load("tmp.bmp", &w, &h, &nrChannels, 0);
   //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, pix_buf_);
   //stbi_image_free(data);
   {
       //test
       //memset(pix_buf_, 0, 33177600);
       //glReadPixels(0, 0, videoFrame->width(), videoFrame->height(), GL_RGBA, GL_UNSIGNED_BYTE, pix_buf_);
       //bmp_write(pix_buf_, pic_w_, pic_h_, 4, R"(d:\xytemp\tmp)");
   }

   // draw our first triangle
   glBindVertexArray(VAO_[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
   //设置0号纹理
   //glUniform1i(glGetUniformLocation(program_, "ourTexture"), 0);



   float ang = model_ang_;
   glm::mat4 mode(1.);
   mode = glm::translate(mode, glm::vec3(0.f, 0.f, 0.f));
   mode = glm::rotate(mode, glm::radians(ang), glm::vec3(0.0, 0.0, 1.0));
   mode = glm::scale(mode, glm::vec3(1., 1., 1.));

   glm::mat4 view(1.);
   // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
   //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
   view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
       glm::vec3(0.0f, 0.0f, 0.0f),
       glm::vec3(0.0f, 1.0f, 0.0f));

   //透视投影
   //glm::mat4 proj = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 100.0f);

   //正交投影
   glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f , 0.1f, 100.f);

   glUniformMatrix4fv(glGetUniformLocation(draw_program, "model"), 1, GL_FALSE, glm::value_ptr(mode));
   glUniformMatrix4fv(glGetUniformLocation(draw_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
   glUniformMatrix4fv(glGetUniformLocation(draw_program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

   GLuint u_offset_pos, texSize_pos, front_pos;
   u_offset_pos = glGetUniformLocation(draw_program, "u_offset");
   texSize_pos = glGetUniformLocation(draw_program, "texSize");
   front_pos = glGetUniformLocation(draw_program, "front");
   static float offset = 0.f;
   if (u_offset_pos != GL_INVALID_INDEX) {
       glUniform1f(u_offset_pos, offset);
       offset += 1.f;
       if (offset > height_) {
           offset = 0.f;
       }
   }
   if (texSize_pos != GL_INVALID_INDEX) {
       GLfloat texSize[] = { 16.f, 16.f };
       glUniform2fv(texSize_pos, 1, texSize);
   }
   if (front_pos != GL_INVALID_INDEX) {
       glUniform1i(front_pos, 0);
   }

   //绘制场景中的物件
   //glDrawElements(GL_TRIANGLES, _scene_draw_indice_sum, GL_UNSIGNED_INT, 0);
   int idx = 0;
   for (auto it = _indarray.begin(); it != _indarray.end(); ++it, ++idx) {
       unsigned char* pix_buf = pix_buf_array_[idx]->Data();
       if (pix_buf == nullptr) {
           continue;
       }
       //以下开始yuv渲染
       assert(textureY_ && textureU_ && textureV_);
       if (_pix_fmt == PIXEL_FORMAT::RENDER_YUV420P) {
           glActiveTexture(GL_TEXTURE1);
           glBindTexture(GL_TEXTURE_2D, textureY_);
           glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pic_w_, pic_h_, GL_LUMINANCE, GL_UNSIGNED_BYTE, pix_buf);
           //设置1号纹理
           glUniform1i(glGetUniformLocation(draw_program, "s_textureY"), 1);

           glActiveTexture(GL_TEXTURE2);
           glBindTexture(GL_TEXTURE_2D, textureU_);
           glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pic_w_ / 2, pic_h_ / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE, pix_buf + pic_w_ * pic_h_);
           //设置2号纹理
           glUniform1i(glGetUniformLocation(draw_program, "s_textureU"), 2);

           glActiveTexture(GL_TEXTURE3);
           glBindTexture(GL_TEXTURE_2D, textureV_);
           glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pic_w_ / 2, pic_h_ / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE, pix_buf + pic_w_ * pic_h_ * 5 / 4);
           //设置3号纹理
           glUniform1i(glGetUniformLocation(draw_program, "s_textureV"), 3);
       }
       else if (_pix_fmt == PIXEL_FORMAT::RENDER_NV12) {
           glActiveTexture(GL_TEXTURE1);
           glBindTexture(GL_TEXTURE_2D, textureY_);
           glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pic_w_, pic_h_, GL_RED, GL_UNSIGNED_BYTE, pix_buf);
           //设置1号纹理
           glUniform1i(glGetUniformLocation(draw_program, "s_textureY"), 1);

           glActiveTexture(GL_TEXTURE2);
           glBindTexture(GL_TEXTURE_2D, textureU_);
           glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pic_w_ / 2, pic_h_ / 2, GL_RG, GL_UNSIGNED_BYTE, pix_buf + pic_w_ * pic_h_);
           //设置2号纹理
           glUniform1i(glGetUniformLocation(draw_program, "s_textureU"), 2);
       }


       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(float) * 6 * idx));
   }

   /*
   {
       unsigned char* tmp = new unsigned char[width_ * 4 * height_];
       glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
       char filenam[255] = { 0 };
       sprintf(filenam, "d:\\tmp\\aaa.bmp");
       bmp_write(tmp, width_, height_, 4, filenam);
       delete[]tmp;
   }
   */

   //unsigned char* tmpbuf = (unsigned char*)malloc(19120128);
   //glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, tmpbuf);
   //bmp_write(tmpbuf, width_, height_, 4, R"(d:\xytemp\tmp)");
   //free(tmpbuf);
   {
       glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
       glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
       glClear(GL_COLOR_BUFFER_BIT);
       glUseProgram(fb_program_);
       glDisable(GL_DEPTH_TEST);

       glBindVertexArray(VAO_[1]);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D, texColorBuffer_);

       //设置0号纹理
       glUniform1i(glGetUniformLocation(fb_program_, "ourTexture"), 0);
       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }


   glBindTexture(GL_TEXTURE_2D, 0);
   eglSwapBuffers(hDisp_, surfRet_);
   glFlush();
   glUseProgram(0);
   //eglWaitGL();
   eglMakeCurrent(hDisp_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   eglWaitClient();
}

void eVideoRender::rotate(int width, int height, float ang) {
   setrect(width, height);
   model_ang_ = ang;
}

void eVideoRender::setrect(int width, int height) {
   Destroy();
   width_ = width;
   height_ = height;
   Init((void*)hwnd_);
}

void eVideoRender::filldata(std::vector<unsigned char>& data, int len, int width, int height, PIXEL_FORMAT fmt) {
   nbase::StdClosure fn = ToWeakCallback(nbase::Bind([=](PIXEL_FORMAT fmt)->void {
       if (done_) {
           return;
       }
       acture_size_ = data.size();
       //memcpy_s(pix_buf_, PIX_BUF_SIZE, &(data[0]), acture_size_);
       for (auto it = pix_buf_array_.begin(); it != pix_buf_array_.end(); ++it) {
           (*it)->PutData(&(data[0]), data.size());
       }
       //pic_w_ = width;
       //pic_h_ = height;
       CheckYUVTexture(width, height, fmt);

       //if (pix_buf_array_.size()) {
           //不渲染,使用定时器间隔渲染
           //OnFrame(nullptr, acture_size_, pic_w_, pic_h_);
       //}

       if (!_b_timer_start) {
           _b_timer_start = true;
           //nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadGL, ToWeakCallback(nbase::Bind(&eGLRender::timerdraw, this)),
           //	nbase::TimeDelta::FromMilliseconds(1000));
           nbase::ThreadManager::PostTask((int)ThreadId::kThreadGL, nbase::Bind(&eVideoRender::timerdraw, this));
       }

       }, fmt));
   nbase::ThreadManager::PostTask((int)ThreadId::kThreadGL, fn);
}

void eVideoRender::Close() {
   done_ = true;
   //nbase::ThreadManager::PostTask((int)ThreadId::kThreadGL,
   //	nbase::Bind(&eGLRender::preClose, this));
   //nbase::ThreadManager::PostTask((int)ThreadId::kThreadGL, nbase::Bind(&eGLRender::destructSelf, this));
   //std::unique_lock<std::mutex> lock(close_mtx_);
   //thread_close_.wait_for(lock, std::chrono::milliseconds(3000));
   //thread_close_.wait(lock);
   //WaitForSingleObject(_hCloseEvent, 5000);
   //delete []pix_buf_;
   //PostMessage(hParent, WM_CLOSE, 0, 0);
}

bool eVideoRender::CreateFBO(size_t width, size_t height) {
   bool ret = true;
   if (framebuffer_) {
       glDeleteFramebuffers(1, &framebuffer_);
       framebuffer_ = 0;
   }
   glGenFramebuffers(1, &framebuffer_);
   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

   //创建纹理附件 生成离屏纹理
   if (texColorBuffer_) {
       eglReleaseTexImage(hDisp_, surfRet_, texColorBuffer_);
       glDeleteTextures(1, &texColorBuffer_);
       texColorBuffer_ = 0;
   }
   glGenTextures(1, &texColorBuffer_);
   glBindTexture(GL_TEXTURE_2D, texColorBuffer_);

   //注意这里data参数传了null，仅仅分配了内存，并没有填充
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

   //也不用关系环绕方式和多级渐变纹理
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // 纹理过滤函数, GL_TEXTURE_WRAP_S S方向上的贴图模式
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // 纹理过滤函数, GL_TEXTURE_WRAP_T T方向上的贴图模式
   glBindTexture(GL_TEXTURE_2D, 0);

   // 将它附加到当前绑定的帧缓冲对象
    /**
    * @brief 将纹理attach到帧缓冲上
    * 第1个参数：帧缓冲的target
    * 第2个参数：附件类型，最后的0意味着可以添加多个颜色附件
    * 第3个参数：附加的纹理类型
    * 第4个参数：附加的纹理本身
    * 第5个参数：多级渐变纹理的级别，这里设置为0
    */
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer_, 0);

   if (rbo_) {
       glDeleteRenderbuffers(1, &rbo_);
       rbo_ = 0;
   }
   //创建rbo附件
   //优点：将数据储存为OpenGL原生的渲染格式，它是为离屏渲染到帧缓冲优化过的。
   glGenRenderbuffers(1, &rbo_);
   glBindRenderbuffer(GL_RENDERBUFFER, rbo_);

   //创建一个深度和模板rbo
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
   glBindRenderbuffer(GL_RENDERBUFFER, 0);

   /**
    * @brief 绑定fbo
    * 第1个参数：帧缓冲的target
    * 第2个参数：附件类型，这里是深度/模板附件
    * 第3个参数：附件的类型，指定RBO
    * 第4个参数：附加的rbo本身
    */
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
       std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
       ret = false;
   }
   glBindFramebuffer(GL_FRAMEBUFFER, 0); //解绑帧缓冲
   return ret;
}

void eVideoRender::ResizeTexture(size_t width, size_t height) {
   //纹理
   eglMakeCurrent(hDisp_, surfRet_, surfRet_, context_);
   glFlush();
   if (textureY_) {
       eglReleaseTexImage(hDisp_, surfRet_, textureY_);
       glDeleteTextures(1, &textureY_);
       textureY_ = 0;
   }
   if (textureU_) {
       eglReleaseTexImage(hDisp_, surfRet_, textureU_);
       glDeleteTextures(1, &textureU_);
       textureU_ = 0;
   }
   if (textureV_) {
       eglReleaseTexImage(hDisp_, surfRet_, textureV_);
       glDeleteTextures(1, &textureV_);
       textureV_ = 0;
   }
   if (_pix_fmt == PIXEL_FORMAT::RENDER_YUV420P) {
       greate_yuv_texture(textureY_, width, height, GL_LUMINANCE);
       greate_yuv_texture(textureU_, width / 2, height / 2, GL_LUMINANCE);
       greate_yuv_texture(textureV_, width / 2, height / 2, GL_LUMINANCE);
       eglMakeCurrent(hDisp_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   }
   else if (_pix_fmt == PIXEL_FORMAT::RENDER_NV12) {
       greate_yuv_texture(textureY_, width, height, GL_RED);
       greate_yuv_texture(textureU_, width / 2, height / 2, GL_RG);
       greate_yuv_texture(textureV_, width / 2, height / 2, GL_RG);
       eglMakeCurrent(hDisp_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   }

   pic_w_ = width;
   pic_h_ = height;
}

void eVideoRender::Destroy(bool bDone) {
   _elearray.clear();
   _indarray.clear();
   _scene_draw_indice_sum = 0;
   eglMakeCurrent(hDisp_, surfRet_, surfRet_, context_);
   glFlush();
   glBindVertexArray(0);
   glBindTexture(GL_TEXTURE_2D, 0);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glUseProgram(0);
   if (texture1_) {
       eglReleaseTexImage(hDisp_, surfRet_, texture1_);
       glDeleteTextures(1, &texture1_);
       texture1_ = 0;
   }
   if (textureY_) {
       eglReleaseTexImage(hDisp_, surfRet_, textureY_);
       glDeleteTextures(1, &textureY_);
       textureY_ = 0;
   }
   if (textureU_) {
       eglReleaseTexImage(hDisp_, surfRet_, textureU_);
       glDeleteTextures(1, &textureU_);
       textureU_ = 0;
   }
   if (textureV_) {
       eglReleaseTexImage(hDisp_, surfRet_, textureV_);
       glDeleteTextures(1, &textureV_);
       textureV_ = 0;
   }
   glDeleteVertexArrays(sizeof(VAO_) / sizeof(VAO_[0]), VAO_);
   glDeleteBuffers(1, &vbo_);
   glDeleteBuffers(1, &ebo_);
   glDeleteBuffers(1, &v_square_bo_);
   glDeleteBuffers(1, &febo_);
   glDeleteProgram(program_);
   glDeleteProgram(program_nv12_);
   if (fb_program_) {
       glDeleteProgram(fb_program_);
       fb_program_ = 0;
   }
   if (framebuffer_) {
       glDeleteFramebuffers(1, &framebuffer_);
       framebuffer_ = 0;
   }
   if (texColorBuffer_) {
       eglReleaseTexImage(hDisp_, surfRet_, texColorBuffer_);
       glDeleteTextures(1, &texColorBuffer_);
       texColorBuffer_ = 0;
   }
   if (rbo_) {
       glDeleteRenderbuffers(1, &rbo_);
       rbo_ = 0;
   }
   eglMakeCurrent(hDisp_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

   eglDestroySurface(hDisp_, surfRet_);
   surfRet_ = EGL_NO_SURFACE;
   eglDestroyContext(hDisp_, context_);
   context_ = EGL_NO_CONTEXT;

   eglTerminate(hDisp_);
   hDisp_ = EGL_NO_DISPLAY;
   //eglWaitGL();
   if (bDone) {
       pix_buf_array_.clear();
   }
   if (_hInitEvents[0]) {
       neosmart::DestroyEvent(_hInitEvents[0]);
       _hInitEvents[0] = 0;
   }
   if (_hInitEvents[1]) {
       neosmart::DestroyEvent(_hInitEvents[1]);
       _hInitEvents[1] = 0;
   }
}

void eVideoRender::timerdraw() {
   if (!done_) {
       if (pix_buf_array_.size()) {
           CheckYUVTexture(pic_w_, pic_h_, _pix_fmt);
           //long long ts = utime_ms();
           OnFrame(nullptr, acture_size_, pic_w_, pic_h_);
           //printf("onframe ms: %lld(ms)\n", utime_ms()-ts);
       }
       nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadGL,
           nbase::Bind(&eVideoRender::timerdraw, this),
           nbase::TimeDelta::FromMilliseconds(33)); //16ms (60fps) 33ms(30fps) 250ms
   }
   else {
       Destroy(true);
       //std::unique_lock<std::mutex> lock(close_mtx_);
       //thread_close_.notify_one();
       //SetEvent(_hCloseEvent);
       //PostMessage(hParentWin_, WM_CLOSE, 1, 0);
   }
}

void eVideoRender::destructSelf(eVideoRender* render) {
   render->Destroy();
   delete render;
}

void eVideoRender::preClose() {
   done_ = true;
}

void eVideoRender::CheckYUVTexture(const int width, const int height, const PIXEL_FORMAT fmt) {
   bool bdifferent_size = static_cast<size_t>(width) != pic_w_ ||
       static_cast<size_t>(height) != pic_h_ ||
       fmt != _pix_fmt;
   if (bdifferent_size || textureY_ == 0 || textureU_ == 0 || textureV_ == 0) {
       _pix_fmt = fmt;
       ResizeTexture(static_cast<size_t>(width),
           static_cast<size_t>(height));
   }
}
