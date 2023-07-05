//#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>
#include <thread>
#include <functional>
#include <stdexcept>

#define  THREADPOOL_MAX_NUM 16 
//#define  THREADPOOL_AUTO_GROW


class threadpool
{
	using Task = std::function<void()>;	//��������
	std::vector<std::thread> _pool;     //�̳߳�
	std::queue<Task> _tasks;            //�������
	std::mutex _lock;                   //ͬ��
	std::condition_variable _task_cv;   //��������
	std::atomic<bool> _run{ true };     //�̳߳��Ƿ�ִ��
	std::atomic<int>  _idlThrNum{ 0 };  //�����߳�����

public:
	inline threadpool(unsigned short size = 4) { addThread(size); }
	inline ~threadpool()
	{
		_run=false;
		_task_cv.notify_all(); // ���������߳�ִ��
		for (std::thread& thread : _pool) {
			//thread.detach(); // ���̡߳���������
			if(thread.joinable())
				thread.join(); // �ȴ���������� ǰ�᣺�߳�һ����ִ����
		}
	}

public:
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
	{
		if (!_run) {    // stoped
			throw std::runtime_error("commit on ThreadPool is stopped.");
		}

		using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, ���� f �ķ���ֵ����
		auto task = std::make_shared<std::packaged_task<RetType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		); // �Ѻ�����ڼ�����,���(��)
		std::future<RetType> future = task->get_future();
		{    // ������񵽶���
			std::lock_guard<std::mutex> lock{ _lock };//�Ե�ǰ���������  lock_guard �� mutex �� stack ��װ�࣬�����ʱ�� lock()��������ʱ�� unlock()
			_tasks.emplace([task](){ // push(Task{...}) �ŵ����к���
				(*task)();
			});
		}
#ifdef THREADPOOL_AUTO_GROW
		if (_idlThrNum < 1 && _pool.size() < THREADPOOL_MAX_NUM)
			addThread(1);
#endif // !THREADPOOL_AUTO_GROW
		_task_cv.notify_one(); // ����һ���߳�ִ��

		return future;
	}

	//�����߳�����
	int idlCount() { return _idlThrNum; }
	//�߳�����
	int thrCount() { return _pool.size(); }
#ifndef THREADPOOL_AUTO_GROW
private:
#endif // !THREADPOOL_AUTO_GROW
	//���ָ���������߳�
	void addThread(unsigned short size)
	{
		for (; _pool.size() < THREADPOOL_MAX_NUM && size > 0; --size)
		{   //�����߳�����,�������� Ԥ�������� THREADPOOL_MAX_NUM
			_pool.emplace_back( [this]{ //�����̺߳���
				while (_run)
				{
					Task task; // ��ȡһ����ִ�е� task
					{
						// unique_lock ��� lock_guard �ĺô��ǣ�������ʱ unlock() �� lock()
						std::unique_lock<std::mutex> lock{ _lock };
						_task_cv.wait(lock, [this]{
								return !_run || !_tasks.empty();
						}); // wait ֱ���� task
						if (!_run && _tasks.empty())
							return;
						task = move(_tasks.front()); // ���Ƚ��ȳ��Ӷ���ȡһ�� task
						_tasks.pop();
					}
					--_idlThrNum;
					task();//ִ������
					++_idlThrNum;
				}
			});
                        ++_idlThrNum;
		}
	}
};

#endif  //https://github.com/lzpong/
