#pragma once
#ifndef MYSTL_VECTOR_H_
#define MYSTL_VECTOR_H_

//Ä£°åÀàvector

#include<initializer_list>

#include "utilities.h"
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
		//static_assert(!std::is_same<bool, T>::value, "vector<bool> is abandoned!");
	public:
		typedef mystl::allocator<T> allocator_type;
		typedef mystl::allocator<T> data_allocator;
	};




}










#endif // !MYSTL_VECTOR_H_
