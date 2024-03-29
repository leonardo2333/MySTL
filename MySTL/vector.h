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

		template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0>
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
		vector& operator=(vector&& rhs)noexcept;
		vector& operator=(std::initializer_list<value_type> ilist)
		{
			vector<value_type> tmp(ilist);
			swap(tmp);
			return *this;
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
		const_iterator begin()const noexcept
		{
			return begin_;
		}
		iterator end()noexcept
		{
			return end_;
		}
		const_iterator end()const noexcept
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
		void reserve(size_type n);
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

		const_reference at(size_type n) const 
		{
			THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
			return (*this)[n];
		}

		reference front()
		{
			MYSTL_DEBUG(!empty());
			return *begin_;
		}
		
		const_reference front()const
		{
			MYSTL_DEBUG(!empty());
			return *begin_;
		}

		reference back()
		{
			MYSTL_DEBUG(!empty());
			return *(end_ - 1);
		}

		const_reference back()const
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

		template<class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0 >
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
		void push_back(const value_type& value);
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
		template<class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type=0>
		void insert(const_iterator pos, Iter first, Iter last)
		{
			MYSTL_DEBUG(pos >= begin_ && pos <= end_ && !(last < first));
			copy_insert(const_cast<iterator>(pos), first, last);
		}

		//erase/clear
		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);
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

	template<class T>
	vector<T>& vector<T>::operator=(const vector& rhs)
	{
		if (this != &rhs)
		{
			const auto len = rhs.size();
			if (len > capacity())
			{
				vector tmp(rhs.begin(), rhs.end());
				swap(tmp);
			}
			else if (size() >= len)
			{
				auto i = mystl::copy(rhs.begin(), rhs.end(), begin());
				data_allocator::destroy(i, end());
				end_ = begin() + len;
			}
			else
			{
				mystl::copy(rhs.begin(), rhs.begin() + size(), begin());
				mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end());
				cap_ = end_ = begin_ + len;
			}
		}
		return *this;
	}

	template<class T>
	vector<T>& vector<T>::operator=(vector&& rhs) noexcept
	{
		destroy_and_recover(begin_, end_, cap_ - begin_);
		begin_ = rhs.begin_;
		end_ = rhs.end_;
		cap_ = rhs.cap_;
		rhs.begin_ = nullptr;
		rhs.end_ = nullptr;
		rhs.cap_ = nullptr;
		return *this;
	}

	template<class T>
	void vector<T>::reserve(size_type n)//是reserve，不是reverse，预分配n大小的内存空间
	{
		if (capacity() < n)
		{
			THROW_LENGTH_ERROR_IF(n > max_size(), "n can not larger than max_size() in vector<T>::reverse(n)");
			const auto old_size = size();
			auto tmp = data_allocator::allocate(n);
			mystl::uninitialized_move(begin_, end_, tmp);
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = tmp;
			end_ = tmp + old_size;
			cap_ = begin_ + n;
		}
	}

	//放弃多余容量
	template<class T>
	void vector<T>::shrink_to_fit()
	{
		if (end_ < cap_)
		{
			reinsert(size());
		}
	}

	//在pos位置原地构造元素，避免额外的复制或移动开销
	template<class T>
	template<class ...Args>
	typename vector<T>::iterator vector<T>::emplace(const_iterator pos, Args&& ...args)
	{
		MYSTL_DEBUG(pos >= begin() && pos <= end());
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = xpos - begin();
		if (end_ != cap_ && xpos == end_)
		{
			data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
			++end_;
			//为什么需要address_of()，而不是直接用end_
		}
		else if (end_ != cap_)
		{
			auto new_end = end_;
			data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
			++new_end;
			mystl::copy_backward(xpos, end_ - 1, end_);
			*xpos = value_type(mystl::forward<Args>(args)...);
			end_ = new_end;
		}
		else
		{
			reallocate_emplace(xpos, mystl::forward<Args>(args)...);
		}
		return begin() + n;
	}

	template<class T>
	template<class ...Args>
	void vector<T>::emplace_back(Args&& ...args)
	{
		if (end_ < cap_)
		{
			data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
			++end_;
		}
		else
		{
			reallocate_emplace(end_, mystl::forward<Args>(args)...);
		}
	}



	template<class T>
	void vector<T>::push_back(const value_type& value)
	{
		if (end_ != cap_)
		{
			data_allocator::construct(mystl::address_of(*end_), value);
			++end_;
		}
		else
		{
			reallocate_insert(end_, value);
		}
	}

	template<class T>
	void vector<T>::pop_back()
	{
		MYSTL_DEBUG(!empty());
		data_allocator::destroy(end_ - 1);
		--end_;
	}

	template<class T>
	typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value)
	{
		MYSTL_DEBUG(pos >= begin() && pos <= end());
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = pos - begin_;
		if (end_ != cap_ && xpos == end_)
		{
			data_allocator::construct(mystl::address_of(*end_), value);
			++end_;
		}
		else if (end_ != cap_)
		{
			auto new_end = end_;
			data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
			++new_end;
			auto value_copy = value;//避免元素因为复制操作而改变
			mystl::copy_backward(xpos, end_ - 1, end_);
			*xpos = mystl::move(value_copy);
			end_ = new_end;
		}
		else
		{
			reallocate_insert(xpos, value);
		}
		return begin + n;
	}

	template<class T>
	typename vector<T>::iterator vector<T>::erase(const_iterator pos)
	{
		MYSTL_DEBUG(pos >= begin() && pos < end());
		iterator xpos = const_cast<iterator>(pos);
		mystl::move(xpos + 1, end_, xpos);
		data_allocator::destroy(end_ - 1);
		--end_;
		return xpos;
	}

	template<class T>
	typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
	{
		MYSTL_DEBUG(first >= begin() && last <= end() && first < last);
		const auto n = first - begin();
		iterator xpos = begin_+n;
		data_allocator::destroy(mystl::move(xpos + (last - first), end_, xpos), end_);
		end_ = end_ - (last - first);
		return begin_ + n;
	}

	template<class T>
	void vector<T>::resize(size_type new_size, const value_type& value)
	{
		if (new_size < size())
		{
			erase(begin() + new_size, end());
		}
		else
		{
			insert(end(), new_size - size(), value);
		}
	}

	template<class T>
	void vector<T>::swap(vector<T>& rhs)noexcept
	{
		if (*this != rhs)
		{
			mystl::swap(begin_, rhs.begin_);
			mystl::swap(end_, rhs.end_);
			mystl::swap(cap_, rhs.cap_);
		}
	}

	//helper function

	//try_init
	template<class T>
	void vector<T>::try_init()noexcept
	{
		try
		{
			begin_ = data_allocator::allocate(16);
			end_ = begin_;
			cap_ = begin_ + 16;
		}
		catch (...)
		{
			begin_ = end_ = cap_ = nullptr;
		}
	}

	template<typename T>
	inline void vector<T>::init_space(size_type size, size_type cap)
	{
		try
		{
			begin_ = data_allocator::allocate(cap);
			end_ = begin_ + size;
			cap_ = begin_ + cap;
		}
		catch (...)
		{
			begin_ = end_ = cap_ = nullptr;
			throw;
		}
	}

	template<typename T>
	inline void vector<T>::fill_init(size_type n, const value_type& value)
	{
		const size_type init_size = mystl::max(static_cast<size_type>(16), n);
		init_space(n, init_size);
		mystl::uninitialized_fill_n(begin_, n, value);
	}

	template<typename T>
	template<class Iter>
	inline void vector<T>::range_init(Iter first, Iter last)
	{
		const size_type init_size = mystl::max(static_cast<size_type>(last - first), static_cast<size_type>(16));
		init_space(static_cast<size_type>(last - first), init_size);
		mystl::uninitialized_copy(first, last, begin_);
	}

	template<typename T>
	inline void vector<T>::destroy_and_recover(iterator first, iterator last, size_type n)
	{
		data_allocator::destroy(first, last);
		data_allocator::deallocate(first, n);
	}
	
	template<typename T>
	inline typename vector<T>::size_type vector<T>::get_new_cap(size_type add_size)
	{
		const auto old_size = capacity();
		THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size, "vector<T>'s size too big");
		if (old_size > max_size() - old_size / 2)
		{
			return old_size + add_size > max_size() - 16 ? old_size + add_size : old_size + add_size + 16;
		}
		return old_size == 0 ? mystl::max(add_size, static_cast<size_type>(16)) :
			mystl::max(old_size + (old_size >> 1), old_size + add_size);
	}

	template<typename T>
	inline void vector<T>::fill_assign(size_type n, const value_type& value)
	{
		if (n > capacity())
		{
			vector<T> tmp(n, value);
			swap(tmp);
		}
		else if (n > size())
		{
			mystl::fill(begin(), end(), value);
			end_ = mystl::uninitialized_fill_n(end_, n - size(), value);
		}
		else
		{
			erase(mystl::fill_n(begin_, n, value), end_);
		}
	}

	template<typename T>
	template<class InputIter>
	inline void vector<T>::copy_assign(InputIter first, InputIter last, input_iterator_tag)
	{
		auto cur = begin_;
		for (; first != last && cur != end_; ++first, ++cur)
		{
			*cur = *first;
		}
		if (first == last)
		{
			erase(cur, end_);
		}
		else
		{
			insert(end_, first, last);
		}
	}

	template<typename T>
	template<class ForwardIter>
	inline void vector<T>::copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag)
	{
		const size_type len = mystl::distance(first, last);
		if (len > capacity())
		{
			vector<T> tmp(first, last);
			swap(tmp);
		}
		else if (size() >= len)
		{
			auto new_end = mystl::copy(first, last, begin_);
			data_allocator::destroy(new_end, end_);
			end_ = new_end;
		}
		else
		{
			auto mid = first;
			mystl::advance(mid, size());
			mystl::copy(first, mid, begin_);
			end_ = mystl::uninitialized_copy(mid, last, end_);
		}
	}
	
	template<typename T>
	template<class ...Args>
	inline void vector<T>::reallocate_emplace(iterator pos, Args && ...args)
	{
		const auto new_size = get_new_cap(1);
		auto new_begin = data_allocator::allocate(new_size);
		auto new_end = new_begin;
		try
		{
			new_end = mystl::uninitialized_move(begin_, pos, new_begin);
			data_allocator::construct(mystl::address_of(*new_end), mystl::forward<Args>(args)...);
			++new_end;
			new_end = mystl::uninitialized_move(pos, end_, new_end);
		}
		catch (...)
		{
			data_allocator::deallocate(new_begin, new_size);
			throw;
		}
		destroy_and_recover(begin_, end_, cap_ - begin_);
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}

	template<typename T>
	inline void vector<T>::reallocate_insert(iterator pos, const value_type& value)
	{
		const auto new_size = get_new_cap(1);
		auto new_begin = data_allocator::allocate(new_size);
		auto new_end = new_begin;
		const value_type value_copy = value;
		try
		{
			new_end = mystl::uninitialized_copy(begin_, pos, new_begin);
			mystl::construct(mystl::address_of(* new_end), value_copy);
			++new_end;
			new_end = mystl::uninitialized_copy(pos, end_, new_end);
		}
		catch (...)
		{
			data_allocator::deallocate(new_begin, new_size);
			throw;
		}
		destroy_and_recover(begin_, end_, cap_ - begin_);
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;
	}

	template<typename T>
	inline typename vector<T>::iterator vector<T>::fill_insert(iterator pos, size_type n, const value_type& value)
	{
		if (!n)
			return pos;
		const size_type xpos = pos - begin_;
		const value_type value_copy = value;
		if (static_cast<size_type>(capacity() - size()) >= n)
		{
			const size_type after_elems = end_ - pos;
			auto old_end = end_;
			if (after_elems > n)
			{
				mystl::uninitialized_copy(end_ - n, end_, end_);
				end_ += n;
				mystl::move_backward(pos, old_end - n, old_end);
				mystl::uninitialized_fill_n(pos, n, value_copy);
			}
			else
			{
				end_ = mystl::uninitialized_fill_n(end_, n - after_elems, value_copy);
				end_ = mystl::uninitialized_move(pos, old_end, end_);
				mystl::uninitialized_fill_n(pos, after_elems, value_copy);
			}
		}
		else
		{
			const size_type new_size = get_new_cap(n);
			auto new_begin = data_allocator::allocate(new_size);
			auto new_end = new_begin;
			try
			{
				new_end = mystl::uninitialized_move(begin_, pos, new_end);
				new_end = mystl::uninitialized_fill_n(new_end, n, value);
				new_end = mystl::uninitialized_move(pos, end_, new_end);
			}
			catch (...)
			{
				destroy_and_recover(new_begin,new_end,new_size);
				throw;
			}
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + new_size;
		}
	}

	template<typename T>
	template<class InputIter>
	inline void vector<T>::copy_insert(iterator pos, InputIter first, InputIter last)
	{
		if (first == last)
			return;
		const auto n = mystl::distance(first, last);
		if ((cap_ - end_) >= n)
		{
			const auto after_elems = end_ - pos;
			auto old_end = end_;
			if (after_elems > n)
			{
				end_ = mystl::uninitialized_copy(end_ - n, end_, end_);
				mystl::move_backward(pos, old_end - n, old_end);
				mystl::uninitialized_copy(first, last, pos);
			}
			else
			{
				auto mid = first;
				mystl::advance(mid, after_elems);
				end_ = mystl::uninitialized_copy(mid, last, end_);
				end_ = mystl::uninitialized_move(pos, old_end, end_);
				mystl::uninitialized_copy(first, mid, pos);
			}
		}
		else
		{
			const auto new_size = get_new_cap(n);
			auto new_begin = data_allocator::allocate(new_size);
			auto new_end = new_begin;
			try
			{
				new_end = mystl::uninitialized_move(begin_, pos, new_begin);
				new_end = mystl::uninitialized_copy(first, last, new_end);
				new_end = mystl::uninitialized_move(pos, end_, new_end);
			}
			catch (...)
			{
				destroy_and_recover(new_begin, new_end, new_size);
				throw;
			}
			data_allocator::deallocate(begin_, cap_ - begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = new_begin + new_size;
		}
	}

	template<typename T>
	inline void vector<T>::reinsert(size_type size)
	{
		auto new_begin = data_allocator::allocate(size);
		try
		{
			mystl::uninitialized_move(begin_, end_, new_begin);
		}
		catch (...)
		{
			data_allocator::deallocate(new_begin, size);
			throw;
		}
		data_allocator::deallocate(begin_, cap_ - begin_);
		begin_ = new_begin;
		end_ = begin_ + size;
		cap_ = begin_ + size;
	}

	//重载比较操作符
	template<class T>
	bool operator==(const vector<T>& lhs, const vector<T>& rhs)
	{
		return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<class T>
	bool operator<(const vector<T>& lhs, const vector<T>& rhs)
	{
		return mystl::lexicagraphical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class T>
	bool operator!=(const vector<T>& lhs, const vector<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template<class T>
	bool operator>(const vector<T>& lhs, const vector<T>& rhs)
	{
		return rhs < lhs;
	}

	template<class T>
	bool operator<=(const vector<T>& lhs, const vector<T>& rhs)
	{
		return !(rhs < lhs);
	}

	template<class T>
	bool operator>=(const vector<T>& lhs, const vector<T>& rhs)
	{
		return !(lhs < rhs);
	}

	template<class T>
	void swap(vector<T>& lhs, vector<T>& rhs)
	{
		lhs.swap(rhs);
	}

}

#endif // !MYSTL_VECTOR_H_
