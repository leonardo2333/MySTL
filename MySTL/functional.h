#pragma once
#ifndef MYSTL_FUNCTIONAL_H_
#define MYSTL_FUNCTIONAL_H_

//定义了函数对象和哈希函数

#include <cstddef>

namespace mystl
{
	//定义一元函数的参数型别和返回值型别
	template<class Arg,class Result>
	struct unarg_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};

	//二元函数的参数型别和返回值型别
	template<class Arg1,class Arg2,class Result>
	struct binary_function
	{
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	//函数对象：加法
	template<class T>
	struct plus:public binary_function<T,T,T>
	{
		T operator()(const T& lhs, const T& rhs) const
		{
			return lhs + rhs;
		}
	};

	//函数对象：减法
	template<class T>
	struct minus:public binary_function<T,T,T>
	{
		T operator()(const T& lhs, const T& rhs) const
		{
			return lhs - rhs;
		}
	};

	//函数对象：乘法
	template<class T>
	struct multiply :public binary_function<T, T, T>
	{
		T operator()(const T& lhs, const T& rhs)const
		{
			return lhs * rhs;
		}
	};

	//函数对象：除法
	template<class T>
	struct divide :public binary_function<T, T, T>
	{
		T operator()(const T& lhs, const T& rhs)const
		{
			return lhs / rhs;
		}
	};

	//函数对象：取模
	template<class T>
	struct Modulus:public binary_function<T,T,T>
	{
		T operator()(const T& lhs, const T& rhs) const
		{
			return lhs % rhs;
		}
	};

	//函数对象：取反
	template<class T>
	struct negate :public unarg_function<T, T>
	{
		T operator()(const T& x)const
		{
			return -x;
		}
	};

	//加法的证同元素
	template<class T>
	T identity_element(plus<T>)
	{
		return T(0);
	}

	//乘法的证同元素
	template<class T>
	T identity_element(multiply<T>)
	{
		return T(1);
	}

	//函数对象：等于
	template<class T>
	struct equal_to:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs == rhs;
		}
	};

	//函数对象：不等于
	template<class T>
	struct not_equal_to:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs != rhs;
		}
	};

	//函数对象：大于
	template<class T>
	struct greater:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs,const T& rhs)const
		{
			return lhs > rhs;
		}
	};

	//函数对象：小于
	template<class T>
	struct less :public binary_function<T, T, bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs < rhs;
		}
	};

	//函数对象：大于等于
	template<class T>
	struct greater_equal :public binary_function<T, T, bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs >= rhs;
		}
	};

	//函数对象：小于等于
	template<class T>
	struct less_equal :public binary_function<T, T, bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs <= rhs;
		}
	};

	//函数对象：逻辑与
	template<class T>
	struct logical_and:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs,const T& rhs)const
		{
			return lhs && rhs;
		}
	};

	//函数对象：逻辑或
	template<class T>
	struct logical_or:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs || rhs;
		}
	};

	//函数对象：逻辑非
	template<class T>
	struct logical_not:public unarg_function<T,bool>
	{
		bool operator()(const T& x) const
		{
			return !x;
		}
	};

	//证同函数
	template<class T>
	struct identity:public unarg_function<T,T>
	{
		const T& operator()(const T& x)const
		{
			return x;
		}
	};

	//选择函数：选择pair的第一个元素
	template<class T>
	struct selectfirst:public unarg_function<T,typename T::first_type>
	{
		typename T::first_type operator()(const T& x) const
		{
			return x.first;
		}
	};

	//选择函数：选择pair的第二个元素
	template<class T>
	struct selectsecond:public unarg_function<T,typename T::second_type>
	{
		typename T::second_type operator()(const T& x)const
		{
			return x.second;
		}
	};
	
	//投射函数：返回第一参数
	template<class Arg1,class Arg2>
	struct projectfirst:public binary_function<Arg1,Arg2,Arg1>
	{
		Arg1 operator()(const Arg1& lhs, const Arg2&)const
		{
			return lhs;
		}
	};

	//投射函数：返回第二参数
	template<class Arg1, class Arg2>
	struct projectfirst :public binary_function<Arg1, Arg2, Arg2>
	{
		Arg2 operator()(const Arg1&, const Arg2& rhs) const
		{
			return rhs;
		}
	};


	//哈希函数对象
	//to be finished

}



#endif // !MYSTL_FUNCTIONAL_H_
