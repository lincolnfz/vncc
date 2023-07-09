#ifndef defthread_h
#define defthread_h

enum class ThreadId
{
	kThreadUI = 0,
	kThreadGL = 1,
	kThreadGlobalMisc = 2,
        kThreadRemoteControl = 3,
	kThreadRender = 4, 
	kThreadSend2Main = 5,
        kThreadDecode = 6,
};

#endif
