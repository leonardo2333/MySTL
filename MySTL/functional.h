#pragma once
#ifndef MYSTL_FUNCTIONAL_H_
#define MYSTL_FUNCTIONAL_H_

//�����˺�������͹�ϣ����

#include <cstddef>

namespace mystl
{
	//����һԪ�����Ĳ����ͱ�ͷ���ֵ�ͱ�
	template<class Arg,class Result>
	struct unarg_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};

	//��Ԫ�����Ĳ����ͱ�ͷ���ֵ�ͱ�
	template<class Arg1,class Arg2,class Result>
	struct binary_function
	{
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	//�������󣺼ӷ�
	template<class T>
	struct plus:public binary_function<T,T,T>
	{
		T operator()(const T& lhs, const T& rhs) const
		{
			return lhs + rhs;
		}
	};

	//�������󣺼���
	template<class T>
	struct minus:public binary_function<T,T,T>
	{
		T operator()(const T& lhs, const T& rhs) const
		{
			return lhs - rhs;
		}
	};

	//�������󣺳˷�
	template<class T>
	struct multiply :public binary_function<T, T, T>
	{
		T operator()(const T& lhs, const T& rhs)const
		{
			return lhs * rhs;
		}
	};

	//�������󣺳���
	template<class T>
	struct divide :public binary_function<T, T, T>
	{
		T operator()(const T& lhs, const T& rhs)const
		{
			return lhs / rhs;
		}
	};

	//��������ȡģ
	template<class T>
	struct Modulus:public binary_function<T,T,T>
	{
		T operator()(const T& lhs, const T& rhs) const
		{
			return lhs % rhs;
		}
	};

	//��������ȡ��
	template<class T>
	struct negate :public unarg_function<T, T>
	{
		T operator()(const T& x)const
		{
			return -x;
		}
	};

	//�ӷ���֤ͬԪ��
	template<class T>
	T identity_element(plus<T>)
	{
		return T(0);
	}

	//�˷���֤ͬԪ��
	template<class T>
	T identity_element(multiply<T>)
	{
		return T(1);
	}

	//�������󣺵���
	template<class T>
	struct equal_to:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs == rhs;
		}
	};

	//�������󣺲�����
	template<class T>
	struct not_equal_to:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs != rhs;
		}
	};

	//�������󣺴���
	template<class T>
	struct greater:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs,const T& rhs)const
		{
			return lhs > rhs;
		}
	};

	//��������С��
	template<class T>
	struct less :public binary_function<T, T, bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs < rhs;
		}
	};

	//�������󣺴��ڵ���
	template<class T>
	struct greater_equal :public binary_function<T, T, bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs >= rhs;
		}
	};

	//��������С�ڵ���
	template<class T>
	struct less_equal :public binary_function<T, T, bool>
	{
		bool operator()(const T& lhs, const T& rhs)const
		{
			return lhs <= rhs;
		}
	};

	//���������߼���
	template<class T>
	struct logical_and:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs,const T& rhs)const
		{
			return lhs && rhs;
		}
	};

	//���������߼���
	template<class T>
	struct logical_or:public binary_function<T,T,bool>
	{
		bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs || rhs;
		}
	};

	//���������߼���
	template<class T>
	struct logical_not:public unarg_function<T,bool>
	{
		bool operator()(const T& x) const
		{
			return !x;
		}
	};

	//֤ͬ����
	template<class T>
	struct identity:public unarg_function<T,T>
	{
		const T& operator()(const T& x)const
		{
			return x;
		}
	};

	//ѡ������ѡ��pair�ĵ�һ��Ԫ��
	template<class T>
	struct selectfirst:public unarg_function<T,typename T::first_type>
	{
		typename T::first_type operator()(const T& x) const
		{
			return x.first;
		}
	};

	//ѡ������ѡ��pair�ĵڶ���Ԫ��
	template<class T>
	struct selectsecond:public unarg_function<T,typename T::second_type>
	{
		typename T::second_type operator()(const T& x)const
		{
			return x.second;
		}
	};
	
	//Ͷ�亯�������ص�һ����
	template<class Arg1,class Arg2>
	struct projectfirst:public binary_function<Arg1,Arg2,Arg1>
	{
		Arg1 operator()(const Arg1& lhs, const Arg2&)const
		{
			return lhs;
		}
	};

	//Ͷ�亯�������صڶ�����
	template<class Arg1, class Arg2>
	struct projectsecond :public binary_function<Arg1, Arg2, Arg2>
	{
		Arg2 operator()(const Arg1&, const Arg2& rhs) const
		{
			return rhs;
		}
	};


	//��ϣ��������
	
	//���ڴ󲿷����ͣ�hash functionʲôҲ����
	template<class T>
	struct hash
	{
	};

	//��ָ���ƫ�ػ�
	template<class T>
	struct hash<T*>
	{
		size_t operator()(T* p)const noexcept
		{
			return reinterpret_cast<size_t>(p);
		}
	};

	//���������ͣ�����ԭֵ
	//�ô�����MYSTL_HASH_FUNC_INTEGER��,�������������������͵Ĺ�ϣ��������
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

	//���ڸ�����,��λ��ϣ
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
