#include "playdialog.h"
#include "common/defThread.h"
#include <qprocess.h>
#include <qapplication.h>
#include <base/jsonext.hpp>

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
        //_video_render = eVideoRender::Create((void *)winId(), rc.width(), rc.height(), 0.f);
        //RunPlayer();
        nbase::StdClosure fn = nbase::Bind(&PlayDialog::RunPlayer, this);
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

void PlayDialog::RunPlayer(){
    QString app_path = QCoreApplication::applicationDirPath();
    std::string str_app_path = app_path.toStdString();
    char sz[512] = {0};
    sprintf(sz, "%s/erender", str_app_path.c_str());
    QString app_file = QString::fromStdString(sz);
    QStringList arguments;
    nlohmann::json root;
    root["winid"] = winId();
    arguments << QString::fromStdString(root.dump());
    QProcess proces(this);
    proces.startDetached(app_file, arguments);
}
