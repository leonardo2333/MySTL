#pragma once
#ifndef MYSTL_ALGO_H_
#define MYSTL_ALGO_H_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4244)
#endif // _MSC_VER

#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "memory.h"

#include "functional.h"

namespace mystl
{
	//all of
	//检查[first,last)区间内是否全部元素均满足unary_pred为真
	template<class InputIter,class UnaryPredicate>
	bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (!unary_pred(*first))
				return false;
		}
		return true;
	}

	//any of
	//检查[first,last)区间内是否存在元素满足unary_pred为真
	template<class InputIter, class UnaryPredicate>
	bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
				return true;
		}
		return false;
	}

	//none_of
	//检查[first,last)区间内是否所有元素均不满足unary_pred为真
	template<class InputIter, class UnaryPredicate>
	bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
				return false;
		}
		return true;
	}

	//count
	//检查[first,last)内与value相等的元素个数
	template<class InputIter,class T>
	size_t count(InputIter first, InputIter last, const T& value)
	{
		size_t res = 0;
		for (; first != last; ++first)
		{
			if (*first == value)
				++res;
		}
		return res;
	}

	//count_if
	//检查[first,last)内满足unary_pred的元素个数
	template<class InputIter, class UnaryPredicate>
	size_t count(InputIter first, InputIter last, UnaryPredicate unary_pred)
	{
		size_t res = 0;
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
				++res;
		}
		return res;
	}

	//find
	//检查[first,last)内与value相等的元素,返回其迭代器
	template<class InputIter,class T>
	InputIter find(InputIter first, InputIter last, const T& value)
	{
		for (; first != last; ++first)
		{
			if (*first == value)
				break;
		}
		return first;
	}

	template<class InputIter, class T,class Compare>
	InputIter find(InputIter first, InputIter last, const T& value,Compare comp)
	{
		for (; first != last; ++first)
		{
			if (Comp(*first,value))
				break;
		}
		return first;
	}

	//find_if
	//检查[first,last)内满足unary_pred的元素,返回其迭代器
	template<class InputIter, class UnaryPredicate>
	InputIter find_if(InputIter first, InputIter last, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
				break;
		}
		return first;
	}

	//find_if_not
	//检查[first,last)内不满足unary_pred的元素,返回其迭代器
	template<class InputIter, class UnaryPredicate>
	InputIter find_if_not(InputIter first, InputIter last, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (!unary_pred(*first))
				break;
		}
		return first;
	}

	//search
	//zai[first1,last1)中查找[first2,last2)的首次出现点
	template<class ForwardIter1, class ForwardIter2>
	ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1,
		ForwardIter2 first2, ForwardIter2 last2)
	{
		auto d1 = mystl::distance(first1,last1);
		auto d2 = mystl::distance(first2,last2);
		if (d2 > d1)
			return last1;
		auto cur1 = first1;
		auto cur2 = first2;
		while (cur2 != last2)
		{
			if (*cur1 == *cur2)
			{
				++cur1;
				++cur2;
			}
			else
			{
				if (d1 == d2)
				{
					return last1;
				}
				else
				{
					cur1 = ++first1;
					cur2 = first2;
					--d1;
				}
			}
		}
		return first1;
	}

	template<class ForwardIter1,class ForwardIter2,class Compare>
	ForwardIter1 search(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2, Compare comp)
	{
		auto d1 = mystl::distance(first1, last1);
		auto d2 = mystl::distance(first2, last2);
		if (d1 < d2)
			return last1;
		ForwardIter1 cur1 = first1;
		ForwardIter2 cur2 = first2;
		while (cur2 != last2)
		{
			if (comp(*cur1,*cur2))
			{
				++cur1;
				++cur2;
			}
			else
			{
				if (d1 == d2)
				{
					return last1;
				}
				else
				{
					cur1=++first1;
					cur2 = first2;
					--d1;
				}
			}
		}
		return cur1;
	}

	//search_n
	//查找连续n个value形成的子序列
	template<class ForwardIter,class Size,class T>
	ForwardIter search_n(ForwardIter first, ForwardIter last, Size n, const T& value)
	{
		if (n <= 0)
			return first;
		first = mystl::find(first, last, value);
		while (first != last)
		{
			auto m = n - 1;
			auto cur = first;
			++cur;
			while (cur != last && m != 0 && *cur = value)
			{
				++cur;
				--m;
			}
			if (!m)
				return first;
			else
				first = mystl::find(cur, last, value);
		}
		return last;
	}

	template<class ForwardIter, class Size, class T,class Compare>
	ForwardIter search_n(ForwardIter first, ForwardIter last, Size n, const T& value,Compare comp)
	{
		if (n <= 0)
			return first;
		first = mystl::find(first, last, value, comp);
		while (first != last)
		{
			auto m = n - 1;
			auto cur = first;
			++cur;
			while (cur != last && m != 0 && comp(*cur,value)
			{
				++cur;
				--m;
			}
			if (!m)
				return first;
			else
				first = mystl::find(cur, last, value, comp);
		}
		return last;
	}

	//find_end
	//在[first1,last1)中查找[first2,last2)最后一次出现的位置，若不存在返回last1
	//forward_iterator_tag版本
	template<class ForwardIter1,class ForwardIter2>
	ForwardIter1 find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2,
		forward_iterator_tag,forward_iterator_tag)
	{
		if (first2 == last2)
			return last1;
		auto res = last1;
		while (true)
		{
			auto tmp = mystl::search(first1, last1, first2, last2);
			if (tmp == last1)
				return res;
			else
			{
				res = tmp;
				first1 = tmp;
				++first1;
			}
		}
	}

	//bidirectional_iterator_tag版本
	template<class BidirectionalIter1,class BidirectionalIter2>
	BidirectionalIter1 find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
		BidirectionalIter2 first2, BidirectionalIter2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag)
	{
		typedef reverse_iterator<BidirectionalIter1> reiter1;
		typedef reverse_iterator<BidirectionalIter2> reiter2;
		reiter1 rlast1(first1);
		reiter2 rlast2(first2);
		reiter1 rres = mystl::search(reiter1(last1), rlast1, reiter2(last2), rlast2);
		if (rres == rlast1)
			return last1;
		else
		{
			auto res = rres.base();
			mystl::advance(res, -mystl::distance(first2, last2));
			return res;
		}
	}

	template<class InputIter1,class InputIter2>
	InputIter1 find_end(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
	{
		return find_end_dispatch(first1, last1, first2, last2, 
			mystl::iterator_category(first1), 
			mystl::iterator_category(first2));
	}

	//使用Comp进行比较
	//forward_iterator_tag版本
	template<class ForwardIter1, class ForwardIter2,class Compare>
	ForwardIter1 find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2,
		Compare comp,forward_iterator_tag, forward_iterator_tag)
	{
		if (first2 == last2)
			return last1;
		auto res = last1;
		while (true)
		{
			auto tmp = mystl::search(first1, last1, first2, last2,comp);
			if (tmp == last1)
				return res;
			else
			{
				res = tmp;
				first1 = tmp;
				++first1;
			}
		}
	}

	//bidirectional_iterator_tag版本
	template<class BidirectionalIter1, class BidirectionalIter2,class Compare>
	BidirectionalIter1 find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
		BidirectionalIter2 first2, BidirectionalIter2 last2,Compare comp,
		bidirectional_iterator_tag, bidirectional_iterator_tag)
	{
		typedef reverse_iterator<BidirectionalIter1> reiter1;
		typedef reverse_iterator<BidirectionalIter2> reiter2;
		reiter1 rlast1(first1);
		reiter2 rlast2(first2);
		reiter1 rres = mystl::search(reiter1(last1), rlast1, reiter2(last2), rlast2,comp);
		if (rres == rlast1)
			return last1;
		else
		{
			auto res = rres.base();
			mystl::advance(res, -mystl::distance(first2, last2));
			return res;
		}
	}

	template<class InputIter1, class InputIter2,class Compare>
	InputIter1 find_end(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2,Compare comp)
	{
		return find_end_dispatch(first1, last1, first2, last2,comp,
			mystl::iterator_category(first1),
			mystl::iterator_category(first2));
	}

	//find_first_of
	//在[first1,last1)中查询[first2,last2)中元素出现的首个位置，返回其迭代器
	template<class InputIter1,class ForwardIter1>
	InputIter1 find_first_of(InputIter1 first1, InputIter1 last1, ForwardIter1 first2, ForwardIter1 last2)
	{

		for (; first1 != last1; ++first1)
		{
			for (auto cur = first2; cur != last2; ++cur)
			{
				if (*cur == *first1)
					return first1;
			}
		}
		return first1;
	}

	template<class InputIter1, class ForwardIter1,class Compare>
	InputIter1 find_first_of(InputIter1 first1, InputIter1 last1, ForwardIter1 first2, ForwardIter1 last2,Compare comp)
	{

		for (; first1 != last1; ++first1)
		{
			for (auto cur = first2; cur != last2; ++cur)
			{
				if (comp(*cur,*first1)
					return first1;
			}
		}
		return first1;
	}

	//for_each
	//对每个元素执行f()，但不能改变元素值
	//可返回值，但会被忽略
	template<class InputIter,class Func>
	Func for_each(InputIter first, InputIter last, Func f)
	{
		for (; first != last; ++first)
		{
			f(*first);
		}
		return f;
		//为什么要返回f?
	}

	//adjacent_find
	//找出第一对匹配的相邻元素
	template<class ForwardIter>
	ForwardIter adjacent_find(ForwardIter first, ForwardIter last)
	{
		if (first == last)
			return last;
		auto next = first;
		while (++next != last)
		{
			if (*first==*next)
				return first;
			first = next;
		}
		return last;
	}

	template<class ForwardIter,class Compare>
	ForwardIter adjacent_find(ForwardIter first, ForwardIter last, Compare comp)
	{
		if (first == last)
			return last;
		auto next = first;
		while (++next != last)
		{
			if (comp(*first, *last))
				return first;
			first = next;
		}
		return last;
	}

	//lower_bound
	//查找第一个不小于value的元素
	template<class ForwardIter,class T>
	ForwardIter lower_bound_dispatch(ForwardIter first, ForwardIter last, const T& value,forward_iterator_tag)
	{
		auto len = mystl::distance(first, last);
		auto half = len;
		ForwardIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first;
			mystl::advance(mid, half);
			if (*mid < value)
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}
		return first;
	}

	template<class RandomIter,class T>
	RandomIter lower_bound_dispatch(RandomIter first, RandomIter last, const T& value,random_access_iterator_tag)
	{
		auto len = last - first;
		auto half = len;
		RandomIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first + half;
			if (*mid < value)
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}
		return first;
	}

	template<class InputIter,class T>
	InputIter lower_bound(InputIter first, InputIter last, const T& value)
	{
		return lower_bound_dispatch(first, last, value, mystl::iterator_category(first));
	}

	template<class ForwardIter, class T, class Compare >
	ForwardIter lower_bound_dispatch(ForwardIter first, ForwardIter last, const T& value,Compare comp, forward_iterator_tag)
	{
		auto len = mystl::distance(first, last);
		auto half = len;
		ForwardIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first;
			mystl::advance(mid, half);
			if (comp(*mid,value))
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}
		return first;
	}

	template<class RandomIter, class T,class Compare>
	RandomIter lower_bound_dispatch(RandomIter first, RandomIter last, const T& value,Compare comp, random_access_iterator_tag)
	{
		auto len = last - first;
		auto half = len;
		RandomIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first + half;
			if (comp( * mid, value))
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
			else
			{
				len = half;
			}
		}
		return first;
	}

	template<class InputIter, class T,class Compare>
	InputIter lower_bound(InputIter first, InputIter last, const T& value,Compare comp)
	{
		return lower_bound_dispatch(first, last, value,comp ,mystl::iterator_category(first));
	}

	//upper_bound
	template<class ForwardIter,class T>
	ForwardIter upper_bound_dispatch(ForwardIter first, ForwardIter last, const T& value,forward_iterator_tag)
	{
		auto len = mystl::distance(first, last);
		auto half = len;
		ForwardIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first;
			mystl::advance(mid, half);
			if (*mid > value)
			{
				len = half;
			}
			else
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
		}
		return first;
	}

	template<class RandomIter,class T>
	RandomIter upper_bound_dispatch(RandomIter first, RandomIter last, const T& value, random_access_iterator_tag)
	{
		auto len = last - first;
		auto half = len;
		RandomIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first+half;
			if (*mid > value)
			{
				len = half;
			}
			else
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
		}
		return first;
	}

	template<class ForwardIter,class T>
	ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& value)
	{
		return upper_bound_dispatch(first, last, value, mystl::iterator_category(first));
	}

	template<class ForwardIter, class T,class Compare>
	ForwardIter upper_bound_dispatch(ForwardIter first, ForwardIter last, const T& value,Compare comp, forward_iterator_tag)
	{
		auto len = mystl::distance(first, last);
		auto half = len;
		ForwardIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first;
			mystl::advance(mid, half);
			if (comp(value,*mid))
			{
				len = half;
			}
			else
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
		}
		return first;
	}

	template<class RandomIter, class T, class Compare >
	RandomIter upper_bound_dispatch(RandomIter first, RandomIter last, const T& value, Compare comp, random_access_iterator_tag)
	{
		auto len = last - first;
		auto half = len;
		RandomIter mid;
		while (len > 0)
		{
			half = len >> 1;
			mid = first + half;
			if (comp(value,*mid))
			{
				len = half;
			}
			else
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
		}
		return first;
	}

	template<class ForwardIter, class T,class Compare>
	ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& value,Compare comp)
	{
		return upper_bound_dispatch(first, last, value, comp, mystl::iterator_category(first));
	}

	//binary_search
	template<class ForwardIter,class T>
	bool binary_search(ForwardIter first, ForwardIter last, const T& value)
	{
		ForwardIter tmp = mystl::lower_bound(first, last, value);
		return tmp != last && *tmp == value;
	}

	template<class ForwardIter,class T,class Compare>
	bool binart_search(ForwardIter first, ForwardIter last, const T& value, Compare comp)
	{
		ForwardIter tmp = mystl::lower_bound(first, last, value, comp);
		return tmp != last && !comp(value, *tmp);
	}

	//equal_range
	//查找与value相等元素组成的区间,返回第一个迭代器指向第一个不小于value的元素，第二个迭代器指向第一个大于value的元素
	template<class ForwardIter,class T>
	mystl::pair<ForwardIter, ForwardIter> equal_range_dispatch(ForwardIter first, ForwardIter last, const T& value,forward_iterator_tag)
	{
		ForwardIter left = mystl::lower_bound_dispatch(first, last, value, forward_iterator_tag);
		if(left==last)
			return mystl::pair<ForwardIter, ForwardIter>(last,last);
		ForwardIter right = mystl::upper_bound_dispatch(first, last, value, forward_iterator_tag);
		return mystl::pair<ForwardIter, ForwardIter>(left, right);
	}

	template<class RandomIter,class T>
	mystl::pair<RandomIter, RandomIter> equal_range_dispatch(RandomIter first, RandomIter last, const T& value, random_access_iterator_tag)
	{
		RandomIter left = mystl::lower_bound_dispatch(first, last, value, random_access_iterator_tag);
		if (left == last)
			return mystl::pair<RandomIter, RandomIter>(last, last);
		RandomIter right = mystl::upper_bound_dispatch(first, last, value, random_access_iterator_tag);
		return mystl::pair<RandomIter, RandomIter>(left, right);
	}

	template<class ForwardIter,class T>
	mystl::pair<ForwardIter, ForwardIter> equal_range(ForwardIter first, ForwardIter last, const T& value)
	{
		return equal_range_dispatch(first, last, value, mystl::iterator_category(first));
	}

	template<class ForwardIter, class T,class Compare>
	mystl::pair<ForwardIter, ForwardIter> equal_range_dispatch(ForwardIter first, ForwardIter last, const T& value,
		Compare comp,forward_iterator_tag)
	{
		ForwardIter left = mystl::lower_bound_dispatch(first, last, value,comp, forward_iterator_tag);
		if (left == last)
			return mystl::pair<ForwardIter, ForwardIter>(last, last);
		ForwardIter right = mystl::upper_bound_dispatch(first, last, value,comp, forward_iterator_tag);
		return mystl::pair<ForwardIter, ForwardIter>(left, right);
	}

	template<class RandomIter, class T,class Compare>
	mystl::pair<RandomIter, RandomIter> equal_range_dispatch(RandomIter first, RandomIter last, const T& value, 
		Compare comp,random_access_iterator_tag)
	{
		RandomIter left = mystl::lower_bound_dispatch(first, last, value,comp, random_access_iterator_tag);
		if (left == last)
			return mystl::pair<RandomIter, RandomIter>(last, last);
		RandomIter right = mystl::upper_bound_dispatch(first, last, value,comp, random_access_iterator_tag);
		return mystl::pair<RandomIter, RandomIter>(left, right);
	}

	template<class ForwardIter, class T,class Compare>
	mystl::pair<ForwardIter, ForwardIter> equal_range(ForwardIter first, ForwardIter last, const T& value,Compare comp)
	{
		return equal_range_dispatch(first, last, value, comp, mystl::iterator_category(first));
	}

	//generate
	//将函数对象gen的运算结果对每个元素赋值
	template<class ForwardIter,class Generator>
	void generate(ForwardIter first, ForwardIter last, Generator gen)
	{
		for (; first != last; ++first)
		{
			*first = gen();
		}
	}

	//generate_n
	template<class ForwardIter,class Size,class Generator>
	void generator_n(ForwardIter first, Size n, Generator gen)
	{
		while (n>0)
		{
			*first = gen();
			++first;
			--n;
		}
	}

	//includes
	//升序序列1是否包含升序序列2
	template<class InputIter1,class InputIter2>
	bool includes(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				++first1;
			}
			else if(*first1>*first2)
			{
				return false;
			}
			else
			{
				++first1;
				++first2;
			}
		}
		return first2 == last2;
	}

	template<class InputIter1, class InputIter2,class Compare>
	bool includes(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2,Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first1,*first2))
			{
				++first1;
			}
			else if (comp(*first2,*first1))
			{
				return false;
			}
			else
			{
				++first1;
				++first2;
			}
		}
		return first2 == last2;
	}

	//is_heap
	//检查元素是否是一个堆
	//大根堆
	template<class RandomIter>
	bool is_heap(RandomIter first, RandomIter last)
	{
		auto n = mystl::distance(first, last);
		auto parent = 0;
		for (auto child = 1; child < n; ++child)
		{
			if (first[parent] < first[child])
				return false;
			if ((child & 1) == 0)
				++parent;
		}
		return true;
	}

	template<class RandomIter,class Compare>
	bool is_heap(RandomIter first, RandomIter last, Compare comp)
	{
		auto n = mystl::distance(first, last);
		auto parent = 0;
		for (auto child = 1; child < n; ++child)
		{
			if (comp(first[parent],first[child]))
				return false;
			if ((child & 1) == 0)
				++parent;
		}
		return true;
	}

	//is_sorted
	//是否升序
	template<class ForwardIter>
	bool is_sorted(ForwardIter first, ForwardIter last)
	{
		if (first == last)
			return true;
		ForwardIter next = first;
		++next;
		while (next != last)
		{
			if (*next < *first)
				return false;
			first = next;
			++next;
		}
		return true;
	}

	template<class ForwardIter,class Compare>
	bool is_sorted(ForwardIter first, ForwardIter last, Compare comp)
	{
		if (first == last)
			return true;
		ForwardIter next = first;
		++next;
		while (next != last)
		{
			if (comp(*next, *first))
				return false;
			first = next;
			++next;
		}
		return true;
	}

	//median
	//三个值的中间值
	template<class T>
	const T& median(const T& left, const T& mid, const T& right)
	{
		if (left < mid)
		{
			if (right < left)
				return left;
			else if (mid < right)
				return mid;
			else
				return right;
		}
		else if (left < right)
			return left;
		else if (mid < right)
			return right;
		else
			return mid;
	}

	template<class T,class Compare>
	const T& median(const T& left, const T& mid, const T& right, Compare comp)
	{
		if (comp(left, mid))
		{
			if (comp(right, left))
				return left;
			else if (comp(mid, right))
				return mid;
			else
				return right;
		}
		else if (comp(left, right))
			return left;
		else if (comp(mid, right))
			return right;
		else
			return mid;
	}

	//max_element
	template<class ForwardIter>
	ForwardIter max_element(ForwardIter first, ForwardIter last)
	{
		ForwardIter res = first;
		while (first != last)
		{
			if (*res < *first)
				res = first;
			++first;
		}
		return res;
	}

	template<class ForwardIter,class Compare>
	ForwardIter max_element(ForwardIter first, ForwardIter last,Compare comp)
	{
		ForwardIter res = first;
		while (first != last)
		{
			if (comp(*res, *first))
				res = first;
			++first;
		}
		return res;
	}

	//min_element
	template<class ForwardIter>
	ForwardIter min_element(ForwardIter first, ForwardIter last)
	{
		ForwardIter res = first;
		while (first != last)
		{
			if (*first < *res)
				res = first;
			++first;
		}
		return res;
	}

	template<class ForwardIter, class Compare>
	ForwardIter min_element(ForwardIter first, ForwardIter last, Compare comp)
	{
		ForwardIter res = first;
		while (first != last)
		{
			if (comp(*first, *res))
				res = first;
			++first;
		}
		return res;
	}

	//swap_ranges
	template<class ForwardIter1,class ForwardIter2>
	ForwardIter2 swap_ranges(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
	{
		while (first1 != last1)
		{
			mystl::swap(*first1, *first2);
			++first1;
			++first2;
		}
		return first2;
	}

	//transform
	template<class InputIter,class OutputIter,class UnaryOperation>
	OutputIter transform(InputIter first, InputIter last, OutputIter res, UnaryOperation unary_op)
	{
		while (first != last)
		{
			*res = unary_op(*first);
			++first;
			++res;
		}
		return res;
	}

	template<class InputIter1,class InputIter2, class OutputIter, class BinaryOperation>
	OutputIter transform(InputIter1 first1, InputIter1 last1,InputIter2 first2, OutputIter res, BinaryOperation binary_op)
	{
		while (first != last)
		{
			*res = binary_op(*first1, *first2);
			++first1;
			++first2;
			++res;
		}
		return res;
	}

	//remove_copy
	template<class InputIter,class OutputIter,class T>
	OutputIter remove_copy(InputIter first, InputIter last, OutputIter res, const T& value)
	{
		while (first != last)
		{
			if (*first != value)
			{
				*res = *first;
				++res;
			}
			++first;
		}
		return res;
	}

	//remove
	template<class ForwardIter,class T>
	ForwardIter remove(ForwardIter first, ForwardIter last, const T& value)
	{
		first = mystl::find(first, last, value);
		ForwardIter next = first;
		return first == last ? first : mystl::remove_copy(++next, last, first, value);
	}

	//remove_copy_if
	template<class ForwardIter,class OutputIter,class UnaryOperation>
	OutputIter remove_copy_if(ForwardIter first, ForwardIter last, OutputIter res, UnaryOperation unary_op)
	{
		while (first != last)
		{
			if (!unary_op(*first))
				*res++ = *first;
			++first;
		}
		return res;
	}

	//remove_if
	template<class ForwardIter,class UnaryOperation>
	ForwardIter remove_if(ForwardIter first, ForwardIter last, UnaryOperation unary_op)
	{
		first = mystl::find_if(first, last, unary_op);
		ForwardIter next = first;
		return first == last ? first : mystl::remove_copy_if(++next, last, first, unary_op);
	}

	//replace
	template<class ForwardIter,class T>
	void replace(ForwardIter first, ForwardIter last, const T& oldvalue, const T& newvalue)
	{
		while (first != last)
		{
			if (*first == oldvalue)
				*first = newvalue;
			++first;
		}
	}

	//replace_copy
	template<class InputIter,class OutputIter,class T>
	OutputIter replace_copy(InputIter first, InputIter last, OutputIter res, const T& oldvalue, const T& newvalue)
	{
		while (first != last)
		{
			if (*first == oldvalue)
			{
				*res++ = newvalue;
			}
			else
			{
				*res++ = *first;
			}
			++first;
		}
		return res;
	}

	//replace_if
	template<class ForwardIter,class T,class UnaryOperation>
	void replace_if(ForwardIter first, ForwardIter last,const T& newvalue, UnaryOperation unary_op)
	{
		while (first != last)
		{
			if (unary_op(*first))
				*first = newvalue;
			++first;
		}
	}

	//replace_copy_if
	template<class InputIter,class OutputIter,class T,class UnaryOperation>
	OutputIter replace_copy_if(InputIter first, InputIter last, OutputIter res, const T& newvalue, UnaryOperation unary_op)
	{
		while (first != last)
		{
			if (unary_op(*first))
			{
				*res = newvalue;
			}
			else
			{
				*res = *first;
			}
			++res;
			++first;
		}
		return res;
	}





}









#endif // !MYSTL_ALGO_H_
