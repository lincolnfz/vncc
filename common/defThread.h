#pragma once
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
