#include "playdialog.h"
#include <QPaintEngine>
#include <myangle/GLES3/gl32.h>
#include <myangle/GLES2/gl2ext.h>
#include <myangle/GLES3/gl3platform.h>

#include <myangle/EGL/egl.h>
#include <myangle/EGL/eglext.h>
#include <myangle/EGL/eglext_angle.h>
#include <myangle/EGL/eglplatform.h>

int angle_back[] = {
    0,
    EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE,
};
EGLDisplay GetDisplay(int idx, void* hdc) {
    EGLDisplay disp = nullptr;
    int engtype = angle_back[idx];
    if (engtype == 0) {
        disp = eglGetDisplay((EGLNativeDisplayType)hdc);
    }
    else {
        /*EGLint displayAttribs[] = {EGL_PLATFORM_ANGLE_TYPE_ANGLE,
                               EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE,
                               EGL_NONE };*/
        std::vector<EGLint> displayAttribs;
        displayAttribs.push_back(EGL_PLATFORM_ANGLE_TYPE_ANGLE);
        displayAttribs.push_back(engtype);
        displayAttribs.push_back(EGL_NONE);

        auto eglGetPlatformDisplayEXT = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
            eglGetProcAddress("eglGetPlatformDisplayEXT"));
        if (eglGetPlatformDisplayEXT) {
            disp = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE,
                reinterpret_cast<void*>(hdc),
                &(displayAttribs[0]));
        }
    }

    return disp;
}


PlayDialog::PlayDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f) {
    QPaintEngine* paintEngine = this->paintEngine();
    GetDisplay(0, (void*)paintEngine);
}

PlayDialog::~PlayDialog(){

}


void PlayDialog::open(){

}
