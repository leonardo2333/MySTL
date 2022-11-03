#pragma once
#ifndef MYSTL_VECTOR_H_
#define MYSTL_VECTOR_H_

//模板类vector

#include<initializer_list>

#include "iterator.h"
#include "utilities.h"
#include "algo.h"
#include "exceptiondef.h"
#include "memory.h"

namespace mystl
{ 
#ifdef max
#pragma message("#undefining macro max")
#undef max
#endif // max
#ifdef min
#pragma message("#undefining macro min")
#undef min
#endif // min


	template<typename T>
	class vector
	{
		static_assert(!std::is_same<bool, T>::value, "vector<bool> is abandoned!");
	public:
		typedef mystl::allocator<T> allocator_type;
		typedef mystl::allocator<T> data_allocator;

		typedef typename allocator_type::value_type value_type;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef mystl::reverse_iterator<iterator> reverse_iterator;
		typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;

		allocator_type get_allocator() 
		{ 
			return data_allocator(); 
		}

	private:
		iterator begin_;//目前使用空间的头部
		iterator end_;//目前使用空间的尾部
		iterator cap_;//目前存储空间的尾部

	public:
		vector()noexcept
		{
			//try_init();
		}

		explicit vector(size_type n)
		{
			fill_init(n, value_type());
		}

		vector(size_type n, const value_type& value)
		{
			fill_init(n, value);
		}

		template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value>::type>
		vector(Iter first, Iter last)
		{
			MYSTL_DEBUG(!(last < first));
			range_init(first, last);
		}

		vector(const vector& rhs)
		{
			range_init(rhs.begin_, rhs.end_);
		}

		vector(vector&& rhs)noexcept:begin_(rhs.begin_),end_(rhs.end_),cap_(rhs.cap_)
		{
			rhs.begin_ = nullptr;
			rhs.end_ = nullptr;
			rhs.cap_ = nullptr;
		}

		vector(std::initializer_list<value_type> ilist)
		{
			range_init(ilist.begin(), ilist.end());
		}

		vector& operator=(const vector& rhs);
		vector& operator=(vector&& rhs);
		vector& operator=(std::initializer_list<value_type> ilist)
		{
			vector tmp(ilist.begin(), ilist.end());
			swap(tmp);
			return this;
		}

		~vector()
		{
			destroy_and_recover(begin_, end_, cap_ - begin_);
			begin_ = end_ = cap_ = nullptr;
		}

		//迭代器相关操作
		iterator begin()noexcept
		{
			return begin_;
		}
		const_iterator begin()noexcept
		{
			return begin_;
		}
		iterator end()noexcept
		{
			return end_;
		}
		const_iterator end()noexcept
		{
			return end_;
		}

		reverse_iterator rbegin()noexcept
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin()const noexcept
		{
			return const_reverse_iterator(end());
		}
		reverse_iterator rend()noexcept
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend()const noexcept
		{
			return const_reverse_iterator(begin());
		}

		const_iterator cbegin() const noexcept
		{
			return begin();
		}
		const_iterator cend()const noexcept
		{
			return end();
		}
		const_reverse_iterator crbegin()const noexcept
		{
			return rbegin();
		}
		const_reverse_iterator crend()const noexcept
		{
			return rend();
		}

		//容量相关
		bool empty()const noexcept
		{
			return begin_ == end_;
		}
		size_type size()const noexcept
		{
			return static_cast<size_type>(end_ - begin_);
		}
		size_type max_size()const noexcept//容器达到这个容量后就不能再扩大
		{
			//return static_cast<size_type>(-1) / sizeof(T);
			//因为signed类型中-1对应所有位均为1
			//因此以上表达式中static_cast<size_type>(-1)代表获得unsigned类型size_type中的最大值
			//可以用以下表达式代替
			return std::numeric_limits<size_type>::max()/sizeof(T);
		}
		size_type capacity()const noexcept//现有容量
		{
			return static_cast<size_type>(cap_ - begin_);
		}
		void reverse(size_type n);
		void shrink_to_fit();

