#ifndef THREAD_H
#define THREAD_H
#include <mutex>
#include <list>
#include <queue>
#include <thread>
#include <condition_variable>
#include <chrono>
#include "base/callback/callback.h"
#include "base/misctool.h"
#include "base/event/pevents.h"

namespace nbase {

class TaskQueue{
    public:
        struct PendingTask
        {
            PendingTask(const StdClosure &task):_std_task(task){
                _sequence_num = 0;
                _delayed_tick = 0;
                _times = 1;
                _interval = 0;
            };

            ~PendingTask(){};

            // 用于优先队列的排序，std::heap默认为大顶堆，
            // 而我们要的是小顶堆，所以这个操作符重载实际得返回大于的比较结果
            bool operator<(const PendingTask& other) const{
                if (_delayed_tick > other._delayed_tick)
                    return true;
                if (_delayed_tick < other._delayed_tick)
                    return false;
                return _sequence_num > other._sequence_num;
            };

            void Run()
            {
                if (!_std_task.Expired() && _std_task.t_) {
                    _std_task.t_();
                }
                else {
                    //assert(false);
                }
            };

        public:
            StdClosure _std_task;
            // 定时任务序号，可作为定时任务的第二排序键，非定时任务此项无效
            unsigned long _sequence_num;
            long _delayed_tick; //提交后延迟多少执行(ms)
            int _times; //可以运行几次
            int _interval; //间隔运行(ms)
        };

    void pushTask(const PendingTask& task){
        PendingTask new_task(task);
        std::unique_lock<std::mutex> lock(_queue_mutex);
        new_task._sequence_num = _sequence_num++;
        _queue.push(new_task);
    };

    PendingTask getTask(){
        std::unique_lock<std::mutex> lock(_queue_mutex);
        PendingTask task = _queue.top();
        _queue.pop();
        return task;
    };

    void checkTopTask(unsigned long& sequence_num, long& trigger_tick ){
        std::unique_lock<std::mutex> lock(_queue_mutex);
        const PendingTask& task = _queue.top();
        sequence_num = task._sequence_num;
        trigger_tick = task._delayed_tick;
    }

    /*void popTask(){
        std::unique_lock<std::mutex> lock(_queue_mutex);
        _queue.pop();
    }*/

    int Get_Length(){
        std::unique_lock<std::mutex> lock(_queue_mutex);
        return _queue.size();
    };

protected:
    std::priority_queue<PendingTask> _queue;
    std::mutex _queue_mutex;
    unsigned long _sequence_num = 0;
    long _ts_tick = 0;
};

class ThreadManager;
class Thread
{
public:
    friend ThreadManager;
    Thread(){

    }
    virtual ~Thread(){};

    void RequestStop(){
            _ASSERT(std::this_thread::get_id() == _self_tid);
            _done = true;
        }

    /**
    * 虚函数，初始化主线程
    * @return void	无返回值
    */
    virtual void Init(){};

    /**
    * 虚函数，主线程退出时，做一些清理工作
    * @return void	无返回值
    */
    virtual void Cleanup(){};

    void StartWithLoop(){
        mutex = PTHREAD_MUTEX_INITIALIZER; /*初始化互斥锁*/
        cond = PTHREAD_COND_INITIALIZER; //初始化条件变量
        pthread_t t_pump;
        pthread_create(&t_pump, nullptr, &Thread::helpPump, this);
        //pthread_join(t_pump, nullptr);
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);/*等待*/
        pthread_mutex_unlock(&mutex);
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);

    };

    void Start(){
        std::thread thd(&Thread::RunMsgPump, this);
        thd.detach();
    }

protected:
    static void* helpPump(void *parm){
        Thread* self = reinterpret_cast<Thread*>(parm);
        pthread_mutex_lock(&self->mutex);
        self->RunMsgPump();
        pthread_cond_signal(&self->cond);
        pthread_mutex_unlock(&self->mutex);
        return 0;
    }

    void SubmitTask(const nbase::StdClosure &task){
        TaskQueue::PendingTask new_task(task);
        _task_queue.pushTask(new_task);
    };

    void SubmitDelayTask(const nbase::StdClosure &task, const long delayms){
        TaskQueue::PendingTask new_task(task);
        new_task._delayed_tick = getNowTickMs() + delayms;
        _delayed_task_queue.pushTask(new_task);
    }

    void SubmitRepeatedTask(const nbase::StdClosure &task, const long delayms, const int interval_ms, const int times){
        TaskQueue::PendingTask new_task(task);
        new_task._delayed_tick = getNowTickMs() + delayms;
        new_task._interval = interval_ms;
        new_task._times = times;
        _delayed_task_queue.pushTask(new_task);
    }

private:
    void RunMsgPump(){
        _self_tid = std::this_thread::get_id();
        Init();
        while(!_done){
            std::unique_lock<std::mutex> lock(_data_lock);
            //处理事务task
            while(_task_queue.Get_Length() > 0){
                _task_queue.getTask().Run();
            }

            //处理延时事务
            unsigned long sequence_num = 0;
            long trigger_tick = 0;
            long now_tick = getNowTickMs();
            while(_delayed_task_queue.Get_Length() > 0){
                _delayed_task_queue.checkTopTask(sequence_num, trigger_tick);
                if(now_tick > trigger_tick){
                    bool bRepeated = false;
                    TaskQueue::PendingTask task = _delayed_task_queue.getTask();
                    task.Run();
                    if(task._times == -1){
                        task._delayed_tick = now_tick + task._interval;
                        bRepeated = true;
                    }else{
                        --(task._times);
                        if(task._times > 0){
                            task._delayed_tick = now_tick + task._interval;
                            bRepeated = true;
                        }
                    }
                    if(bRepeated){
                        _delayed_task_queue.pushTask(task);
                    }

                }else{
                    break;
                }
            }

            //while(_queue.Get_Length() == 0){
                //有可能收到假事件,所以再加上列队长度判断
                //_thread_con.wait(lock);
                _thread_con.wait_for(lock, std::chrono::milliseconds(_millisec_timeout));
            //}
        }
        Cleanup();
    };

private:
    TaskQueue _task_queue;
    TaskQueue _delayed_task_queue;
    std::mutex _data_lock;
    std::condition_variable _thread_con;
    std::thread::id _self_tid; //自已所在的线程id
    bool _done = false;
    int _millisec_timeout = 5;
    pthread_mutex_t mutex;// = PTHREAD_MUTEX_INITIALIZER; /*初始化互斥锁*/
    pthread_cond_t cond;// = PTHREAD_COND_INITIALIZER; //初始化条件变量
};

}

#endif // THREAD_H
