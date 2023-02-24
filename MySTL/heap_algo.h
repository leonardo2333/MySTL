#pragma once
#ifndef MYSTL_HEAP_ALGO_H_
#define MYSTL_HEAP_ALGO_H_

//包含堆的算法：push_heap,pop_heap,sort_heap,make_heap

#include "iterator.h"

namespace mystl
{
	//push_heap
	//接受两个迭代器，表示heap首尾，且新元素一插入到底部容器的尾端，调整heap
	template<class RandomIter,class Distance,class T>
	void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value)
	{
		auto parent = (holeIndex - 1) / 2;
		while (holeIndex>topIndex&&*(first+parent)<value)
		{
			//大顶堆
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) >> 1;
		}
		*(first + holeIndex) = value;
	}

	template<class RandomIter,class Distance>
	void push_heap_d(RandomIter first, RandomIter last, Distance*)
	{
		mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
	}

	template<class RandomIter>
	void push_heap(RandomIter first, RandomIter last)
	{
		mystl::push_heap_d(first, last, difference_type(first));
	}

	template<class RandomIter, class Distance, class T,class Compare>
	void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value,Compare comp)
	{
		auto parent = (holeIndex - 1) >> 1;
		while (holeIndex > topIndex && comp( * (first + parent), value))
		{
			//大顶堆
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) >> 1;
		}
		*(first + holeIndex) = value;
	}

	template<class RandomIter, class Distance,class Compare>
	void push_heap_d(RandomIter first, RandomIter last, Distance*,Compare comp)
	{
		mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1),comp);
	}

	template<class RandomIter, class Compare>
	void push_heap(RandomIter first, RandomIter last,Compare comp)
	{
		mystl::push_heap_d(first, last, difference_type(first),comp);
	}

	//pop_heap
	//将容器的根节点移动到容器尾部，调整heap
	template<class RandomIter,class T,class Distance>
	void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
	{
		//下溯过程
		auto topIndex = holeIndex;
		auto rchild = (holeIndex + 1) << 1;
		while (rchild<len)
		{
			if (*(first + rchild) < *(first + rchild - 1))
			{
				--rchild;
			}
			*(first + holeIndex) = *(first + rchild);
			holeIndex = rchild;
			rchild = (rchild + 1) << 1;
		}
		if (rchild == len)
		{
			*(first + holeIndex) = *(first + rchild - 1);
			holeIndex = rchild - 1;
		}
		//上溯过程
		mystl::push_heap_aux(first, holeIndex, topIndex, value);
	}

	template<class RandomIter,class T,class Distance>
	void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*)
	{
		*result = *first;
		mystl::adjust_heap(first, static_cast<Distance>(0), last - first, value);
	}

	template<class RandomIter>
	void pop_heap(RandomIter first, RandomIter last)
	{
		mystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1), difference_type(first));
	}

	template<class RandomIter, class T, class Distance, class Compare>
	void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,Compare comp)
	{
		//下溯过程
		auto topIndex = holeIndex;
		auto rchild = (holeIndex + 1) << 1;
		while (rchild < len)
		{
			if (comp( * (first + rchild), *(first + rchild - 1)))
			{
				--rchild;
			}
			*(first + holeIndex) = *(first + rchild);
			holeIndex = rchild;
			rchild = (rchild + 1) << 1;
		}
		if (rchild == len)
		{
			*(first + holeIndex) = *(first + rchild - 1);
			holeIndex = rchild - 1;
		}
		//上溯过程
		mystl::push_heap_aux(first, holeIndex, topIndex, value,comp);
	}

	template<class RandomIter, class T, class Distance, class Compare>
	void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*,Compare comp)
	{
		*result = *first;
		mystl::adjust_heap(first, static_cast<Distance>(0), last - first, value,comp);
	}

	template<class RandomIter, class Compare>
	void pop_heap(RandomIter first, RandomIter last,Compare comp)
	{
		mystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1), difference_type(first),comp);
	}

	//sort_heap
	template<class RandomIter>
	void sort_heap(RandomIter first, RandomIter last)
	{
		while (last - first > 1)
		{
			mystl::pop_heap(first, last--);
		}
	}

	template<class RandomIter,class Compare>
	void sort_heap(RandomIter first, RandomIter last,Compare comp)
	{
		while (last - first > 1)
		{
			mystl::pop_heap(first, last--,comp);
		}
	}

	//make_heap
	template<class RandomIter,class Distance>
	void make_heap_aux(RandomIter first, RandomIter last, Distance*)
	{
		if (last - first < 2)
		{
			return;
		}
		auto len = last - first;
		auto holeIndex = (len - 2) >> 1;
		while (true)
		{
			mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex));
			if (holeIndex == 0)
				return;
			--holeIndex;
		}
	}

	template<class RandomIter>
	void make_heap(RandomIter first, RandomIter last)
	{
		mystl::make_heap_aux(first, last, difference_type(first));
	}

	template<class RandomIter, class Distance,class Compare>
	void make_heap_aux(RandomIter first, RandomIter last, Distance*,Compare comp)
	{
		if (last - first < 2)
		{
			return;
		}
		auto len = last - first;
		auto holeIndex = (len - 2) >> 1;
		while (true)
		{
			mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex),comp);
			if (holeIndex == 0)
				return;
			--holeIndex;
		}
	}

	template<class RandomIter,class Compare>
	void make_heap(RandomIter first, RandomIter last,Compare comp)
	{
		mystl::make_heap_aux(first, last, difference_type(first),comp);
	}
}

#endif // !MYSTL_HEAP_ALGO_H_
