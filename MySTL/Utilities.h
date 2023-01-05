#pragma once

#ifndef MYSTL_UTILITIES_H_
#include <cstddef>
#include "type_traits.h"
//定义通用工具，包括move、forward、swap等
namespace mystl
{
	//move
	
	template<class T>
	typename std::remove_reference<T>::type&& move(T&& arg)noexcept
	{ 
		return static_cast<typename std::remove_reference<T>::type&&>(arg);
	}
	//std::remove_reference用于移除变量的引用，获得原类型的变量
	
	//forward
	template<class T>
	T&& forward(typename std::remove_reference<T>::type& arg)noexcept
	{
		return static_cast<T&&>(arg);
	}

	template<class T>
	T&& forward(typename std::remove_reference<T>::type&& arg)noexcept
	{
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
		//在值为左值引用时，抛出编译错误
		return static_cast<T&&>(arg);
	}

	//swap
	template<class T>
	void swap(T& val1, T& val2)
	{
		auto tmp(mystl::move(val1));
		val1 = mystl::move(val2);
		val2 = mystl::move(tmp);
	}

	template<class ForwardIter1,class ForwardIter2>
	ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 end1, ForwardIter2 first2)
	{
		for (; first1 != end1; ++first1, (void)++first2)//void修饰表示变量已被使用，避免警告
			mystl::swap(first1, first2);
		return first2;
	}

	template<class T,size_t N>
	void swap(T(&a)[N], T(&b)[N])
	{
		mystl::swap_range(a, a + N, b);
	}

	//pair
	template<class T1,class T2>
	struct pair
	{
		typedef T1 first_type;
		typedef T2 second_type;

		first_type first;
		second_type second;

		//default constructor
		template<class Ty1 = T1, class Ty2 = T2,
			typename=typename std::enable_if<
			std::is_default_constructible<Ty1>::value&&
			std::is_default_constructible<Ty2>::value>::type>
		constexpr pair() :first(), second() {};
		//typename=typename表示变量已被使用，避免警告
		
		//implicit constructor 隐式构造
		template<class Ty1 = T1, class Ty2 = T2,
			typename=typename std::enable_if<
			std::is_copy_constructible<Ty1>::value&&
			std::is_copy_constructible<Ty2>::value&&
			std::is_convertible<const Ty1&,T1>::value&&
			std::is_convertible<const Ty2&,T2>::value>::type>
		constexpr pair(const Ty1& a, const Ty2& b) :first(a), second(b) {};

		//explicit constructor 显式构造
		template<class Ty1=T1,class Ty2=T2,
			typename std::enable_if<
			std::is_copy_constructible<Ty1>::value&&
			std::is_copy_constructible<Ty2>::value&&
			!(std::is_convertible<const Ty1&, T1>::value&&
			std::is_convertible<const Ty2&, T2>::value)>::type>
		explicit constexpr pair(const Ty1& a, const Ty2& b) :first(a), second(b) {};


		//default constructor 默认构造函数
		pair(const pair& rhs) = default;
		pair(pair&& rhs) = default;

		//implicit constructor for other type
		template<class Other1,class Other2,
			typename=typename std::enable_if<
			std::is_constructible<T1,Other1>::value&&
			std::is_constructible<T2,Other2>::value&&
			std::is_convertible<Other1&&,T1>::value&&
			std::is_convertible<Other2&&,T2>::value>::type>
		constexpr pair(Other1&& a, Other2&& b) :
			first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b)){};

		//explicit constructor for other type
		template<class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<T1,Other1>::value&&
			std::is_constructible<T2,Other2>::value&&
			!(std::is_convertible<Other1&&, T1>::value&&
			std::is_convertible<Other2&&, T2>::value)>::type>
		explicit constexpr pair(Other1&& a, Other2&& b) :
			first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b)) {};

		//implicit constructor for other pair 
		//lvalue ref
		template<class Other1, class Other2,
			typename = typename std::enable_if<
			std::is_constructible<T1,const Other1&>::value&&
			std::is_constructible<T2,const Other2&>::value&&
			std::is_convertible<const Other1&, T1>::value&&
			std::is_convertible<const Other2&, T2>::value>::type>
		constexpr pair(const pair<Other1,Other2>& p) :
			first(p.first), second(p.second) {};

		//explicit constructor for other pair 
		//lvalue ref
		template<class Other1, class Other2,
			typename  std::enable_if<
			std::is_constructible<T1, const Other1&>::value&&
			std::is_constructible<T2, const Other2&>::value &&
			!(std::is_convertible<const Other1&, T1>::value&&
			std::is_convertible<const Other2&, T2>::value)>::type>
		explicit constexpr pair(const pair<Other1, Other2>& p) :
			first(p.first), second(p.second) {};

		//implicit constructor for other pair 
		//rvalue ref
		template<class Other1, class Other2,
			typename = typename std::enable_if<
			std::is_constructible<T1, Other1>::value&&
			std::is_constructible<T2, Other2>::value&&
			std::is_convertible<Other1, T1>::value&&
			std::is_convertible<Other2, T2>::value>::type>
		constexpr pair(pair<Other1, Other2>&& p) :
			first(mystl::forward<Other1>(p.first)), second(mystl::forward<Other2>(p.second)) {};

		//explicit constructor for other pair 
		//rvalue ref
		template<class Other1, class Other2,
			typename  std::enable_if<
			std::is_constructible<T1, Other1>::value&&
			std::is_constructible<T2, Other2>::value &&
			!(std::is_convertible<Other1, T1>::value&&
				std::is_convertible<Other2, T2>::value)>::type>
		explicit constexpr pair(pair<Other1, Other2>&& p) :
			first(mystl::forward<Other1>(p.first)), second(mystl::forward<Other2>(p.second)) {};

		//赋值
		pair& operator=(const pair& rhs)
		{
			first = rhs.first;
			second = rhs.second;
			return *this;
		}
		
		pair& operator=(pair&& rhs)
		{
			first = mystl::move(rhs.first);
			second = mystl::move(rhs.second);
			return *this;
		}

		template<typename Other1,typename Other2>
		pair& operator=(const pair<Other1,Other2>& p)
		{
			first = p.first;
			second = p.second;
			return *this;
		}

		template<typename Other1,typename Other2>
		pair& operator=(pair<Other1, Other2>&& p)
		{
			first = mystl::move(p.first);
			second = mystl::move(p.second);
			return *this;
		}

		~pair() = default;

		void swap(pair& other)
		{
			mystl::swap(first, other.first);
			mystl::swap(second, other.second);
		}

	};

	//重载比较运算符
	template<class T1,class T2>
	bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<class T1, class T2>
	bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first < rhs.first || lhs.first == rhs.first && lhs.second < rhs.second;
	}

	template<class T1, class T2>
	bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first > rhs.first || lhs.first == rhs.first && lhs.second > rhs.second;
	}

	template<class T1, class T2>
	bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs > rhs);
	}

	template<class T1, class T2>
	bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs < rhs);
	}

	template<class T1, class T2>
	bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs == rhs);
	}

	template<class T1,class T2>
	pair<T1, T2> make_pair(T1&& first, T2&& second)
	{
		return pair<T1, T2>(mystl::forward<T1>(first), mystl::forward<T2>(second));
	}
}

#endif // !MYSTL_UTILITIES_H_
