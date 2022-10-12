#pragma once
#ifndef MYSTL_CONSTRUCT_H_
#define MYSTL_CONSTRUCT_H_

#include <new>

#ifdef _MSC_VER
#pragma warning(push)

#endif // _MSC_VER


//包含构造和析构函数
namespace mystl
{
	template<class T>
	void construct(T* ptr)
	{
		::new ((void*)ptr) T();
	}

	template<class T1, class T2>
	void construct(T1* ptr, const T2& value)
	{
		::new ((void*)ptr) T1(value);
	}

	template<class T1, class... Args>
	void construct(T1* ptr, Args&&... args)
	{
		::new ((void*)ptr) T1(mystl::forward<Args>(args)...);
	}

	template<class T>
	void destroy_one(T*,std::true_type){}

	template<class T>
	void destory_one(T* ptr, std::false_type)
	{
		if (ptr)
		{
			ptr->~T();
		}
	}

	template<class ForwardIter>
	void destroy_cat(ForwardIter,ForwardIter,std::true_type){}

	template<class ForwardIter>
	void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
	{
		for (; first != last; ++first)
		{
			destroy(&*first);
		}
	}

	template<class T>
	void destroy(T* ptr)
	{
		destroy_one(ptr, std::is_trivially_destructible<T>{});
	}

	template<class ForwardIter>
	void destroy(ForwardIter first, ForwardIter last)
	{
		destroy_cat(first, last, std::is_trivially_destructible<typename iterator_traits<ForwardIter>::value_type>{});
	}
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // !MYSTL_CONSTRUCT_H_



