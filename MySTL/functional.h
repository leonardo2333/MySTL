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
	struct projectsecond :public binary_function<Arg1, Arg2, Arg2>
	{
		Arg2 operator()(const Arg1&, const Arg2& rhs) const
		{
			return rhs;
		}
	};


	//哈希函数对象
	
	//对于大部分类型，hash function什么也不做
	template<class T>
	struct hash
	{
	};

	//对指针的偏特化
	template<class T>
	struct hash<T*>
	{
		size_t operator()(T* p)const noexcept
		{
			return reinterpret_cast<size_t>(p);
		}
	};

	//对整型类型，返回原值
	//该处定义MYSTL_HASH_FUNC_INTEGER宏,用于批量定义整型类型的哈希函数对象
#define MYSTL_HASH_FUNC_INTEGER(Type) \
	template<>\
	struct hash<Type>\
	{\
		size_t operator()(Type x)const noexcept\
		{\
			return static_cast<size_t>(x);\
		}\
	};

	MYSTL_HASH_FUNC_INTEGER(bool);
	MYSTL_HASH_FUNC_INTEGER(char);
	MYSTL_HASH_FUNC_INTEGER(unsigned char);
	MYSTL_HASH_FUNC_INTEGER(signed char);
	MYSTL_HASH_FUNC_INTEGER(wchar_t);
	MYSTL_HASH_FUNC_INTEGER(char16_t);
	MYSTL_HASH_FUNC_INTEGER(char32_t);
	MYSTL_HASH_FUNC_INTEGER(short);
	MYSTL_HASH_FUNC_INTEGER(unsigned short);
	MYSTL_HASH_FUNC_INTEGER(int);
	MYSTL_HASH_FUNC_INTEGER(unsigned int);
	MYSTL_HASH_FUNC_INTEGER(long);
	MYSTL_HASH_FUNC_INTEGER(unsigned long);
	MYSTL_HASH_FUNC_INTEGER(long long);
	MYSTL_HASH_FUNC_INTEGER(unsigned long long);

#undef MYSTL_HASH_FUNC_INTEGER

	//对于浮点数,逐位哈希
	inline size_t bitwise_hash(const unsigned char* first, size_t count)
	{
#if(_MSC_VER&&_WIN64)||((__GNUC__||__clang__)&&__SIZEOF_POINTER__==8)
		const size_t fnv_offset = 14695981039346656037ull;
		const size_t fnv_prime = 1099511628211ull;
#else
		const size_t fnv_offset = 2166136261u;
		const size_t fnv_prime = 16777619u;
#endif
		size_t res = fnv_offset;
		for (size_t i = 0; i < count; ++i)
		{
			res ^= (size_t)first[i];
			res *= fnv_prime;
		}
		return res;
	}

	template<>
	struct hash<float>
	{
		size_t operator()(const float& x)
		{
			return x == 0.0f ? 0 : bitwise_hash((const unsigned char*)&x, sizeof(float));
		}
	};

	template<>
	struct hash<double>
	{
		size_t operator()(const double& x)
		{
			return x == 0.0f ? 0 : bitwise_hash((const unsigned char*)&x, sizeof(double));
		}
	};

	template<>
	struct hash<long double>
	{
		size_t operator()(const long double& x)
		{
			return x == 0.0f ? 0 : bitwise_hash((const unsigned char*)&x, sizeof(double));
		}
	};
}



#endif // !MYSTL_FUNCTIONAL_H_
