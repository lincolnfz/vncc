#include "playdialog.h"
#include "common/defThread.h"

PlayDialog::PlayDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f) {
    //connect(this, &PlayDialog::heightChanged , this, &PlayDialog::handle_heightChange);
    this->winId();
}

PlayDialog::~PlayDialog(){

}

void PlayDialog::handle_heightChange(int arg){
    this->isWindow();
}

void PlayDialog::resizeEvent(QResizeEvent * resize_ev){
    const QSize rc = resize_ev->size();
    if(_video_render == nullptr){
        void * p = nullptr;
        int s = sizeof(p);
        _video_render = eVideoRender::Create((void *)winId(), rc.width(), rc.height(), 0.f);
        nbase::StdClosure fn = nbase::Bind(&PlayDialog::testgl, this);
        nbase::ThreadManager::PostDelayedTask((int)ThreadId::kThreadGL, fn, nbase::TimeDelta::FromSeconds(1));
    }
}

void PlayDialog::testgl() {
    FILE* yuvfile;
    yuvfile = fopen("/home/lin/1280x720.yuv", "rb");
    if (yuvfile != 0) {
        fseek(yuvfile, 0, SEEK_END);
        long flen = ftell(yuvfile);
        fseek(yuvfile, 0, SEEK_SET);
        unsigned char* szbuf = new unsigned char[flen];
        fread(szbuf, 1, flen, yuvfile);
        //_gl_render->OnFrame(szbuf, flen, 1280, 720);
        std::vector<unsigned char> yuvdata(szbuf, szbuf+flen);
        _video_render->filldata(yuvdata, flen, 1280, 720, eVideoRender::PIXEL_FORMAT::RENDER_YUV420P);
        delete[] szbuf;
        fclose(yuvfile);
    }
}
