#pragma once
#ifndef MYSTL_ITERATOR_H_
#define MTSTL_ITERATOR_H_

//���������
#include <cstddef>
#include "type_traits.h"

namespace mystl
{ 
	//5�ֵ�����
	struct input_iterator_tag{};//ֻ��
	struct output_iterator_tag{};//ֻд
	struct forward_iterator_tag:public input_iterator_tag{};//ǰ�����
	struct bidirectional_iterator_tag:public forward_iterator_tag{};//˫�����
	struct random_access_iterator_tag:public bidirectional_iterator_tag{};//�������

	//iteratorģ��
	template<class Category,class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
	struct iterator
	{
		typedef Category iterator_category;
		typedef T value_type;
		typedef Distance distance_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};

	//iterator ����
	template<class T>
	struct has_iterator_cat
	{
	private:
		struct two
		{
			char a, b;
		};
		template<class U>
		static two test(...);

		template<class U>
		static char test(typename U::iterator_category* = 0);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template<class Iterator,bool>
	struct iterator_traits_impl{};

	//iterator_traits_impl��ƫ�ػ�
	template<class Iterator>
	struct iterator_traits_impl<Iterator,true> 
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
		typedef typename Iterator::distance_type distance_type;
	};

	template<class Iterator,bool>
	struct iterator_traits_helper{};

	template<class Iterator>
	struct iterator_traits_helper<Iterator,true>:public iterator_traits_impl<Iterator,
		std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value||
		std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>
	{
	};

	//��ȡ����������
	template<class Iterator>
	struct iterator_traits:public iterator_traits_helper<Iterator,has_iterator_cat<Iterator>::value>{};

	//���ԭ��ָ���ƫ�ػ��汾
	template<class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t distance_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t distance_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

	template<class T,class U,bool =has_iterator_cat<iterator_traits<T>>::value>
	struct has_iterator_cat_of:public m_bool_constant<
		std::is_convertible<typename iterator_traits<T>::iterator_category,U>::value>
	{
	};

	//��ȡĳ�ֵ�����
	template<class T,class U>
	struct has_iterator_cat_of<T,U,false>:public m_false_type{};
	
	template<class Iter>
	struct is_input_iterator:public has_iterator_cat_of<Iter,input_iterator_tag>{};

	template<class Iter>
	struct is_output_iterator:public has_iterator_cat_of<Iter,output_iterator_tag>{};

	template<class Iter>
	struct is_forward_iterator:public has_iterator_cat_of<Iter,forward_iterator_tag>{};

	template<class Iter>
	struct is_bidirectional_iterator:public has_iterator_cat_of<Iter,bidirectional_iterator_tag>{};

	template<class Iter>
	struct is_random_access_iterator:public has_iterator_cat_of<Iter,random_access_iterator_tag>{};

	template<class Iterator>
	struct is_iterator:public m_bool_constant<is_input_iterator<Iterator>::value||
		is_output_iterator<Iterator>::value>
	{
	};

	//��ȡĳ�ֵ�������category
	template<class Iterator>
	typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category Category;
		return Category();
	}

	//��ȡ��������distance_type
	template<class Iterator>
	typename iterator_traits<Iterator>::distance_type* distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::distance_type*>(0);
	}

	//��ȡ��������value_type
	template<class Iterator>
	typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//���º������ڼ��������֮��ľ���
	
	//input_iterator_tag�汾
	template<class InputIterator>
	typename iterator_traits<InputIterator>::distance_type 
		distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::distance_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	//random_access_iterator_tag�汾
	template<class RandomIterator>
	typename iterator_traits<RandomIterator>::distance_type
		distance_dispatch(RandomIterator first, RandomIterator last, random_access_iterator_tag)
	{
		return last - first;
	}

	template<class InputIterator>
	typename iterator_traits<InputIterator>::distance_type
		distance(InputIterator first, InputIterator last)
	{
		distance_dispatch(first, last, iterator_category(first));
	}

	//���º��������õ�����ǰ��n

	//input_iterator_tag�汾
	template<class InputIterator,class Distance>
	void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
	{
		while (n--)
			++i;
	}
	
	//bidirectional_iterator_tag�汾
	template<class BidirectionalIterator,class Distance>
	void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
			while (n--)
				++i;
		else
			while (n++)
				--i;
	}

	//random_access_iterator_tag�汾
	template<class RandomIterator, class Distance>
	void advance_dispatch(RandomIterator& i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	template<class InputIterator,class Distance>
	void advance(InputIterator& i, Distance n)
	{
		advance_dispatch(i, n, iterator_category(i));
	}

	//ģ���ࣺreverse_iterator
	//���������
	template<class Iterator>
	class reverse_iterator
	{
	private:
		Iterator current;

	public:
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::distance_type distance_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef typename iterator_traits<Iterator>::reference reference;

		typedef Iterator iterator_type;
		typedef reverse_iterator<Iterator> self;

		//���캯��
		reverse_iterator() {};
		explicit reverse_iterator(iterator_type i) :current(i) {};
		reverse_iterator(const self& rhs) :current(rhs.current) {};

		iterator_type base()const
		{
			return current;
		}


		//���������
		reference operator*()const
		{
			auto tmp = current;
			return *--tmp;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		//����++i
		self& operator++()
		{
			--current;
			return *this;
		}

		//����i++������int����������i++�ı�ǣ�����������
		self operator++(int)
		{
			self tmp = *this;
			--current;
			return tmp;
		}

		self& operator--()
		{
			++current;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			++current;
			return tmp;
		}

		self& operator+=(distance_type n)
		{
			current -= n;
			return *this;
		}

		self operator+(distance_type n) const
		{
			self tmp(current-n);
		}

		self& operator-=(distance_type n)
		{
			current += n;
			return *this;
		}

		self operator-(distance_type n) const
		{
			return self(current + n);
		}

		reference operator[](distance_type n) const
		{
			return *(*this + n);
		}
	};

	//����operator-
	template<class Iterator>
	typename reverse_iterator<Iterator>::distance_type
		operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() - lhs.base();
	}

	template<class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs==rhs);
	}

	template<class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs.base() < rhs.base());
	}

	template<class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return lhs.base() > rhs.base();
	}

	template<class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs.base() > rhs.base());
	}


}

#endif // !MYSTL_ITERATOR_H_
