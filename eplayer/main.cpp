#include <QCoreApplication>
#include <base/base.h>
#include <common/defThread.h>
#include <common/eInstructions.h>
#include <base/event/pevents.h>

class MainThrad : public nbase::Thread {
public:
    MainThrad(): Thread(){};
    virtual ~MainThrad(){};

    virtual void Init() override {

    };

    /**
    * 虚函数，主线程退出时，做一些清理工作
    * @return void	无返回值
    */
    virtual void Cleanup() override {

    };
};

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //return a.exec();
    neosmart::neosmart_event_t hold_event = neosmart::CreateEvent();
    std::shared_ptr<MainThrad> sp_mainThd = std::make_shared<MainThrad>();
    nbase::ThreadManager::RegisterThread((int)ThreadId::kThreadUI, sp_mainThd);
    sp_mainThd->Start();
    neosmart::WaitForEvent(hold_event);
}
