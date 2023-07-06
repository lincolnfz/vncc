#ifndef EVIDEORENDER_H
#define EVIDEORENDER_H
#include "base/base.h"
#include <myangle/GLES3/gl32.h>
#include <myangle/GLES2/gl2ext.h>
#include <myangle/GLES3/gl3platform.h>

#include <myangle/EGL/egl.h>
#include <myangle/EGL/eglext.h>
#include <myangle/EGL/eglext_angle.h>
#include <myangle/EGL/eglplatform.h>
#include <base/event/pevents.h>

class eVideoRender : public nbase::SupportWeakCallback
{
public:
    eVideoRender(const eVideoRender&) = delete;
    virtual ~eVideoRender();

public:
    enum class PIXEL_FORMAT {
        RENDER_UNKNOW,
        RENDER_YUV420P,
        RENDER_NV12,
    };
    static std::shared_ptr<eVideoRender> Create(const void* hwnd, size_t width,
        size_t height, float model_ang);

    void rotate(int width, int height, float ang);

    void setrect(int width, int height);

    void filldata(std::vector<unsigned char>&, int len, int width, int height, PIXEL_FORMAT fmt);

    void Close();

private:
    struct Signle_Square {
        float tr[8] = { 1.f,  1.f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0, 0.0 };
        float br[8] = { 1.f, -1.f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 1.0 };
        float bl[8] = { -1.f, -1.f, 0.0f, 0.0f, 0.0f, 1.0f, 0., 1. };
        float tl[8] = { -1.f,  1.f, 0.0f, 1.0f, 0.0f, 0.0f, 0., 0.0 };
    };

    struct Draw_Indices {
        unsigned int tri_frist[3] = { 0, 1, 3 };
        unsigned int tri_second[3] = { 1, 2, 3 };
    };

    class Pix_Ele {
    public:
        Pix_Ele() {
            _dataptr = nullptr;
            _datalen = 0;
        }
        virtual ~Pix_Ele() {
            FreeMemory();
        }
        unsigned char* Data() {
            return _dataptr;
        }
        int Datalen() {
            return _datalen;
        }
        void PutData(const unsigned char* data, const int len) {
            if (_datalen < len) {
                FreeMemory();
                AllocMemory(len);
            }
            memcpy(_dataptr, data, len);
        }
        void FreeMemory() {
            if (_dataptr) {
                free(_dataptr);
                _dataptr = nullptr;
            }
            _datalen = 0;
        }
    private:
        void AllocMemory(int len) {
            FreeMemory();
            _dataptr = (unsigned char*)malloc(len);
            _datalen = len;
        }
        unsigned char* _dataptr = nullptr;
        int _datalen = 0;
    };

    eVideoRender(size_t width, size_t height, float model_ang);
    void OnFrame(const unsigned char* buf, int buf_len, int width, int height);
    bool InitSurface();
    bool Init(const void* hwnd);
    bool CreateFBO(size_t width, size_t height);
    void ResizeTexture(size_t width, size_t height);
    void Destroy(bool bDone = false);
    void timerdraw();
    void greate_yuv_texture(unsigned int& texture, int width, int height, GLint gltype);
    static void destructSelf(eVideoRender*);
    void preClose();
    void Generate_Square_array(int line, int col, std::vector<Signle_Square>& out, std::vector<Draw_Indices>& ind_array);
    void CheckYUVTexture(const int width, const int height, const PIXEL_FORMAT fmt);
    void CleanContext();

    bool init_ = false;
    EGLDisplay hDisp_ = nullptr;
    EGLContext context_ = nullptr;
    EGLSurface surfRet_ = nullptr;
    GLuint program_ = 0;
    GLuint program_nv12_ = 0;
    GLuint fb_program_ = 0;
    GLuint texColorBuffer_ = 0;
    GLuint depthRenderbuffer = 0;
    unsigned int VAO_[2] = {0};
    unsigned int vbo_ = 0;
    unsigned int ebo_ = 0;
    unsigned int febo_ = 0;
    unsigned int v_square_bo_ = 0;
    unsigned int texture1_ = 0;
    unsigned int texture_test_ = 0;
    unsigned int textureY_ = 0;
    unsigned int textureU_ = 0;
    unsigned int textureV_ = 0;

    size_t width_ = 0;
    size_t height_ = 0;
    int fps_ = 0;
    size_t pic_w_ = 0;
    size_t pic_h_ = 0;
    //unsigned char pic_pix_buf_[33177600];

    uint64_t hwnd_ = 0;
    //unsigned char* pix_buf_ = nullptr;// [19120128] ;
    std::vector<unsigned char> pix_buf_;
    std::vector<std::unique_ptr<Pix_Ele>> pix_buf_array_;
    int acture_size_ = 0;
    float model_ang_ = 0.f;
    bool done_ = false; //opengl work done
    //std::mutex close_mtx_;
    //std::condition_variable thread_close_;

    GLuint framebuffer_ = 0;
    GLuint rbo_ = 0;
    int _scene_draw_indice_sum = 0;

    //顶点数组
    std::vector<Signle_Square> _elearray;

    //绘制索引数组
    std::vector<Draw_Indices> _indarray;

    bool _b_single_res = false;

    //渲染定时器启动
    bool _b_timer_start = false;

    PIXEL_FORMAT _pix_fmt = PIXEL_FORMAT::RENDER_YUV420P;

    neosmart::neosmart_event_t _hInitEvents[2] = {nullptr};
    neosmart::neosmart_event_t _hCloseEvent = nullptr;
};

#endif // EVIDEORENDER_H
