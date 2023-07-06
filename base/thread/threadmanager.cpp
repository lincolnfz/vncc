#include "threadmanager.h"

namespace nbase {

ThreadManager* ThreadManager::s_inst = nullptr;

ThreadManager::ThreadManager()
{

}

ThreadManager::~ThreadManager(){

}

ThreadManager* ThreadManager::GetInstance(){
    if(ThreadManager::s_inst == nullptr){
        ThreadManager::s_inst = new ThreadManager;
    }
    return s_inst;
}

bool ThreadManager::RegisterThread(int self_identifier, std::shared_ptr<Thread> sp){
    bool bret = false;
    std::shared_ptr<Thread> got = GetInstance()->findThd(self_identifier);
    if(got){
        bret = false;
    }else{
        GetInstance()->insertThd(self_identifier, sp);
    }
    return  bret;
}

bool ThreadManager::PostTask(int identifier, const StdClosure &task){
    bool bret = false;
    std::shared_ptr<Thread> sp = GetInstance()->findThd(identifier);
    if(sp){
        sp->SubmitTask(task);
        bret = true;
    }
    return bret;
}

bool ThreadManager::PostDelayedTask(int identifier, const StdClosure &task, TimeDelta delay){
    bool bret = false;
    int64_t delay_ms = delay.ToMilliseconds();
    std::shared_ptr<Thread> sp = GetInstance()->findThd(identifier);
    if(sp){
        sp->SubmitDelayTask(task, delay_ms);
        bret = true;
    }
    return bret;
}

bool ThreadManager::PostRepeatedTask(int identifier, const StdClosure& task,
                                     const TimeDelta& delay, int times /*= 1*/){
    bool bret = false;
    int64_t delay_ms = delay.ToMilliseconds();
    std::shared_ptr<Thread> sp = GetInstance()->findThd(identifier);
    if(sp){
        sp->SubmitRepeatedTask(task, delay_ms, delay_ms, times);
        bret = true;
    }
    return bret;
}

void ThreadManager::insertThd(int identifier, std::shared_ptr<Thread> sp){
    _threads.insert(std::make_pair(identifier, sp));
}

std::shared_ptr<Thread> ThreadManager::findThd(int identifier){
    std::shared_ptr<Thread> sp;
    auto got = _threads.find(identifier);
    if(got != _threads.end()){
        sp = got->second;
    }
    return sp;
}

}
