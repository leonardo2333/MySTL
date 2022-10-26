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
	struct projectfirst :public binary_function<Arg1, Arg2, Arg2>
	{
		Arg2 operator()(const Arg1&, const Arg2& rhs) const
		{
			return rhs;
		}
	};


	//��ϣ��������
	//to be finished

}



#endif // !MYSTL_FUNCTIONAL_H_
