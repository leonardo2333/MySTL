#pragma once
#ifndef MYSTL_ALGOBASE_H_
#define MYSTL_ALGOBASE_H_

//定义mystl的基本算法
#include <cstring>
#include "utilities.h"
#include "iterator.h"
 
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

	//max
	template<class T>
	const T& max(const T& lhs, const T& rhs)
	{
		return lhs > rhs?lhs:rhs;
	}

	template<class T,class Compare>
	const T& max(const T& lhs, const T& rhs, Compare comp)
	{
		return comp(lhs, rhs) ? lhs : rhs;
	}

	//min
	template<class T>
	const T& min(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	template<class T,class Compare>
	const T& min(const T& lhs, const T& rhs, Compare comp)
	{
		return comp(lhs, rhs) ? lhs : rhs;
	}

	//iter_swap
	//将两个迭代器所指对象对调
	template<class Iterator1,class Iterator2>
	void iter_swap(Iterator1 lhs, Iterator2 rhs)
	{
		mystl::swap(lhs, rhs);
	}

	//copy
	//input_iterator版本
	template<class InputIter,class OutputIter>
	OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result, mystl::input_iterator_tag)
	{
		for (; first != last; ++first, ++result)
		{
			*result = *first;
		}
		return result;
	}

	//random_access_iterator版本
	template<class RandomIter,class OutputIter>
	OutputIter unchecked_copy_cat
		(RandomIter first, RandomIter last, OutputIter result, mystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n,++result,++first)
		{
			*result = *first;
		}
		return result;
	}

	template<class InputIter,class OutputIter>
	OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy_cat(first, last, result, mystl::iterator_category(first));
	}

	//trivially_copy_assignable类型的特化版本
	template<class T,class U>
	typename std::enable_if<std::is_same<typename std::remove_const<T>::type,U>::value&&
		std::is_trivially_copy_assignable<U>::value,U*>::type 
		unchecked_copy(T* first, T* last, U* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n)
		{
			std::memmove(result, first, n * sizeof(U));
		}
		return result + n;
	}

	template<class InputIter,class OutputIter>
	OutputIter copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy(first, last, result);
	}

	//copy_backward
	//bidirectional_iterator版本
	template<class BidirectionalIterator1,class BidirectionalIterator2>
	BidirectionalIterator2 unchecked_copy_backward_cat(BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result, mystl::bidirectional_iterator_tag)
	{
		while (first != last)
		{
			*--result = *--last;
		}
		return result;
	}

	//random_access_iterator版本
	template<class RandomIter1,class RandomIter2>
	RandomIter2 unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
		RandomIter2 resullt, random_access_iterator_tag)
	{
		for(auto n=last-first;n>0;--n)
		{
			*--result = *--last;
		}
		return result;
	}

	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 unchecked_copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		return unchecked_copy_backward_cat(first, last, result, mystl::iterator_category(first));
	}

	//trivially_copy_assignable类型的特化版本
	template<class BidirectionalIter1,class BidirectionalIter2>
	typename std::enable_if<
		std::is_same<
		typename std::remove_const<BidirectionalIter1>::type,BidirectionalIter2>::value&&
		std::is_trivially_copy_assignable<BidirectionalIter1>::value,BidirectionalIter2*>::type
		unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		const auto n = static_cast<BidirectionalIter1>(last - first);
		if (n)
		{
			result -= n;
			memmove(result, first, n * sizeof(BidirectionalIter2));
		}
		return result;
	}

	template<class BidirectionalIter1,class BidirectionalIter2>
	BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		return unchecked_copy_backward(first, last, result);
	}

	//copy_if
	template<class InputIter,class OutputIter,class UnaryPredict>
	OutputIter copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredict unary_pred)
	{
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
			{
				*result = *first;
				++result;
			}
		}
		return result;
	}

	//copy_n
	template<class InputIter,class Size,class OutputIter>
	mystl::pair<InputIter, OutputIter> unchecked_copy_n(InputIter first, Size n, OutputIter result,input_iterator_tag)
	{
		while (n--)
		{
			*result = *first;
			++result;
			++result;
		}
		return result;
	}

	template<class RandomIter,class Size,class OutputIter>
	mystl::pair<RandomIter, OutputIter> unchecked_copy_n(RandomIter first, Size n, OutputIter result,random_access_iterator_tag)
	{
		auto last = first + n;
		return mystl::pair<RandomIter, OutputIter>(last, mystl::copy(first, last, result));
	}

	template<class InputIter,class Size,class OutputIter>
	mystl::pair<InputIter, OutputIter> copy_n(InputIter first, Size n, OutputIter result)
	{
		return unchecked_copy_n(first, n, result,mystl::iterator_category(first));
	}

	//move
	//input_iterator版本
	template<class InputIter, class OutputIter>
	OutputIter unchecked_move_cat(InputIter first, InputIter last, OutputIter result, mystl::input_iterator_tag)
	{
		for (; first != last; ++first, ++result)
		{
			*result = mystl::move( * first);
		}
		return result;
	}

	//random_access_iterator版本
	template<class RandomIter, class OutputIter>
	OutputIter unchecked_move_cat
	(RandomIter first, RandomIter last, OutputIter result, mystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n, ++result, ++first)
		{
			*result = mystl::move( * first);
		}
		return result;
	}

	template<class InputIter, class OutputIter>
	OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move_cat(first, last, result, mystl::iterator_category(first));
	}

	//trivially_move_assignable类型的特化版本
	template<class T, class U>
	typename std::enable_if<std::is_same<typename std::remove_const<T>::type, U>::value&&
		std::is_trivially_move_assignable<U>::value, U*>::type
		unchecked_move(T* first, T* last, U* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n)
		{
			std::memmove(result, first, n * sizeof(U));
		}
		return result + n;
	}

	template<class InputIter, class OutputIter>
	OutputIter move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move(first, last, result);
	}

	//move_backward
	//bidirectional_iterator版本
	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 unchecked_move_backward_cat(BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result, mystl::bidirectional_iterator_tag)
	{
		while (first != last)
		{
			*--result = mystl::move( * --last);
		}
		return result;
	}

	//random_access_iterator版本
	template<class RandomIter1, class RandomIter2>
	RandomIter2 unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
		RandomIter2 resullt, random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n)
		{
			*--result = mystl::move( * --last);
		}
		return result;
	}

	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 unchecked_move_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
		BidirectionalIterator2 result)
	{
		return unchecked_move_backward_cat(first, last, result, mystl::iterator_category(first));
	}

	//trivially_move_assignable类型的特化版本
	template<class BidirectionalIter1, class BidirectionalIter2>
	typename std::enable_if<
		std::is_same<
		typename std::remove_const<BidirectionalIter1>::type, BidirectionalIter2>::value&&
		std::is_trivially_move_assignable<BidirectionalIter1>::value, BidirectionalIter2*>::type
		unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		const auto n = static_cast<BidirectionalIter1>(last - first);
		if (n)
		{
			result -= n;
			memmove(result, first, n * sizeof(BidirectionalIter2));
		}
		return result;
	}

	template<class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2 move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		return unchecked_move_backward(first, last, result);
	}

	//equal
	template<class InputIter1,class InputIter2>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		while (first1 != last1)
		{
			if (*first1 != *first2)
				return false;
			++first1;
			++first2;
		}
		return true;
	}

	template<class InputIter1, class InputIter2,class Compare>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2,Compare comp)
	{
		while (first1 != last1)
		{
			if (!comp(*first1,*first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	} 

	//fill_n
	template<class OutputIter,class Size,class T>
	OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
	{
		while (n--)
		{
			*first = value;
			++first;
		}
		return first;
	}

	//one-byte类型的特化版本
	template<class T,class Size,class U>
	typename std::enable_if<std::is_integral<T>::value&&sizeof(T) == 1&&!std::is_same<T,bool>::value&&
		std::is_integral<U>::value && sizeof(U) == 1, T*>::type unchecked_fill_n(T* first, Size n, U value)
	{
		if (n)
			std::memset(first, (unsigned char)value, (size_t)n);
		return first + n;
	}

	template<class OutputIter,class Size,class T>
	OutputIter fill_n(OutputIter first, Size n, const T& value)
	{
		return unchecked_fill_n(first, n, value);
	}

	//fill
	//forward_iterator版本
	template<class ForwardIter,class T>
	void fill_cat(ForwardIter first, ForwardIter last, const T& value,mystl::forward_iterator_tag)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
	}

	//random_access_iterator版本
	template<class RandomIter,class T>
	void fill_cat(RandomIter first, RandomIter last, const T& value, mystl::random_access_iterator_tag)
	{
		fill_n(first, last - first, value);
	}

	template<class ForwardIter,class T>
	void fill(ForwardIter first, ForwardIter last, const T& value)
	{
		return fill_cat(first, last, value, mystl::iterator_category(first));
	}

	//lexicographical_compare
	//以字典序对两个序列进行比较，有如下情况：
	//（1）第一序列的元素较小，返回true；否则，返回false；
	//（2）到达last1且未到达last2，返回true；
	//（3）到达last2，返回false；
	template<class InputIter1,class InputIter2>
	bool lexicagraphical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			else if (*first1 > *first2)
				return false;
		}
		return first1 == last1 && first2 != last2;
	}

	template<class InputIter1, class InputIter2,class Compare>
	bool lexicagraphical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2,Compare comp)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (comp(*first1,*first2)
				return true;
			else if (comp(*first2,*first1))
				return false;
		}
		return first1 == last1 && first2 != last2;
	}

	//const unsigned char*全特化版本
	bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1,
		const unsigned char* first2, const unsigned char* last2)
	{
		const auto len1 = last1 - first1;
		const auto len2 = last2 - first2;
		const auto res = std::memcmp(first1, first2, mystl::min(len1, len2));
		return res != 0 ? res < 0 : len1 < len2;
	}

	//mismatch
	//比较两个序列，返回指向首对不匹配元素的迭代器
	template<class InputIter1,class InputIter2>
	mystl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *first2)
				break;
		}
		return mystl::make_pair<InputIter1, InputIter2>(first1, first2);
	}

	template<class InputIter1, class InputIter2,class Compare>
	mystl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2,Compare comp)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!comp(*first1,*first2))
				break;
		}
		return mystl::make_pair<InputIter1, InputIter2>(first1, first2);
	}
}

#endif // !MYSTL_ALGOBASE_H_