		//访问元素
		reference operator[](size_type n)
		{
			MYSTL_DEBUG(n < size());
			return *(begin_ + n);
		}

		const_reference operator[](size_type n)const
		{
			MYSTL_DEBUG(n < size());
			return *(begin_ + n);
		}

		reference at(size_type n)
		{
			THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
			return (*this)[n];
		}

		const_reference at(size_type n)
		{
			THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
			return (*this)[n];
		}

		reference front()
		{
			MYSTL_DEBUG(!empty());
			return *begin_;
		}
		
		const_reference front()
		{
			MYSTL_DEBUG(!empty());
			return *begin_;
		}

		reference back()
		{
			MYSTL_DEBUG(!empty());
			return *(end_ - 1);
		}

		const_reference back()
		{
			MYSTL_DEBUG(!empty());
			return *(end_ - 1);
		}

		pointer data() noexcept
		{
			return begin_;
		}

		const_pointer data() const noexcept
		{
			return begin_;
		}

		//修改容器相关操作
		//assign
		void assign(size_type n, const value_type& value)
		{
			fill_assign(n, value);
		}

		template<class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value>::type >
		void assign(Iter first, Iter last)
		{
			MYSTL_DEBUG(!(last < first));
			copy_assign(first, last, iterator_category(first));
		}

		void assign(std::initializer_list<value_type> ilist)
		{
			copy_assign(ilist.begin(), ilist.end(), mystl::forward_iterator_tag{});
		}

		//emplace/emplace_back
		template<class... Args>
		iterator emplace(const_iterator pos, Args&& ...args);

		template<class ... Args>
		void emplace_back(Args&& ...args);

		//push_back/pop_back
		void push_back(const vaue_type& value);
		void push_back(value_type&& value)
		{
			emplace_back(mystl::move(value));
		}

		void pop_back();

		//insert
		iterator insert(const_iterator pos, const value_type& value);
		iterator insert(const_iterator pos, value_type&& value)
		{
			return emplace(pos, mystl::move(value));
		}
		iterator insert(const_iterator pos, size_type n, const value_type& value)
		{
			MYSTL_DEBUG(pos >= begin_ && pos <= end_);
			return fill_insert(const_cast<iterator>(pos), n, value);
		}
		template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value>::type>
		void insert(const_iterator pos, Iter first, Iter last)
		{
			MYSTL_DEBUG(pos >= begin_ && pos <= end_ && !(last < first));
			copy_insert(const_cast<iterator>(pos), first, last);
		}

		//erase/clear
		iterator erase(const_iterator pos);
		Iterator erase(const_iterator first, const_iterator last);
		void clear() { erase(begin_, end_); }

		//resize/reverse
		void resize(size_type new_size)
		{
			return resize(new_size, value_type());
		}
		void resize(size_type new_size, const value_type& value);
		
		void reverse() { mystl::reverse(begin_, end_); }

		//swap
		void swap(vector& rhs)noexcept;

	private:
		//initialize/destroy
		void try_init() noexcept;

		void init_space(size_type size, size_type cap);

		void fill_init(size_type n, const value_type& value);

		template<class Iter>
		void range_init(Iter first, Iter last);

		void destroy_and_recover(iterator first, iterator last, size_type n);

		//calculate the growth size
		size_type get_new_cap(size_type add_size);

		//assign
		void fill_assign(size_type n, const value_type& value);

		template<class InputIter>
		void copy_assign(InputIter first, InputIter last, input_iterator_tag);

		template<class ForwardIter>
		void copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag);

		//reallocate
		template<class... Args>
		void reallocate_emplace(iterator pos, Args&& ...args);
		void reallocate_insert(iterator pos, const value_type& value);

		//insert
		iterator fill_insert(iterator pos, size_type n, const value_type& value);
		template<class InputIter>
		void copy_insert(iterator pos, InputIter first, InputIter last);

		//shrink_to_fit
		void reinsert(size_type size);


	};




}










#endif // !MYSTL_VECTOR_H_
