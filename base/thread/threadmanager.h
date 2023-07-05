#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include <map>
#include "base/callback/callback.h"
#include "base/thread/thread.h"
#include "base/time/time.h"

namespace nbase{

class ThreadManager
{
public:
    virtual ~ThreadManager();

    static ThreadManager* GetInstance();

    static bool RegisterThread(int self_identifier, std::shared_ptr<Thread> sp);
    // 取消当前线程托管
    // 线程运行结束之前必须调用UnregisterThread取消托管
    static bool UnregisterThread();

    static bool PostTask(int identifier, const StdClosure &task);
    static bool PostDelayedTask(int identifier, const StdClosure &task, TimeDelta delay);
    static bool PostRepeatedTask(int identifier, const StdClosure& task,
                                 const TimeDelta& delay, int times = 1);

private:
    ThreadManager();
    void insertThd(int identifier, std::shared_ptr<Thread> sp);
    std::shared_ptr<Thread> findThd(int identifier);
    static ThreadManager* s_inst;

    std::map<int, std::shared_ptr<Thread>> _threads;
};

}

#endif // THREADMANAGER_H
