#pragma once
#ifndef MYSTL_MEMORY_H_
#define MYSTL_MEMORY_H_

//负责动态内存管理
//包含基本函数、空间配置器、未初始化的空间管理及auto_ptr模板类

#include<cstddef>
#include<cstdlib>
#include<climits>

#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace mystl
{
	//获取对象地址
	template<class T>
	constexpr T* address_of(T& value)noexcept
	{
		return &value;
	}

	//获取/释放临时缓冲区
	template<class T>
	pair<T*, ptrdiff_t>get_buffer_helper(ptrdiff_t len)
	{
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		//保证临时缓冲区大小小于等于INT_MAX
		while (len > 0)
		{
			T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
			if (tmp)
				return pair<T*, ptrdiff_t>(tmp, len);
			len /= 2;
			//尝试分配len*sizeof(T)大小的内存，
			//若成功，返回指针和缓冲区长度
			//否则len长度减半
		}
		return pair<T*, ptrdiff_t>(nullptr, 0);
	}

	template<class T>
	pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len)
	{
		return mystl::get_buffer_helper<T>(len);
	}

	template<class T>
	void release_temporary_buffer(T* ptr)
	{
		free(ptr);
	}


	//-------------------------------------------------------------------------
	//类模板：temporary_buffer
	//临时缓冲区的申请与释放
	template<class ForwardIterator,class T>
	class temporary_buffer
	{
	private:
		ptrdiff_t ori_len;//申请缓冲区大小
		ptrdiff_t len;//实际缓冲区大小
		T* buffer;//指向缓冲区的指针

	public:
		temporary_buffer(ForwardIterator first, ForwardIterator last)
		{
			try
			{
				len = mystl::distance(first, last);
				allocate_buffer();
				if (len > 0)
					initialize_buffer(*buffer, std::is_trivially_default_constructible<T>());
			}
			catch(...)
			{
				free(buffer);
				buffer = nullptr;
				len = 0;
			}
		};
		temporary_buffer(const temporary_buffer&);//to be defined
		~temporary_buffer() 
		{
			mystl::destroy(buffer, buffer + len);
			free(buffer);
		};

		ptrdiff_t size() const noexcept
		{
			return len;
		}

		ptrdiff_t requested_size() const noexcept
		{
			return ori_len;
		}

		T* begin() noexcept
		{
			return buffer;
		}

		T* end() noexcept
		{
			return buffer + len;
		}

	private:
		void allocate_buffer()
		{
			ori_len = len;
			if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
				len = INT_MAX / sizeof(T);
			while (len > 0)
			{
				buffer = static_cast<T*>(malloc(len * sizeof(T)));
				if (buffer)
					break;
				len /= 2;
			}
		};
		void initialize_buffer(const T&,std::true_type){}
		void initialize_buffer(const T& value, std::false_type)
		{
			mystl::uninitialized_fill_n(buffer, len, value);
		}
		
		void operator=(const temporary_buffer&);//to be defined
	};

	//----------------------------------------------------------------------------
	//模板类：auto_ptr
	//智能指针
	template<class T>
	class auto_ptr
	{
	public:
		typedef T elem_type;

	private:
		elem_type* m_ptr;//实际指针

	public:
		//constructor、destructor
		explicit auto_ptr(T* p=nullptr):m_ptr(p){}
		auto_ptr(auto_ptr& rhs) :m_ptr(rhs.release()){};
		template<class U>
		auto_ptr(auto_ptr<U>& rhs) :m_ptr(rhs.release()){};

		auto_ptr& operator=(auto_ptr& rhs)
		{
			if (this != &rhs)
			{
				delete m_ptr;
				m_ptr = rhs.release();
			}
			return *this;
		}

		template<class U>
		auto_ptr& operator=(auto_ptr<U>& rhs)
		{
			if (this->get() != rhs.get())
			{
				delete m_ptr;
				m_ptr = rhs.release();
			}
			return *this;
		}

		~auto_ptr()
		{
			delete m_ptr;
		}

		T& operator*()const { return *m_ptr; }
		T* operator->()const { return m_ptr; }

		T* get()const { return m_ptr; }

		T* release()
		{
			T* tmp = m_ptr;
			m_ptr = nullptr;
			return tmp;
		}

		void reset(T* p = nullptr)
		{
			if (m_ptr != p)
			{
				delete m_ptr;
				m_ptr = p;
			}
		}
	};
}

#endif // !MYSTL_MEMORY_H_
