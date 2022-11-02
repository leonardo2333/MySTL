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

	//reverse
	//bidirectional_iterator版本
	template<class BidirectionalIter>
	void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,bidirectional_iterator_tag)
	{
		while (first != last && first != --last)
		{
			mystl::iter_swap(first, last);
			++first;
		}
	}

	//random_iterator版本
	template<class RandomIter>
	void reverse_dispatch(RandomIter first, RandomIter last, random_access_iterator_tag)
	{
		while(first<last)
		{
			mystl::iter_swap(first++, --last);
		}
	}

	template<class BidirectionalIter>
	void reverse(BidirectionalIter first, BidirectionalIter last)
	{
		return reverse_dispatch(first, last, mystl::iterator_category(first));
	}

	//reverse_copy
	template<class BidirectionalIter, class OutputIter>
	OutputIter reverse_copy(BidirectionalIter first, BidirectionalIter last, OutputIter res)
	{
		while (first != last)
		{
			--last;
			*res = *last;
			++res;
		}
		return res;
	}

	//random_shuffle
	//随机重排
	template<class RandomIter>
	void random_shuffle(RandomIter first, RandomIter last)
	{
		if (first == last)
			return;
		srand((unsigned)time(0));
		for (auto i = last - 1; i != first; --i)
		{
			mystl::iter_swap(i, first + (rand() % (i - first - 1)));
		}
	}

	template<class RandomIter,class RandomNumberGenerator>
	void random_shuffle(RandomIter first, RandomIter last, RandomNumberGenerator& rand)
	{
		if (first == last)
			return;
		for (auto i = last - 1; i != first; --i)
		{
			mystl::iter_swap(i, first + (rand() % (i - first - 1)));
		}
	}

	//rotate
	//将[first,mid)和[mid,last)之内的元素互换，返回交换后的mid
	//forward_iterator版本
	template<class ForwardIter>
	ForwardIter rotate_dispatch(ForwardIter first, ForwardIter mid, ForwardIter last,forward_iterator_tag)
	{
		auto first2 = mid;
		do
		{
			mystl::swap(*first++, *first2++);
			if (first == mid)
				mid = first2;
		} while (first2 != last);

		auto new_mid = first;
		first2 = mid;
		while (first2 != last)
		{
			mystl::swap(*first++, *first2++);
			if (first == mid)
				mid = first2;
			else if (first2 == last)
				first2 = mid;
		}
		return new_mid;
	}

	//bidirectional_iterator版本
	template<class BidirectionalIter>
	BidirectionalIter rotate_dispatch(BidirectionalIter first, BidirectionalIter mid, BidirectionalIter last,
		bidirectional_iterator_tag)
	{
		mystl::reverse_dispatch(first, mid, bidirectional_iterator_tag);
		mystl::reverse_dispatch(mid, last, bidirectional_iterator_tag);
		while (first != mid && mid != last)
		{
			mystl::swap(*first++, *--last);
		}
		if (first == mid)
		{
			mystl::reverse_dispatch(mid, last, bidirectional_iterator_tag);
			return last;
		}
		else
		{
			mystl::reverse_dispatch(first, mid, bidirectional_iterator_tag);
			return first;
		}
	}

	//最大公约数
	template<class T>
	T rgcd(T m, T n)
	{
		while (n != 0)
		{
			auto t = m % n;
			m = n;
			n = t;
		}
		return m;
	}

	//random_iterator版本
	template<class RandomIter>
	RandomIter rotate_dispatch(RandomIter first, RandomIter mid, RandomIter last, random_access_iterator_tag)
	{
		auto n = last - first;
		auto l = mid - first;
		auto r = n - l;
		auto res = first + (last - mid);
		if (l == r)
		{
			mystl::swap_ranges(first, mid, mid);
			return res;
		}
		auto cycle_times = rgcd(n, l);
		for (auto i = 0; i < cycle_times; ++i)
		{
			auto tmp = *first;
			auto p = first;
			if (l < r)
			{
				for (auto j = 0; j < r / cycle_times; ++j)
				{
					if (p > first + r)
					{
						*p = *(p - r);
						p -= r;
					}
					*p = *(p + l);
					p += l;
				}
			}
			else
			{
				for (auto j = 0; j < l / cycle_times - 1; ++j)
				{
					if (p < last - l)
					{
						*p = *(p + l);
						p += l;
					}
					else
					{
						*p = *(p - r);
						p -= r;
					}
				}
			}
			*p = tmp;
			++first;
		}
		return res;
	}

	template<class ForwardIter>
	ForwardIter rotate(ForwardIter first, ForwardIter mid, ForwardIter last)
	{
		if (first == mid)
			return last;
		if (mid == last)
			return first;
		return rotate_dispatch(first, mid, last, mystl::iterator_category(first));
	}

	//rotate_copy
	template<class ForwardIter,class OutputIter>
	ForwardIter rotate_copy(ForwardIter first, ForwardIter mid, ForwardIter last, OutputIter res)
	{
		return mystl::copy(first, mid, mystl::copy(mid, last, res));
	}

	//is_premutation
	//判断序列1是否为序列2的排列组合
	template<class ForwardIter1,class ForwardIter2,class BinaryOperation>
	bool is_permutation_aux(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2,
		BinaryOperation binary_op)
	{
		constexpr bool is_ra_it = mystl::is_random_access_iterator<ForwardIter1>::value &&
			mystl::is_random_access_iterator<ForwardIter2>::value;
		if (is_ra_it)
		{
			auto len1 = last1 - first1;
			auto len2 = last2 - first2;
			if (len1 != len2)
				return false;
		}
		
		//找出相同的前缀段
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (!binary_op(*first1, *first2))
				break;
		}
		if (is_ra_it)
		{
			if (first1 == last1)
				return true;
		}
		else
		{
			auto len1 = mystl::distance(first1, last1);
			auto len2 = mystl::distance(first2, last2);
			if (len1 == 0 && len2 == 0)
				return true;
			if (len1 != len2)
				return false;
		}

		//判断剩余部分
		for (auto i = first1; i != last1; ++i)
		{
			bool is_repeated = false;
			for (auto j = first1; j != i; ++j)
			{
				if (binary_op(*j, *i))
				{
					is_repeated = true;
					break;
				}
			}

			if (!is_repeated)
			{
				auto c2 = 0;
				for (auto j = first2; j != last2; ++j)
				{
					if (binary_op(*i, *j))
						++c2;
				}
				if (c2 == 0)
					return false;
				auto c1 = 1;
				j = i;
				for (++j; j != last1; ++j)
				{
					if (binary_op(*i, *j))
						++c1;
				}
				if (c1 != c2)
					return false;
			}
		}
		return true;
	}

	template<class ForwardIter1,class ForwardIter2,class BinaryOperation>
	bool is_permutation(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2,
		BinaryOperation binary_op)
	{
		return is_permutation_aux(first1, last1, first2, last2, binary_op);
	}

	template<class ForwardIter1,class ForwardIter2>
	bool is_permutation(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, ForwardIter2 last2)
	{
		static_assert(std::is_same<typename mystl::iterator_traits<ForwardIter1>::value_type,
			typename mystl::iterator_traits<ForwardIter2>::value_type>::value, "Iterator type should be same");
		return is_permutation_aux(first1, last1, first2, last2, 
			mystl::equal_to<typename mystl::iterator_traits<ForwardIter1>::value_type>());
	}

	//next_permutation
	//取得序列的下一个排列组合
	template<class BidirectionalIter>
	bool next_permutation(BidirectionalIter first, BidirectionalIter last)
	{
		auto i = last;
		if (first == last || first == --i)
			return false;
		while (true)
		{
			auto ii = i;
			if (*--i < *ii)
			{
				auto j = last;
				while(!(*i<*--j)){}
				mystl::iter_swap(i, j);
				mystl::reverse(ii, last);
				return true;
			}
			if (i == first)
			{
				mystl::reverse(first, last);
				return false;
			}
		}
	}

	template<class BidirectionalIter,class Compare>
	bool next_permutation(BidirectionalIter first, BidirectionalIter last,Compare comp)
	{
		auto i = last;
		if (first == last || first == --i)
			return false;
		while (true)
		{
			auto ii = i;
			if (comp( * --i, *ii))
			{
				auto j = last;
				while (!comp(*i , *--j)) {}
				mystl::iter_swap(i, j);
				mystl::reverse(ii, last);
				return true;
			}
			if (i == first)
			{
				mystl::reverse(first, last);
				return false;
			}
		}
	}

	//prev_permutation
	template<class BidirectionalIter>
	bool prev_permutation(BidirectionalIter first, BidirectionalIter last)
	{
		auto i = last;
		if (first == last || first == --i)
			return false;
		while (true)
		{
			auto ii = i;
			if (*ii < *--i)
			{
				auto j = last;
				while (!(*--j < *i)) {}
				mystl::iter_swap(i, j);
				mystl::reverse(ii, last);
				return true;
			}
			if (i == first)
			{
				mystl::reverse(first, last);
				return false;
			}
		}
	}

	template<class BidirectionalIter, class Compare>
	bool prev_permutation(BidirectionalIter first, BidirectionalIter last, Compare comp)
	{
		auto i = last;
		if (first == last || first == --i)
			return false;
		while (true)
		{
			auto ii = i;
			if (comp(*ii, *--i))
			{
				auto j = last;
				while (!comp(*--j, *i)) {}
				mystl::iter_swap(i, j);
				mystl::reverse(ii, last);
				return true;
			}
			if (i == first)
			{
				mystl::reverse(first, last);
				return false;
			}
		}
	}

	//merge
	template<class InputIter1,class InputIter2,class OutputIter>
	OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter res)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*res = first1;
				++first1;
			}
			else
			{
				*res = first2;
				++first2;
			}
			++res;
		}
		return first1 == last1 ? mystl::copy(first2, last2, res) : mystl::copy(first1, last1, res);
	}

	template<class InputIter1, class InputIter2, class OutputIter,class Compare>
	OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter res,Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp( * first1 , *first2))
			{
				*res = first1;
				++first1;
			}
			else
			{
				*res = first2;
				++first2;
			}
			++res;
		}
		return first1 == last1 ? mystl::copy(first2, last2, res) : mystl::copy(first1, last1, res);
	}

	//inplace_merge
	template<class BidirectionalIter,class Distance>
	void merge_without_buffer(BidirectionalIter first, BidirectionalIter mid, BidirectionalIter last,
		Distance len1, Distance len2)
	{
		if (len1 == 0 || len2 == 0)
			return;
		if (len1 + len2 == 2)
		{
			if (*mid < *first)
				mystl::iter_swap(first, mid);
			return;
		}
		auto first_cut = first;
		auto second_cut = mid;
		Distance len11 = 0, len22 = 0;
		if (len1 > len2)
		{
			len11 = len1 >> 1;
			mystl::advance(first_cut, len11);
			second_cut = mystl::lower_bound(mid, last, *first_cut);
			len22 = mystl::distance(mid, second_cut);
		}
		else
		{
			len22 = len2 >> 1;
			mystl::advance(second_cut, len22);
			first_cut = mystl::upper_bound(first, mid, *second_cut);
			len11 = mystl::distance(first, first_cut);
		}
		auto new_mid = mystl::rotate(first_cut, mid, second_cut);
		mystl::merge_without_buffer(first, first_cut, new_mid, len11, len22);
		mystl::merge_without_buffer(new_mid, second_cut, last, len1 - len11, len2 - len22);
	}

	template<class BidirectionalIter1,class BidirectionalIter2>
	BidirectionalIter1 merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
		BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter1 res)
	{
		if (first1 == last1)
			return mystl::copy_backward(first2, last2, res);
		if (first2 == last2)
			return mystl::copy_backward(first1, last1, res);
		--last1;
		--last2;
		while (true)
		{
			if (*last2 < *last1) 
			{
				*--res = *last1;
				if (first1 == last1)
					return mystl::copy_backward(first2, last2, res);
				--last1;
			}
			else
			{
				*--res = *last2;
				if (first2 == last2)
					return mystl::copy_backward(first1, last1, res);
				--last2;
			}
		}
	}

	template<class BidirectionalIter1,class BidirectionalIter2,class Distance>
	BidirectionalIter1 rotate_adaptive(BidirectionalIter1 first, BidirectionalIter1 mid, BidirectionalIter1 last,
		Distance len1, Distance len2, BidirectionalIter2 buffer, Distance buffer_size)
	{
		BidirectionalIter2 buffer_end;
		if (len1 > len2 && len2 <= buffer_size)
		{
			buffer_end = mystl::copy(mid, last, buffer);
			mystl::copy_backward(first, mid, last);
			return mystl::copy(buffer, buffer_end, first);
		}
		else if (len1 <= buffer_size)
		{
			buffer_end = mystl::copy(first, mid, buffer);
			mystl::copy(mid, last, first);
			return mystl::copy_backward(buffer, buffer_end, last);
		}
		else
		{
			return mystl::rotate(first, mid, last);
		}
	}

	template<class BidirectionalIter1, class BidirectionalIter2, class Distance>
	void merge_adaptive(BidirectionalIter1 first, BidirectionalIter1 mid, BidirectionalIter1 last,
		Distance len1, Distance len2, BidirectionalIter2 buffer, Distance buffer_size)
	{
		BidirectionalIter2 buffer_end;
		if (len1 > len2 && len2 <= buffer_size)
		{
			buffer_end = mystl::copy(mid, last, buffer);
			mystl::merge_backward(buffer, buffer_end, first, mid, last);
		}
		else if (len1 <= buffer_size)
		{
			buffer_end = mystl::copy(first, mid, buffer);
			mystl::merge(buffer, buffer_end, mid, last, first);
		}
		else
		{
			auto first_cut = first;
			auto second_cut = mid;
			Distance len11 = 0, len22 = 0;
			if (len1 > len2)
			{
				len11 = len1 >> 1;
				mystl::advance(first_cut, len11);
				second_cut = mystl::lower_bound(mid, last, *first_cut);
				len22 = mystl::distance(mid, second_cut);
			}
			else
			{
				len22 = len2 >> 1;
				mystl::advance(second_cut, len22);
				first_cut = mystl::upper_bound(first, mid, *second_cut);
				len11 = mystl::distance(first, first_cut);
			}
			auto new_mid = mystl::rotate_adaptive(first_cut, mid, second_cut, len1-len11, len22, buffer, buffer_size);
			mystl::merge_adaptive(first, first_cut, new_mid, len11, len2, buffer, buffer_size);
			mystl::merge_adaptive(new_mid, second_cut, last, len1-len11, len2 - len22, buffer, buffer_size);
		}
	}

	template<class BidirectionalIter,class T>
	void inplace_merge_aux(BidirectionalIter first, BidirectionalIter mid, BidirectionalIter last, T*)
	{
		auto len1 = mystl::distance(first, mid);
		auto len2 = mystl::distance(mid, last);
		temporary_buffer<BidirectionalIter, T> buf(first, last);
		if (!buf.begin())
		{
			mystl::merge_without_buffer(first, mid, last, len1, len2);
		}
		else
		{
			mystl::merge_adaptive(first, mid, last, len1, len2, buf.begin(), buf.size());
		}
	}

	template<class BidirectionalIter>
	void inplace_merge(BidirectionalIter first, BidirectionalIter mid, BidirectionalIter last)
	{
		if (first == mid || mid == last)
			return;
		mystl::inplace_merge_aux(first, mid, last, mystl::value_type(first));
	}

	//重载使用comp运算符
	template<class BidirectionalIter, class Distance,class Compare>
	void merge_without_buffer(BidirectionalIter first, BidirectionalIter mid, BidirectionalIter last,
		Distance len1, Distance len2,Compare comp)
	{
		if (len1 == 0 || len2 == 0)
			return;
		if (len1 + len2 == 2)
		{
			if (comp( * mid , *first))
				mystl::iter_swap(first, mid);
			return;
		}
		auto first_cut = first;
		auto second_cut = mid;
		Distance len11 = 0, len22 = 0;
		if (len1 > len2)
		{
			len11 = len1 >> 1;
			mystl::advance(first_cut, len11);
			second_cut = mystl::lower_bound(mid, last, *first_cut,comp);
			len22 = mystl::distance(mid, second_cut);
		}
		else
		{
			len22 = len2 >> 1;
			mystl::advance(second_cut, len22);
			first_cut = mystl::upper_bound(first, mid, *second_cut,comp);
			len11 = mystl::distance(first, first_cut);
		}
		auto new_mid = mystl::rotate(first_cut, mid, second_cut);
		mystl::merge_without_buffer(first, first_cut, new_mid, len11, len22,comp);
		mystl::merge_without_buffer(new_mid, second_cut, last, len1 - len11, len2 - len22,comp);
	}

	template<class BidirectionalIter1, class BidirectionalIter2,class Compare>
	BidirectionalIter1 merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
		BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter1 res,Compare comp)
	{
		if (first1 == last1)
			return mystl::copy_backward(first2, last2, res);
		if (first2 == last2)
			return mystl::copy_backward(first1, last1, res);
		--last1;
		--last2;
		while (true)
		{
			if (comp( * last2 ,*last1))
			{
				*--res = *last1;
				if (first1 == last1)
					return mystl::copy_backward(first2, last2, res);
				--last1;
			}
			else
			{
				*--res = *last2;
				if (first2 == last2)
					return mystl::copy_backward(first1, last1, res);
				--last2;
			}
		}
	}

	template<class BidirectionalIter1, class BidirectionalIter2, class Distance, class Compare >
	void merge_adaptive(BidirectionalIter1 first, BidirectionalIter1 mid, BidirectionalIter1 last,
		Distance len1, Distance len2, BidirectionalIter2 buffer, Distance buffer_size,Compare comp)
	{
		BidirectionalIter2 buffer_end;
		if (len1 > len2 && len2 <= buffer_size)
		{
			buffer_end = mystl::copy(mid, last, buffer);
			mystl::merge_backward(buffer, buffer_end, first, mid, last,comp);
		}
		else if (len1 <= buffer_size)
		{
			buffer_end = mystl::copy(first, mid, buffer);
			mystl::merge(buffer, buffer_end, mid, last, first,comp);
		}
		else
		{
			auto first_cut = first;
			auto second_cut = mid;
			Distance len11 = 0, len22 = 0;
			if (len1 > len2)
			{
				len11 = len1 >> 1;
				mystl::advance(first_cut, len11);
				second_cut = mystl::lower_bound(mid, last, *first_cut,comp);
				len22 = mystl::distance(mid, second_cut);
			}
			else
			{
				len22 = len2 >> 1;
				mystl::advance(second_cut, len22);
				first_cut = mystl::upper_bound(first, mid, *second_cut,comp);
				len11 = mystl::distance(first, first_cut);
			}
			auto new_mid = mystl::rotate_adaptive(first_cut, mid, second_cut, len1 - len11, len22, buffer, buffer_size);
			mystl::merge_adaptive(first, first_cut, new_mid, len11, len2, buffer, buffer_size,comp);
			mystl::merge_adaptive(new_mid, second_cut, last, len1 - len11, len2 - len22, buffer, buffer_size,comp);
		}
	}

	template<class BidirectionalIter, class T,class Compare>
	void inplace_merge_aux(BidirectionalIter first, BidirectionalIter mid, BidirectionalIter last,T*, Compare comp)
	{
		auto len1 = mystl::distance(first, mid);
		auto len2 = mystl::distance(mid, last);
		temporary_buffer<BidirectionalIter, T> buf(first, last);
		if (!buf.begin())
		{
			mystl::merge_without_buffer(first, mid, last, len1, len2,comp);
		}
		else
		{
			mystl::merge_adaptive(first, mid, last, len1, len2, buf.begin(), buf.size(),comp);
		}
	}

	template<class BidirectionalIter,class Compare>
	void inplace_merge(BidirectionalIter first, BidirectionalIter mid, BidirectionalIter last,Compare comp)
	{
		if (first == mid || mid == last)
			return;
		mystl::inplace_merge_aux(first, mid, last, mystl::value_type(first),comp);
	}

	//partial_sort
	//部分排序，使最小的N个元素排在序列首部
	//to be finished
	template<class RandomIter>
	void partial_sort(RandomIter first, RandomIter mid, RandomIter last)
	{
		static_assert(false);
	}

	template<class RandomIter,class Compare>
	void partial_sort(RandomIter first, RandomIter mid, RandomIter last, Compare comp)
	{

	}

	//partition
	//一元运算符判定为true的元素移到序列首部
	template<class BidirectionalIter,class UnaryOperation>
	BidirectionalIter partition(BidirectionalIter first, BidirectionalIter last, UnaryOperation unary_op)
	{
		while (true)
		{
			while (first != last && unary_op(*first))
			{
				++first;
			}
			if (first == last)
				break;
			--last;
			while (first != last && !unary_op(*last))
				--last;
			if (first == last)
				break;
			mystl::iter_swap(first, last);
			++first;
		}
		return first;
	}

	//partition_copy
	template<class InputIter,class OutputIter1,class OutputIter2,class UnaryOperation>
	mystl::pair<OutputIter1, OutputIter2> partition_copy(InputIter first, InputIter last,
		OutputIter1 res1, OutputIter2 res2, UnaryOperation unary_op)
	{
		while (first != last)
		{
			if (unary_op(*first))
			{
				*res1 = *first;
				++res1;
			}
			else
			{
				*res2 = *first;
				++res2;
			}
			++first;
		}
		return mystl::pair<OutputIter1, OutputIter2>(res1, res2);
	}

	//sort
	
	constexpr static size_t kSmallSectionSize = 128;
	//当区间小于该值时采用插入排序
	//用于控制分割恶化的情况

	//求log2(n)
	template<class Size>
	Size slg2(Size n)
	{
		Size res = 0;
		while (n > 1)
		{
			n >> =1;
			++res;
		}
		return res;
	}

	//unchecked_partition
	//分割函数
	template<class RandomIter,class T>
	RandomIter unchecked_partition(RandomIter first, RandomIter last, const T& pivot)
	{
		while (true)
		{
			while (*first < pivot)
				++first;
			--last;
			while (pivot < *last)
				--last;
			if (!(first < last))
				return first;
			mystl::iter_swap(first, last);
			++first;
		}
	}

	//内省式排序，先进行快速排序，当分割行为有恶化倾向时，改用堆排序
	template<class RandomIter,class Size>
	void intro_sort(RandomIter first, RandomIter last, Size depth_limit)
	{
		while (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			if (depth_limit == 0)//到达最大分割深度限制
			{
				mystl::partial_sort(first, last, last);//改用堆排序
				return;
			}
			--depth_limit;
			auto mid = mystl::median(*first, *(first + (last - first) / 2), *(last - 1));
			auto cut = mystl::unchecked_partition(first, last, mid);
			mystl::intro_sort(cut, last, depth_limit);//对[cur,last)排序
			last = cut;//对[first,cur)排序
		}
	}

	//unchecked_linear_insert
	//插入排序辅助函数
	template<class RandomIter,class T>
	void unchecked_linear_insert(RandomIter last, const T& value)
	{
		auto next = last;
		--next;
		while (value < *next)
		{
			*last = *next;
			last = next;
			--next;
		}
		*last = value;
	}
	//可能存在越界问题?×
	//insertion_sort中排除了value<*first的情况

	//unchecked_insertion_sort
	//插入排序函数
	template<class RandomIter>
	void unchecked_insertion_sort(RandomIter first, RandomIter last)
	{
		for (auto i = first+1; i != last; ++i)
		{
			mystl::unchecked_linear_insert(i, *i);
		}
	}

	//insertion_sort
	//插入排序函数
	template<class RandomIter>
	void insertion_sort(RandomIter first, RandomIter last)
	{
		if (first == last)
			return;
		for (auto i = first + 1; i != last; ++i)
		{
			auto value = *i;
			if (value < *first)
			{
				mystl::copy_backward(first, i, i + 1);
				*first = value;
			}
			else
			{
				mystl::unchecked_insertion_sort(i, value);
			}
		}
	}

	//final_insertion_sort
	//最终插入排序函数
	template<class RandomIter>
	void final_isertion_sort(RandomIter first, RandomIter last)
	{
		if (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			mystl::insertion_sort(first, first + kSmallSectionSize);
			mystl::unchecked_insertion_sort(first + kSmallSectionSize, last);
		}
		else
		{
			mystl::insertion_sort(first, last);
		}
	}

	template<class RandomIter>
	void sort(RandomIter first, RandomIter last)
	{
		if (first != last)
		{
			mystl::intro_sort(first, last, slg2(last - first) * 2);
			mystl::final_isertion_sort(first, last);
		}
	}
	
	//unchecked_partition
	//分割函数
	template<class RandomIter, class T,class Compare>
	RandomIter unchecked_partition(RandomIter first, RandomIter last, const T& pivot,Compare comp)
	{
		while (true)
		{
			while (comp( * first ,pivot))
				++first;
			--last;
			while (comp(pivot , *last))
				--last;
			if (!(first < last))
				return first;
			mystl::iter_swap(first, last);
			++first;
		}
	}

	//内省式排序，先进行快速排序，当分割行为有恶化倾向时，改用堆排序
	template<class RandomIter, class Size,class Compare>
	void intro_sort(RandomIter first, RandomIter last, Size depth_limit,Compare comp)
	{
		while (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			if (depth_limit == 0)//到达最大分割深度限制
			{
				mystl::partial_sort(first, last, last,comp);//改用堆排序
				return;
			}
			--depth_limit;
			auto mid = mystl::median(*first, *(first + (last - first) / 2), *(last - 1));
			auto cut = mystl::unchecked_partition(first, last, mid,comp);
			mystl::intro_sort(cut, last, depth_limit,comp);//对[cur,last)排序
			last = cut;//对[first,cur)排序
		}
	}

	//unchecked_linear_insert
	//插入排序辅助函数
	template<class RandomIter, class T,class Compare>
	void unchecked_linear_insert(RandomIter last, const T& value,Compare comp)
	{
		auto next = last;
		--next;
		while (comp(value , *next))
		{
			*last = *next;
			last = next;
			--next;
		}
		*last = value;
	}
	//可能存在越界问题?×
	//insertion_sort中排除了value<*first的情况

	//unchecked_insertion_sort
	//插入排序函数
	template<class RandomIter,class Compare>
	void unchecked_insertion_sort(RandomIter first, RandomIter last,Compare comp)
	{
		for (auto i = first + 1; i != last; ++i)
		{
			mystl::unchecked_linear_insert(i, *i,comp);
		}
	}

	//insertion_sort
	//插入排序函数
	template<class RandomIter,class Compare>
	void insertion_sort(RandomIter first, RandomIter last,Compare comp)
	{
		if (first == last)
			return;
		for (auto i = first + 1; i != last; ++i)
		{
			auto value = *i;
			if (comp(value , *first))
			{
				mystl::copy_backward(first, i, i + 1);
				*first = value;
			}
			else
			{
				mystl::unchecked_insertion_sort(i, value,comp);
			}
		}
	}

	//final_insertion_sort
	//最终插入排序函数
	template<class RandomIter,class Compare>
	void final_isertion_sort(RandomIter first, RandomIter last,Compare comp)
	{
		if (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			mystl::insertion_sort(first, first + kSmallSectionSize,comp);
			mystl::unchecked_insertion_sort(first + kSmallSectionSize, last,comp);
		}
		else
		{
			mystl::insertion_sort(first, last,comp);
		}
	}

	template<class RandomIter, class Compare >
	void sort(RandomIter first, RandomIter last,Compare comp)
	{
		if (first != last)
		{
			mystl::intro_sort(first, last, slg2(last - first) * 2,comp);
			mystl::final_isertion_sort(first, last,comp);
		}
	}

	//nth_element
	//对序列重排，使小于第n个元素的排在前，大于的排在后
	template<class RandomIter>
	void nth_element(RandomIter first, RandomIter nth, RandomIter last)
	{
		//nth直接给定了？
		//为什么不直接调用unchecked_partition?
		//传进来的nth只是指向第n个位置，对应的值并不是正确的值
		//通过一次次的二分，每次二分确定一个cut
		//根据cut和nth位置关系对对应区间进一步细分，直到得到结果
		//时间复杂度O(nlogn)
		if (nth == last)
			return;
		while (last - first > 3)
		{
			auto cut = mystl::unchecked_partition(first, last, 
				mystl::median(*first, *(last - 1), *(first + (last - first) / 2));
			if (cut <= nth)
				first = cut;
			else
				last = cut;
		}
		mystl::insertion_sort(first, last);
	}
	
	template<class RandomIter,class Compare>
	void nth_element(RandomIter first, RandomIter nth, RandomIter last,Compare comp)
	{
		if (nth == last)
			return;
		while (last - first > 3)
		{
			auto cut = mystl::unchecked_partition(first, last,
				mystl::median(*first, *(last - 1), *(first + (last - first) / 2),comp);
			if (cut <= nth)
				first = cut;
			else
				last = cut;
		}
		mystl::insertion_sort(first, last,comp);
	}

	//unique_copy
	//forward_iterator版本
	template<class InputIter,class ForwardIter>
	ForwardIter unique_copy_dispatch(InputIter first, InputIter last, ForwardIter res, forward_iterator_tag)
	{
		*res = *first;
		while (++first != last)
		{
			if (*res != *first)
				*++res = *first;
		}
		return ++res;
	}

	//output_iterator版本（只读）
	template<class InputIter,class OutputIter>
	OutputIter unique_copy_dispatch(InputIter first, InputIter last, OutputIter res, output_iterator_tag)
	{
		auto value = *first;
		*res = value;
		while (++first != last)
		{
			if (value != *first)
				*++res = *first;
		}
		return ++res;
	}

	template<class InputIter,class ForwardIter>
	ForwardIter unique_copy(InputIter first, InputIter last, ForwardIter res)
	{
		if (first == last)
			return res;
		return unique_copy_dispatch(first, last, res, mystl::iterator_category(res));
	}

	//forward_iterator版本
	template<class InputIter, class ForwardIter,class Compare>
	ForwardIter unique_copy_dispatch(InputIter first, InputIter last, ForwardIter res,Compare comp, forward_iterator_tag)
	{
		*res = *first;
		while (++first != last)
		{
			if (!comp( * res, *first))
				*++res = *first;
		}
		return ++res;
	}

	//output_iterator版本（只读）
	template<class InputIter, class OutputIter,class Compare>
	OutputIter unique_copy_dispatch(InputIter first, InputIter last, OutputIter res,Compare comp, output_iterator_tag)
	{
		auto value = *first;
		*res = value;
		while (++first != last)
		{
			if (!comp(value ,*first))
				*++res = *first;
		}
		return ++res;
	}

	template<class InputIter, class ForwardIter,class Compare>
	ForwardIter unique_copy(InputIter first, InputIter last, ForwardIter res,Compare comp)
	{
		if (first == last)
			return res;
		return unique_copy_dispatch(first, last, res, mystl::iterator_category(res),comp);
	}

	//unique
	template<class ForwardIter>
	ForwardIter unique(ForwardIter first, ForwardIter last)
	{
		first = mystl::adjacent_find(first, last);
		return mystl::unique_copy(first, last, first);
	}

	template<class ForwardIter,class Compare>
	ForwardIter unique(ForwardIter first, ForwardIter last,Compare comp)
	{
		first = mystl::adjacent_find(first, last,comp);
		return mystl::unique_copy(first, last, first,comp);
	}
}



#endif // !MYSTL_ALGO_H_
