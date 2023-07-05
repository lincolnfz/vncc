#ifndef BASE_UTIL_CALLBACK_H_
#define BASE_UTIL_CALLBACK_H_
#pragma once

#include <memory>
#include <functional>
//#include "base/base_export.h"

//typedef std::function<void(void)> StdClosure;

namespace nbase
{

class WeakFlag
{

};

template<typename T>
class WeakCallback
{
public:
	WeakCallback(const std::weak_ptr<WeakFlag>& weak_flag, const T& t) :
		weak_flag_(weak_flag),
		t_(t)
	{

	}

	WeakCallback(const std::weak_ptr<WeakFlag>& weak_flag, T&& t) :
		weak_flag_(weak_flag),
		t_(std::move(t))
	{

	}

	template<class WeakType>
	WeakCallback(const WeakType& weak_callback) :
		weak_flag_(weak_callback.weak_flag_),
		t_(weak_callback.t_)
	{

	}

	template<class... Args>
	auto operator ()(Args && ... args) const
#if _MSC_VER > 1900
#else
		->decltype(t_(std::forward<Args>(args)...))
#endif
	{
		if (!weak_flag_.expired()) {
			return t_(std::forward<Args>(args)...);
		}
		return decltype(t_(std::forward<Args>(args)...))();
	}

	bool Expired() const
	{
		return weak_flag_.expired();
	}


	std::weak_ptr<WeakFlag> weak_flag_;
	mutable T t_;
};

typedef WeakCallback<std::function<void(void)>> StdClosure;
typedef WeakCallback<std::function<bool(void)>> StdBlockTaskClosure;

class  SupportWeakCallback
{
public:
	typedef std::weak_ptr<WeakFlag> _TyWeakFlag;
public:
	virtual ~SupportWeakCallback(){};

	template<typename CallbackType>
	auto ToWeakCallback(const CallbackType& closure)
		->WeakCallback<CallbackType>
	{
		return WeakCallback<CallbackType>(GetWeakFlag(), closure);
	}

	// 
	template<class F, class... Args>
	auto ToWeakCallParmsback(F&& f, Args && ... args) 
		->WeakCallback<decltype(std::bind(f, args...))>
	{
		std::weak_ptr<WeakFlag> weak_flag = GetWeakFlag();
		auto bind_obj = std::bind(f, args...);
		//static_assert(std::is_base_of<nbase::SupportWeakCallback, C>::value, "nbase::SupportWeakCallback should be base of C");
		WeakCallback<decltype(bind_obj)> weak_callback(weak_flag, std::move(bind_obj));
		return weak_callback;
	}

	std::weak_ptr<WeakFlag> GetWeakFlag()
	{
		if (m_weakFlag.use_count() == 0) {
			m_weakFlag.reset((WeakFlag*)nullptr);
		}
		return m_weakFlag;
	}

private:
	template<typename ReturnValue, typename... Param, typename WeakFlag>
	static std::function<ReturnValue(Param...)> ConvertToWeakCallback(
		const std::function<ReturnValue(Param...)>& callback, std::weak_ptr<WeakFlag> expiredFlag)
	{
		auto weakCallback = [expiredFlag, callback](Param... p) {
			if (!expiredFlag.expired()) {
				return callback(p...);
			}
			return ReturnValue();
		};

		return weakCallback;
	}

protected:
	std::shared_ptr<WeakFlag> m_weakFlag;
};

//WeakCallbackFlag一般作为类成员变量使用，要继承，可使用不带Cancel()函数的SupportWeakCallback
//这里禁止继承，主要担心误用。当使用这个类的功能，打包出多个支持weak语义的callback时，一旦错误的调用了Cancel，
//将会取消所有callback，这种情况可能不是用户希望的。此时，应该使用多个不带Cancel函数的WeakCallbackFlag类型的成员变量，
//每个对应一个callback，一一对应的控制每个支持weak语义的callback。
class WeakCallbackFlag final : public SupportWeakCallback
{
public:
	void Cancel()
	{
		m_weakFlag.reset();
	}

	bool HasUsed()
	{
		return m_weakFlag.use_count() != 0;
	}
};


// global function 
template<class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
auto Bind(F && f, Args && ... args)
	->decltype(std::bind(f, args...))
{
	return std::bind(f, args...);
}

// const class member function 
template<class R, class C, class... DArgs, class P, class... Args>
auto Bind(R(C::*f)(DArgs...) const, P && p, Args && ... args)
	->WeakCallback<decltype(std::bind(f, p, args...))>
{
	std::weak_ptr<WeakFlag> weak_flag = ((SupportWeakCallback*)p)->GetWeakFlag();
	auto bind_obj = std::bind(f, p, args...);
	static_assert(std::is_base_of<nbase::SupportWeakCallback, C>::value, "nbase::SupportWeakCallback should be base of C");
	WeakCallback<decltype(bind_obj)> weak_callback(weak_flag, std::move(bind_obj));
	return weak_callback;
}

// non-const class member function 
template<class R, class C, class... DArgs, class P, class... Args>
auto Bind(R(C::*f)(DArgs...), P && p, Args && ... args) 
	->WeakCallback<decltype(std::bind(f, p, args...))>
{
	std::weak_ptr<WeakFlag> weak_flag = ((SupportWeakCallback*)p)->GetWeakFlag();
	auto bind_obj = std::bind(f, p, args...);
	static_assert(std::is_base_of<nbase::SupportWeakCallback, C>::value, "nbase::SupportWeakCallback should be base of C");
	WeakCallback<decltype(bind_obj)> weak_callback(weak_flag, std::move(bind_obj));
	return weak_callback;
}

}


//other version //muduo
namespace muduo{
template <typename CLASS, typename... ARGS> class WeakCallback {
public:
  WeakCallback(const std::weak_ptr<CLASS> &object,
               const std::function<void(CLASS *, ARGS...)> &function)
      : object_(object), function_(function) {}

  // Default dtor, copy ctor and assignment are okay

  void operator()(ARGS &&... args) const {
    std::shared_ptr<CLASS> ptr(object_.lock());
    if (ptr) {
      function_(ptr.get(), std::forward<ARGS>(args)...);
    }
    // else
    // {
    //   LOG_TRACE << "expired";
    // }
  }

private:
  std::weak_ptr<CLASS> object_;
  std::function<void(CLASS *, ARGS...)> function_;
};

template <typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...>
makeWeakCallback(const std::shared_ptr<CLASS> &object,
                 void (CLASS::*function)(ARGS...)) {
    return WeakCallback<CLASS, ARGS...>(object, function);
}

template <typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...>
makeWeakCallback(const std::shared_ptr<CLASS> &object,
                 void (CLASS::*function)(ARGS...) const) {
                 //这里函数指针和std::function<void(CLASS *, ARGS...)
  return WeakCallback<CLASS, ARGS...>(object, function);
}
}

#endif  // BASE_UTIL_CALLBACK_H_