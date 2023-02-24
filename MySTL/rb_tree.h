#pragma once
#ifndef MYSTL_RB_TREE_H_
#define MYSTL_RB_TREE_H_

#include<initializer_list>
#include<cassert>

#include"functional.h"
#include"iterator.h"
#include"memory.h"
#include"type_traits.h"
#include"exceptiondef.h"

namespace mystl
{
	//rb_tree�ڵ���ɫ����
	typedef bool rb_tree_color_type;
	static constexpr rb_tree_color_type rb_tree_red = false;
	static constexpr rb_tree_color_type rb_tree_black = true;

	//forward declaration
	template<class T> struct rb_tree_node_base;
	template<class T> struct rb_tree_node;
	template<class T> struct rb_tree_iterator;
	template<class T> struct rb_tree_const_iterator;

	//rb tree value traits
	template<class T,bool>
	struct rb_tree_value_traits_imp
	{
		typedef T key_type;
		typedef T mapped_type;
		typedef T value_type;

		template<class Ty>
		static const key_type& get_key(const Ty& value)
		{
			return value;
		}

		template<class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value;
		}
	};

	template<class T>
	struct rb_tree_value_traits_imp<T, true>
	{
		typedef typename std::remove_cv<typename T::first_type>::type key_type;
		//remove_cv����ȥ���������η��е�const volatile�޶�
		typedef typename T::second mapped_type;
		typedef T value_type;

		template<class Ty>
		static const key_type& get_key(const Ty& value)
		{
			return value.first;
		}

		template<class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value;
		}
	};

	template<class T>
	struct rb_tree_value_traits
	{
		static constexpr bool is_map = mystl::is_pair<T>::value;

		typedef rb_tree_value_traits_imp<T, is_map> value_traits_type;

		typedef typename value_traits_type::key_type key_type;
		typedef typename value_traits_type::mapped_type mapped_type;
		typedef typename value_traits_type::value_type value_type;

		template<class Ty>
		static const key_type& get_key(const Ty& value)
		{
			return value_traits_type::get_key(value);
		}

		template<class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value_traits_type::get_value(value);
		}
	};

	//rb tree node traits
	template<class T>
	struct rb_tree_node_traits
	{
		typedef rb_tree_color_type color_type;
		
		typedef rb_tree_value_traits<T> value_traits;
		typedef typename value_traits::key_type key_type;
		typedef typename value_traits::mapped_type mapped_type;
		typedef typename value_traits::value_type value_type;

		typedef rb_tree_node_base<T>* base_ptr;
		typedef rb_tree_node<T>* node_ptr;
	};

	template<class T>
	struct rb_tree_node_base
	{
		typedef rb_tree_color_type color_type;
		typedef rb_tree_node_base<T>* base_ptr;
		typedef rb_tree_node<T>* node_ptr;

		base_ptr parent;//���ڵ�
		base_ptr left;//���ӽڵ�
		base_ptr right;//���ӽڵ�
		color_type color;//��ɫ

		base_ptr get_base_ptr()
		{
			return &*this;
		}

		node_ptr get_node_ptr()
		{
			return reinterpret_cast<node_ptr>(&*this);
		}

		node_ptr& get_node_ref()
		{
			return reinterpret_cast<node_ptr&>(*this);
		}
	};

	template<class T>
	struct rb_tree_node:public rb_tree_node_base<T>
	{
		typedef rb_tree_node_base<T>* base_ptr;
		typedef rb_tree_node<T>* node_ptr;

		T value;

		base_ptr get_base_ptr()
		{
			return static_cast<base_ptr>(&*this);
		}

		node_ptr get_node_ptr()
		{
			return &*this;
		}
	};

	//rb tree traits
	template<class T>
	struct rb_tree_traits
	{
		typedef rb_tree_value_traits<T> value_traits;

		typedef typename value_traits::key_type key_type;
		typedef typename value_traits::mapped_type mapped_type;
		typedef typename value_traits::value_type value_type;

		typedef value_type* ptr;
		typedef value_type& ref;
		typedef const value_type* const_ptr;
		typedef const value_type& const_ref;

		typedef rb_tree_node_base<T> base_type;
		typedef rb_tree_node<T> node_type;

		typedef base_type* base_ptr;
		typedef node_type* node_ptr;
	};

	//rb_tree�ĵ�����
	template<class T>
	struct rb_tree_iterator_base : public mystl::iterator<mystl::bidirectional_iterator_tag, T>
	{
		typedef typename rb_tree_traits<T>::base_ptr base_ptr;
		
		base_ptr node;//�ڵ�����

		rb_tree_iterator_base() :node(nullptr) {};

		//������ǰ��
		void inc()
		{
			if (node->right!=nullptr)
			{
				node = rb_tree_min(node->right);
			}
			else
			{
				auto fa = node->parent;
				while (node == fa->right)
				{
					node = fa;
					fa = node->parent;
				}
				if (node->right != fa)
					node = fa;
			}
		}

		//����������
		void dec()
		{
			if (node->parent->parent == node && rb_tree_is_red(node))
			{//nodeΪheader
				node = node->right;
			}
			else if (node->left!=nullptr)
			{
				node = rb_tree_max(node->left);
			}
			else
			{
				auto fa = node->parent;
				while (node == fa->left)
				{
					node = fa;
					fa = node->parent;
				}
				node = fa;
			}
		}

		bool operator==(const rb_tree_iterator_base& rhs) { return node == rhs.node; }
		bool operator!=(const rb_tree_iterator_base& rhs) { return node != rhs.node; }
	};

	template<class T>
	struct rb_tree_iterator :public rb_tree_iterator_base<T>
	{
		typedef rb_tree_traits<T> tree_traits;

		typedef typename tree_traits::value_type value_type;
		typedef typename tree_traits::ptr ptr;
		typedef typename tree_traits::ref ref;
		typedef typename tree_traits::base_ptr base_ptr;
		typedef typename tree_traits::node_ptr node_ptr;

		typedef rb_tree_iterator<T> iterator;
		typedef rb_tree_const_iterator<T> const_iterator;
		typedef iterator self;

		using rb_tree_iterator_base<T>::node;

		rb_tree_iterator(){}
		rb_tree_iterator(base_ptr p) { node = p; }
		rb_tree_iterator(node_ptr p) { node = p; }
		rb_tree_iterator(const iterator& rhs) { node = rhs.node; }
		rb_tree_iterator(const const_iterator& rhs) { node = rhs.node; }

		ref operator*() const { return node->get_node_ptr()->value; }
		ptr operator->()const { return &(operator*()); }

		self& operator++()
		{
			this->inc();
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			this->inc();
			return tmp;
		}

		self& operator--()
		{
			this->dec();
			return *this;
		}

		self operator--(int)
		{
			self tmp(*this);
			this->dec();
			return tmp;
		}
	};

	template<class T>
	struct rb_tree_const_iterator :public rb_tree_iterator_base<T>
	{
		typedef rb_tree_traits<T> tree_traits;

		typedef typename tree_traits::value_type value_type;
		typedef typename tree_traits::const_ptr ptr;
		typedef typename tree_traits::const_ref ref;
		typedef typename tree_traits::base_ptr base_ptr;
		typedef typename tree_traits::node_ptr node_ptr;

		typedef rb_tree_iterator<T> iterator;
		typedef rb_tree_const_iterator<T> const_iterator;
		typedef const_iterator self;

		using rb_tree_iterator_base<T>::node;

		rb_tree_const_iterator() {}
		rb_tree_const_iterator(base_ptr p) { node = p; }
		rb_tree_const_iterator(node_ptr p) { node = p; }
		rb_tree_const_iterator(const iterator& rhs) { node=rhs.node; }
		rb_tree_const_iterator(const const_iterator& rhs) {	node = rhs.node;}

		ref operator*() const { return node->get_node_ptr()->value; }
		ptr operator->()const { return &(operator*()); }

		self& operator++()
		{
			this->inc();
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			this->inc();
			return tmp;
		}

		self& operator--()
		{
			this->dec();
			return *this;
		}

		self operator--(int)
		{
			self tmp(*this);
			this->dec();
			return tmp;
		}
	};

	//tree algorithm
	template<class NodePtr>
	NodePtr rb_tree_min(NodePtr p)noexcept
	{
		while (p->left)
			p = p->left;
		return p;
	}

	template<class NodePtr>
	NodePtr rb_tree_max(NodePtr p)noexcept
	{
		while (p->right)
			p = p->right;
		return p;
	}

	template<class NodePtr>
	bool rb_tree_is_lchild(NodePtr node)noexcept
	{
		return node == node->parent->left;
	}

	template<class NodePtr>
	bool rb_tree_is_rchild(NodePtr node)noexcept
	{
		return node == node->parent->right;
	}

	template<class NodePtr>
	bool rb_tree_is_red(NodePtr node)noexcept
	{
		return node->color == rb_tree_red;
	}

	template<class NodePtr>
	bool rb_tree_is_black(NodePtr node)noexcept
	{
		return node->color == rb_tree_black;
	}

	template<class NodePtr>
	void rb_tree_set_red(NodePtr node)noexcept
	{
		node->color = rb_tree_red;
	}

	template<class NodePtr>
	void rb_tree_set_black(NodePtr node)noexcept
	{
		node->color = rb_tree_black;
	}

	template<class NodePtr>
	NodePtr rb_tree_next(NodePtr node)noexcept
	{
		if (node->right!=nullptr)
			return rb_tree_min(node->right);
		while (rb_tree_is_rchild(node))
			node = node->parent;
		return node->parent;
	}

	/*---------------------------------------*\
	|       p                         p       |
	|      / \                       / \      |
	|     x   d    rotate left      y   d     |
	|    / \       ===========>    / \        |
	|   a   y                     x   c       |
	|      / \                   / \          |
	|     b   c                 a   b         |
	\*---------------------------------------*/
	// ����������һΪ�����㣬������Ϊ���ڵ�
	template<class NodePtr>
	void rb_tree_rotate_left(NodePtr x, NodePtr& root)noexcept
	{
		auto y = x->right;
		x->right = y->left;
		if (y->left != nullptr)
			y->left->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (rb_tree_is_lchild(x))
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	/*----------------------------------------*\
	|     p                         p          |
	|    / \                       / \         |
	|   d   x      rotate right   d   y        |
	|      / \     ===========>      / \       |
	|     y   a                     b   x      |
	|    / \                           / \     |
	|   b   c                         c   a    |
	\*----------------------------------------*/
	// ����������һΪ�����㣬������Ϊ���ڵ�
	template<class NodePtr>
	void rb_tree_rotate_right(NodePtr x, NodePtr& root)noexcept
	{
		auto y = x->left;
		x->left = y->right;
		if (y->right != nullptr)
			y->right->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (rb_tree_is_lchild(x))
			x->parent->left = y;
		else
			x->parent->right = y;
		y->right = x;
		x->parent = y;
	}


	// ����ڵ��ʹ rb tree ����ƽ�⣬����һΪ�����ڵ㣬������Ϊ���ڵ�
	//
	// case 1: �����ڵ�λ�ڸ��ڵ㣬�������ڵ�Ϊ��
	// case 2: �����ڵ�ĸ��ڵ�Ϊ�ڣ�û���ƻ�ƽ�⣬ֱ�ӷ���
	// case 3: ���ڵ������ڵ㶼Ϊ�죬��ڵ������ڵ�Ϊ�ڣ��游�ڵ�Ϊ�죬
	//         Ȼ�����游�ڵ�Ϊ��ǰ�ڵ㣬��������
	// case 4: ���ڵ�Ϊ�죬����ڵ�Ϊ NIL ���ɫ�����ڵ�Ϊ���ң����ӣ���ǰ�ڵ�Ϊ�ң��󣩺��ӣ�
	//         �ø��ڵ��Ϊ��ǰ�ڵ㣬���Ե�ǰ�ڵ�Ϊ֧�����ң���
	// case 5: ���ڵ�Ϊ�죬����ڵ�Ϊ NIL ���ɫ�����ڵ�Ϊ���ң����ӣ���ǰ�ڵ�Ϊ���ң����ӣ�
	//         �ø��ڵ��Ϊ��ɫ���游�ڵ��Ϊ��ɫ�����游�ڵ�Ϊ֧���ң�����
	//
	// �ο�����: http://blog.csdn.net/v_JULY_v/article/details/6105630
	//           http://blog.csdn.net/v_JULY_v/article/details/6109153
	template<class NodePtr>
	void rb_tree_insert_rebalance(NodePtr x, NodePtr& root)noexcept
	{
		rb_tree_set_red(x);//���������ڵ�Ϊ��ɫ
		while (x != root && rb_tree_is_red(x->parent))
		{
			if (rb_tree_is_lchild(x->parent))//������ڵ�Ϊ���ӽڵ�
			{
				auto uncle = x->parent->parent->right;
				if (uncle != nullptr &&rb_tree_is_red(uncle))
				{//case 3:uncle��parent��Ϊ��ɫ
					rb_tree_set_black(uncle);
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					x = x->parent->parent;
				}
				else
				{
					if (rb_tree_is_rchild(x))
					{//case 4:parent��child��
						x = x->parent;
						rb_tree_rotate_left(x, root);
					}
					//��ת��Ϊcase 5:
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_right(x->parent->parent, root);
					break;
				}
			}
			else
			{
				auto uncle = x->parent->parent->left;
				if (uncle != nullptr && rb_tree_is_red(uncle))
				{//case 3
					rb_tree_set_black(uncle);
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					x = x->parent->parent;
				}
				else
				{
					if (rb_tree_is_lchild(x))
					{//case 4 
						x = x->parent;
						rb_tree_rotate_right(x, root);
					}
					//case 5
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_left(x->parent->parent, root);
					break;
				}
			}
		}
		rb_tree_set_black(root);
	}

	// ɾ���ڵ��ʹ rb tree ����ƽ�⣬����һΪҪɾ���Ľڵ㣬������Ϊ���ڵ㣬������Ϊ��С�ڵ㣬������Ϊ���ڵ�
	// 
	// �ο�����: http://blog.csdn.net/v_JULY_v/article/details/6105630
	//           http://blog.csdn.net/v_JULY_v/article/details/6109153
	template<class NodePtr>
	NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
	{
		//y������ܵ��滻�ڵ㣬ָ������ɾ���Ľڵ�
		auto y = (z->left==nullptr || z->right==nullptr) ? z : rb_tree_next(z);
		//xΪy�Ķ��ӻ���nil
		auto x = (y->left!=nullptr ? y->left : y->right);
		//xpΪx�ĸ��ڵ�
		NodePtr xp = nullptr;


		// y != z ˵�� z �������ǿ��ӽڵ㣬��ʱ y ָ�� z ������������ڵ㣬x ָ�� y �����ӽڵ㡣
		// �� y ���� z ��λ�ã��� x ���� y ��λ�ã������ y ָ�� z
		if (y != z)
		{
			z->left->parent = y;
			y->left = z->left;
			
			// ��� y ���� z �����ӽڵ㣬��ô z �����ӽڵ�һ��������
			if (y != z->right)
			{
				xp = y->parent;
				if (x!=nullptr)
					x->parent = y->parent;
				y->parent->left = x;
				y->right = z->right;
				z->right->parent = y;
			}
			else
			{
				xp = y;
			}

			//����y��z�ĸ��ڵ�
			if (root == z)
				root = y;
			else if (rb_tree_is_lchild(z))
				z->parent->left = y;
			else
				z->parent->right = y;
			y->parent = z->parent;
			mystl::swap(y->color, z->color);
			y = z;
		}
		else// y == z ˵�� z ����ֻ��һ������
		{
			xp = y->parent;
			if (x)
				x->parent = y->parent;
			if (root == z)
				root = x;
			else if (rb_tree_is_lchild(z))
				z->parent->left = x;
			else
				z->parent->right = x;
			if (leftmost == z)
				leftmost = (x == nullptr ? xp : rb_tree_min(x));
			if (rightmost == z)
				rightmost = (x == nullptr ? xp : rb_tree_max(x));
		}

		// ��ʱ��y ָ��Ҫɾ���Ľڵ㣬x Ϊ����ڵ㣬�� x �ڵ㿪ʼ������
		// ���ɾ���Ľڵ�Ϊ��ɫ����������û�б��ƻ����������������������x Ϊ���ӽڵ�Ϊ������
		// case 1: �ֵܽڵ�Ϊ��ɫ����ڵ�Ϊ�죬�ֵܽڵ�Ϊ�ڣ�Χ�Ƹ��ڵ�������ң�������������
		// case 2: �ֵܽڵ�Ϊ��ɫ���������ӽڵ㶼Ϊ��ɫ�� NIL�����ֵܽڵ�Ϊ�죬���ڵ��Ϊ��ǰ�ڵ㣬��������
		// case 3: �ֵܽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ�� NIL�����ӽڵ�Ϊ��ɫ�� NIL��
		//         ���ֵܽڵ�Ϊ�죬�ֵܽڵ�����ӽڵ�Ϊ�ڣ����ֵܽڵ�Ϊ֧���ң���������������
		// case 4: �ֵܽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ�����ֵܽڵ�Ϊ���ڵ����ɫ�����ڵ�Ϊ��ɫ���ֵܽڵ�����ӽڵ�
		//         Ϊ��ɫ���Ը��ڵ�Ϊ֧�����ң������������ʵ�����ɣ��㷨����
		if (rb_tree_is_black(y))
		{
			while (x != root && (x == nullptr || rb_tree_is_black(x)))
			{
				if (x == xp->left)
				{
					auto brother = xp->right;
					if (rb_tree_is_red(brother))
					{//case 1
						rb_tree_set_black(brother);
						rb_tree_set_red(xp);
						rb_tree_rotate_left(xp, root);
						brother = xp->right;
					}
					if ((brother->left == nullptr || rb_tree_is_black(brother->left)) &&
						(brother->right == nullptr || rb_tree_is_black(brother->right)))
					{//case 2
						rb_tree_set_red(brother);
						x = xp;
						xp = xp->parent;
					}
					else
					{
						if (brother->right == nullptr || rb_tree_is_black(brother->right))
						{//case 3
							if (brother->left != nullptr)
								rb_tree_set_black(brother->right);
							rb_tree_set_red(brother);
							rb_tree_rotate_right(brother, root);
							brother = xp->right;
						}
						//case 4
						brother->color = xp->color;
						rb_tree_set_black(xp);
						if (brother->right != nullptr)
							rb_tree_set_black(brother->right);
						rb_tree_rotate_left(xp, root);
						break;
					}
				}
				else
				{
					auto brother = xp->left;
					if (rb_tree_is_red(brother))
					{ // case 1
						rb_tree_set_black(brother);
						rb_tree_set_red(xp);
						rb_tree_rotate_right(xp, root);
						brother = xp->left;
					}
					if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
						(brother->right == nullptr || !rb_tree_is_red(brother->right)))
					{ // case 2
						rb_tree_set_red(brother);
						x = xp;
						xp = xp->parent;
					}
					else
					{
						if (brother->left == nullptr || !rb_tree_is_red(brother->left))
						{ // case 3
							if (brother->right != nullptr)
								rb_tree_set_black(brother->right);
							rb_tree_set_red(brother);
							rb_tree_rotate_left(brother, root);
							brother = xp->left;
						}
						// תΪ case 4
						brother->color = xp->color;
						rb_tree_set_black(xp);
						if (brother->left != nullptr)
							rb_tree_set_black(brother->left);
						rb_tree_rotate_right(xp, root);
						break;
					}
				}
			}
			if (x != nullptr)
				rb_tree_set_black(x);
		}
		return y;
	}

	//ģ����rb_tree
	//����һ�����������ͣ�������������ֵ�Ƚ�����
	template<class T,class Compare>
	class rb_tree
	{
	public:
		typedef rb_tree_traits<T>                        tree_traits;
		typedef rb_tree_value_traits<T>                  value_traits;

		typedef typename tree_traits::base_type          base_type;
		typedef typename tree_traits::base_ptr			 base_ptr;
		typedef typename tree_traits::node_type          node_type;
		typedef typename tree_traits::node_ptr           node_ptr;
		typedef typename tree_traits::key_type           key_type;
		typedef typename tree_traits::mapped_type        mapped_type;
		typedef typename tree_traits::value_type         value_type;
		typedef Compare                                  key_compare;

		typedef mystl::allocator<T>                      allocator_type;
		typedef mystl::allocator<T>                      data_allocator;
		typedef mystl::allocator<base_type>              base_allocator;
		typedef mystl::allocator<node_type>              node_allocator;

		typedef typename allocator_type::pointer         ptr;
		typedef typename allocator_type::const_pointer   const_ptr;
		typedef typename allocator_type::reference       ref;
		typedef typename allocator_type::const_reference const_ref;
		typedef typename allocator_type::size_type       size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef rb_tree_iterator<T>                      iterator;
		typedef rb_tree_const_iterator<T>                const_iterator;
		typedef mystl::reverse_iterator<iterator>        reverse_iterator;
		typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

		allocator_type get_allocator()const { return node_allocator(); }
		key_compare key_comp()const { return key_comp_; }

	private:
		base_ptr header_;//ͷ�ڵ㣬����ڵ㻥Ϊ���ڵ�
		size_type node_count_;//�ڵ�����
		key_compare key_comp_;//�Ƚ�׼��

		base_ptr& root()const { return header_->parent; }
		base_ptr& leftmost()const { return header_->left; }
		base_ptr& rightmost()const { return header_->right; }

	public:
		rb_tree() { rb_tree_init(); }
		rb_tree(const rb_tree& rhs);
		rb_tree(rb_tree&& rhs);
		
		rb_tree& operator=(const rb_tree& rhs);
		rb_tree& operator=(rb_tree&& rhs);

		~rb_tree() { clear(); }

		//��������ز���
		iterator               begin()         noexcept
		{
			return leftmost();
		}
		const_iterator         begin()   const noexcept
		{
			return leftmost();
		}
		iterator               end()           noexcept
		{
			return header_;
		}
		const_iterator         end()     const noexcept
		{
			return header_;
		}

		reverse_iterator       rbegin()        noexcept
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin()  const noexcept
		{
			return const_reverse_iterator(end());
		}
		reverse_iterator       rend()          noexcept
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend()    const noexcept
		{
			return const_reverse_iterator(begin());
		}

		const_iterator         cbegin()  const noexcept
		{
			return begin();
		}
		const_iterator         cend()    const noexcept
		{
			return end();
		}
		const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}
		const_reverse_iterator crend()   const noexcept
		{
			return rend();
		}

		// ������ز���

		bool      empty()    const noexcept { return node_count_ == 0; }
		size_type size()     const noexcept { return node_count_; }
		size_type max_size() const noexcept { return static_cast<size_type>(-1); }

		// ����ɾ����ز���

		// emplace
		template<class ...Args>
		iterator emplace_multi(Args&& ...args);

		template<class ...Args>
		mystl::pair<iterator, bool> emplace_unique(Args&& ...args);

		template<class ...Args>
		iterator emplace_multi_use_hint(iterator hint, Args&& ...args);

		template<class ...Args>
		iterator emplace_unique_use_hint(iterator hint, Args&& ...args);

		// insert

		iterator  insert_multi(const value_type& value);
		iterator  insert_multi(value_type&& value)
		{
			return emplace_multi(mystl::move(value));
		}

		iterator  insert_multi(iterator hint, const value_type& value)
		{
			return emplace_multi_use_hint(hint, value);
		}
		iterator  insert_multi(iterator hint, value_type&& value)
		{
			return emplace_multi_use_hint(hint, mystl::move(value));
		}

		template<class InputIter>
		void insert_multi(InputIter first, InputIter last)
		{
			size_type n = mystl::distance(first, last);
			THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
			for (; n > 0; --n, ++first)
				insert_multi(end(), *first);
		}

		mystl::pair<iterator, bool> insert_unique(const value_type& value);
		mystl::pair<iterator, bool> insert_unique(value_type&& value)
		{
			return emplace_unique(mystl::move(value));
		}

		iterator insert_unique(iterator hint, const value_type& value)
		{
			return emplace_unique_use_hint(hint, value);
		}
		iterator insert_unique(iterator hint, value_type&& value)
		{
			return emplace_unique_use_hint(hint, mystl::move(value));
		}

		template<class InputIter>
		void insert_unique(InputIter first, InputIter last)
		{
			size_type n = mystl::distance(first, last);
			THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
			for (; n > 0; --n, ++first)
				insert_unique(end(), *first);
		}

		//erase
		iterator erase(iterator hint);
		size_type erase_multi(const key_type& key);
		size_type erase_unique(const key_type& key);

		void erase(iterator first, iterator last);

		void clear();

		//rb_tree ��ز���
		iterator find(const key_type& key);
		const_iterator find(const key_type& key)const;

		size_type count_multi(const key_type& key)const
		{
			auto p = equal_range_multi(key);
			return static_cast<size_type>(mystl::distance(p.first, p.second));
		}

		size_type count_unique(const key_type& key)const
		{
			return find(key)!=end()?1:0;
		}

		iterator lower_bound(const key_type& key);
		const_iterator lower_bound(const key_type& key)const;

		iterator       upper_bound(const key_type& key);
		const_iterator upper_bound(const key_type& key) const;

		mystl::pair<iterator,iterator> equal_range_multi(const key_type& key)
		{
			return mystl::make_pair<iterator, iterator>(lower_bound(key), upper_bound(key));
		}

		mystl::pair<const_iterator, const_iterator> equal_range_multi(const key_type& key)const
		{
			return mystl::make_pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
		}

		mystl::pair<iterator, iterator> equal_range_unique(const key_type& key)
		{
			auto it = find(key);
			auto next = it;
			return it == end() ? make_pair(it, it) : make_pair(it, ++next);
		}
		
		mystl::pair<const_iterator, const_iterator> equal_range_unique(const key_type& key)const
		{
			auto it = find(key);
			auto next = it;
			return it == end() ? make_pair<const_iterator, const_iterator>(it, it) 
				: make_pair<const_iterator, const_iterator>(it, ++next);
		}

		void swap(rb_tree& rhs)noexcept;

	private:
		//node related
		template<class ...Args> 
		node_ptr create_node(Args&&... args);
		node_ptr clone_node(base_ptr x);
		void destroy_node(node_ptr p);


		//init/reset
		void rb_tree_init();
		void reset();

		//get insert pos
		mystl::pair<base_ptr, bool> get_insert_multi_pos(const key_type& key);
		mystl::pair<mystl::pair<base_ptr, bool>,bool> get_insert_unique_pos(const key_type& key);

		// insert value / insert node
		iterator insert_value_at(base_ptr x, const value_type& value, bool add_to_left);
		iterator insert_node_at(base_ptr x, node_ptr node, bool add_to_left);

		// insert use hint
		iterator insert_multi_use_hint(iterator hint, key_type key, node_ptr node);
		iterator insert_unique_use_hint(iterator hint, key_type key, node_ptr node);

		// copy tree / erase tree
		base_ptr copy_from(base_ptr x, base_ptr p);
		void     erase_since(base_ptr x);
	};


	/************************************************************************************/

	template<class T, class Compare>
	inline rb_tree<T, Compare>::rb_tree(const rb_tree& rhs)
	{
		rb_tree_init();
		if (rhs.node_count_ != 0)
		{
			root() = copy_from(rhs.root(), header_);
			leftmost() = rb_tree_min(root());
			rightmost() = rb_tree_max(root());
		}
		node_count_ = rhs.node_count_;
		key_comp_ = rhs.key_comp_;
	}
	template<class T, class Compare>
	inline rb_tree<T, Compare>::rb_tree(rb_tree&& rhs):
		header_(mystl::move(rhs.header_)),
		node_count_(mystl::move(rhs.node_count_)),
		key_comp_(mystl::move(rhs.key_comp_))
	{
		rhs.reset();
	}
	template<class T, class Compare>
	inline rb_tree<T, Compare>& rb_tree<T, Compare>::operator=(const rb_tree& rhs)
	{
		if (this != &rhs)
		{
			clear();
			if (rhs.node_count_ != 0)
			{
				root() = copy_from(rhs.root(), header_);
				leftmost() = rb_tree_min(root());
				rightmost() = rb_tree_max(root());
			}
			node_count_ = rhs.node_count_;
			key_comp_ = rhs.key_comp_;
		}
		return *this;
	}
	template<class T, class Compare>
	inline rb_tree<T, Compare>& rb_tree<T, Compare>::operator=(rb_tree&& rhs)
	{
		clear();
		header_ = mystl::move(rhs.header_);
		node_count_ = mystl::move(rhs.node_count_);
		key_comp_ = mystl::move(rhs.key_comp_);
		rhs.reset();
		return *this;
	}

	// �͵ز���Ԫ�أ���ֵ�����ظ�
	template<class T, class Compare>
	template<class ...Args>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::emplace_multi(Args && ...args)
	{
		THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
		node_ptr np = create_node(mystl::forward<Args>(args)...);
		auto res = get_insert_multi_pos(value_traits::get_key(np->value));
		return insert_node_at(res.first, np, res.second);
	}

	// �͵ز���Ԫ�أ���ֵ�������ظ�
	template<class T, class Compare>
	template<class ...Args>
	inline mystl::pair<typename rb_tree<T, Compare>::iterator, bool> rb_tree<T, Compare>::emplace_unique(Args && ...args)
	{
		THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
		node_ptr np = create_node(mystl::forward<Args>(args)...);
		auto res = get_insert_unique_pos(value_traits::get_key(np->value));
		if (res.second)
		{
			return mystl::make_pair(insert_node_at(res.first.first, np, res.first.second), true);
		}
		destroy_node(np);
		return mystl::make_pair(iterator(res.first.first), false);
	}

	template<class T, class Compare>
	template<class ...Args>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::emplace_multi_use_hint(iterator hint, Args && ...args)
	{
		THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
		node_ptr np = create_node(mystl::forward<Args>(args)...);
		if (node_count_ == 0)
		{
			return insert_node_at(header_, np, true);
		}
		key_type key = value_traits::get_key(np->value);
		if (hint == begin())
		{
			if (key_comp_(key, value_traits::get_key(*hint)))
			{
				return insert_node_at(hint.node, np, true);
			}
			else
			{
				auto pos = get_insert_multi_pos(key);
				return insert_node_at(pos.first, np, pos.second);
			}
		}
		else if (hint == end())
		{
			if (!key_comp_(key, value_traits::get_key(rightmost()->get_node_ptr()->value)))
			{
				return insert_node_at(hint.node, np, false);
			}
			else
			{
				auto pos = get_insert_multi_pos(key);
				return insert_node_at(pos.first, np, pos.second);
			}
		}
		return insert_multi_use_hint(hint, key, np);
	}

	template<class T, class Compare>
	template<class ...Args>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::emplace_unique_use_hint(iterator hint, Args && ...args)
	{
		THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
		node_ptr np = create_node(mystl::forward<Args>(args)...);
		if (node_count_ == 0)
		{
			return insert_node_at(header_, np, true);
		}
		key_type key = value_traits::get_key(np->value);
		if (hint == begin())
		{ // λ�� begin ��
			if (key_comp_(key, value_traits::get_key(*hint)))
			{
				return insert_node_at(hint.node, np, true);
			}
			else
			{
				auto pos = get_insert_unique_pos(key);
				if (!pos.second)
				{
					destroy_node(np);
					return pos.first.first;
				}
				return insert_node_at(pos.first.first, np, pos.first.second);
			}
		}
		else if (hint == end())
		{ // λ�� end ��
			if (key_comp_(value_traits::get_key(rightmost()->get_node_ptr()->value), key))
			{
				return insert_node_at(rightmost(), np, false);
			}
			else
			{
				auto pos = get_insert_unique_pos(key);
				if (!pos.second)
				{
					destroy_node(np);
					return pos.first.first;
				}
				return insert_node_at(pos.first.first, np, pos.first.second);
			}
		}
		return insert_unique_use_hint(hint, key, np);
	}

	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_multi(const value_type& value)
	{
		THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
		auto res = get_insert_multi_pos(value_traits::get_key(value));
		return insert_value_at(res.first, value, res.second);
	}
	template<class T, class Compare>
	inline mystl::pair<typename rb_tree<T, Compare>::iterator, bool> rb_tree<T, Compare>::insert_unique(const value_type& value)
	{
		THROW_LENGTH_ERROR_IF(node_count_ > max_size() - 1, "rb_tree<T, Comp>'s size too big");
		auto res = get_insert_unique_pos(value_traits::get_key(value));
		if (res.second)
		{ // ����ɹ�
			return mystl::make_pair(insert_value_at(res.first.first, value, res.first.second), true);
		}
		return mystl::make_pair(res.first.first, false);
	}

	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::erase(iterator hint)
	{
		auto node = hint.node->get_node_ptr();
		iterator next(node);
		++next;

		rb_tree_erase_rebalance(hint.node, root(), leftmost(), rightmost());
		destroy_node(node);
		--node_count_;
		return next;
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::size_type rb_tree<T, Compare>::erase_multi(const key_type& key)
	{
		auto p = equal_range_multi(key);
		size_type n = mystl::distance(p.first, p.second);
		erase(p.first, p.second);
		return n;
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::size_type rb_tree<T, Compare>::erase_unique(const key_type& key)
	{
		auto it = find(key);
		if (it != end())
		{
			erase(it);
			return 1;
		}
		return 0;
	}

	template<class T, class Compare>
	inline void rb_tree<T, Compare>::erase(iterator first, iterator last)
	{
		if (first == begin() && last == end())
			clear();
		else
		{
			while (first != last)
			{
				erase(first++);
			}
		}
	}

	template<class T, class Compare>
	inline void rb_tree<T, Compare>::clear()
	{
		if (node_count_ != 0)
		{
			erase_since(root());
			header_->left = header_;
			header_->parent = nullptr;
			header_->right = header_;
			node_count_ = 0;
		}
	}
	
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::find(const key_type& key)
	{
		auto y = header_;  // ���һ����С�� key �Ľڵ�
		auto x = root();
		while (x != nullptr)
		{
			if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
			{ // key С�ڵ��� x ��ֵ��������
				y = x, x = x->left;
			}
			else
			{ // key ���� x ��ֵ��������
				x = x->right;
			}
		}
		iterator j = iterator(y);
		return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::const_iterator rb_tree<T, Compare>::find(const key_type& key) const
	{
		auto y = header_;  // ���һ����С�� key �Ľڵ�
		auto x = root();
		while (x != nullptr)
		{
			if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
			{ // key С�ڵ��� x ��ֵ��������
				y = x;
				x = x->left;
			}
			else
			{ // key ���� x ��ֵ��������
				x = x->right;
			}
		}
		const_iterator j = const_iterator(y);
		return (j == end() || key_comp_(key, value_traits::get_key(*j))) ? end() : j;
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::lower_bound(const key_type& key)
	{
		auto y = header_;
		auto x = root();
		while (x != nullptr)
		{
			if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
			{ // key <= x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return iterator(y);
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::const_iterator rb_tree<T, Compare>::lower_bound(const key_type& key) const
	{
		auto y = header_;
		auto x = root();
		while (x != nullptr)
		{
			if (!key_comp_(value_traits::get_key(x->get_node_ptr()->value), key))
			{ // key <= x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return const_iterator(y);
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::upper_bound(const key_type& key)
	{
		auto y = header_;
		auto x = root();
		while (x != nullptr)
		{
			if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value)))
			{ // key < x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return iterator(y);
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::const_iterator rb_tree<T, Compare>::upper_bound(const key_type& key) const
	{
		auto y = header_;
		auto x = root();
		while (x != nullptr)
		{
			if (key_comp_(key, value_traits::get_key(x->get_node_ptr()->value)))
			{ // key < x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return const_iterator(y);
	}
	template<class T, class Compare>
	inline void rb_tree<T, Compare>::swap(rb_tree& rhs) noexcept
	{
		if (this != &rhs)
		{
			mystl::swap(header_, rhs.header_);
			mystl::swap(key_comp_, rhs.key_comp_);
			mystl::swap(node_count_, rhs.node_count_);
		}
	}

	//helper function

	template<class T, class Compare>
	template<class ...Args>
	inline typename rb_tree<T, Compare>::node_ptr rb_tree<T, Compare>::create_node(Args && ...args)
	{
		auto tmp = node_allocator::allocate(1);
		try
		{
			data_allocator::construct(mystl::address_of(tmp->value),mystl::forward<Args>(args)...);
			tmp->left = tmp->right = tmp->parent = nullptr;
		}
		catch (...)
		{
			node_allocator::deallocate(tmp);
			throw;
		}
		return tmp;
	}

	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::node_ptr rb_tree<T, Compare>::clone_node(base_ptr x)
	{
		node_ptr tmp = create_node(x->get_node_ptr()->value);
		tmp->color = x->color;
		tmp->left = tmp->right = nullptr;
		return tmp;
	}
	template<class T, class Compare>
	inline void rb_tree<T, Compare>::destroy_node(node_ptr p)
	{
		data_allocator::destroy(&p->value);
		node_allocator::deallocate(p);
	}
	template<class T, class Compare>
	inline void rb_tree<T, Compare>::rb_tree_init()
	{
		header_ = base_allocator::allocate(1);
		header_->color = rb_tree_red;
		root() = nullptr;
		leftmost() = header_;
		rightmost() = header_;
		node_count_ = 0;
	}

	template<class T, class Compare>
	inline void rb_tree<T, Compare>::reset()
	{
		header_ = nullptr;
		node_count_ = 0;
	}

	template<class T, class Compare>
	inline mystl::pair<typename rb_tree<T, Compare>::base_ptr, bool> rb_tree<T, Compare>::get_insert_multi_pos(const key_type& key)
	{
		auto x = root();
		auto y = header_;
		bool add_to_left = true;
		while (x != nullptr)
		{
			y = x;
			add_to_left = key_comp_(key, value_traits::get_key(x->get_node_ptr()->value));
			x = (add_to_left ? x->left : x->right);
		}
		return mystl::make_pair(y, add_to_left);
	}

	// ����һ�� pair����һ��ֵΪһ�� pair�����������ĸ��ڵ��һ�� bool ��ʾ�Ƿ�����߲��룬
	// �ڶ���ֵΪһ�� bool����ʾ�Ƿ����ɹ�
	template<class T, class Compare>
	inline mystl::pair<mystl::pair<typename rb_tree<T, Compare>::base_ptr, bool>,bool> rb_tree<T, Compare>::get_insert_unique_pos(const key_type& key)
	{
		auto x = root();
		auto y = header_;
		bool add_to_left = true;  // ��Ϊ��ʱҲ�� header_ ��߲���
		while (x != nullptr)
		{
			y = x;
			add_to_left = key_comp_(key, value_traits::get_key(x->get_node_ptr()->value));
			x = (add_to_left ? x->left : x->right);
		}
		iterator j = iterator(y);// ��ʱ y Ϊ�����ĸ��ڵ�
		if (add_to_left)
		{
			if (y == header_ || j == begin())
			{ // �����Ϊ����������������ڵ㴦���϶����Բ����µĽڵ�
				return mystl::make_pair(mystl::make_pair(y, true), true);
			}
			else
			{ // ������������ظ��ڵ㣬��ô --j �����ظ���ֵ
				--j;
			}
		}
		if (key_comp_(value_traits::get_key(*j), key))
		{
			return mystl::make_pair(mystl::make_pair(y, add_to_left), true);
		}
		return mystl::make_pair(mystl::make_pair(y, add_to_left), false);
	}

	// insert_value_at ����
	// x Ϊ�����ĸ��ڵ㣬 value ΪҪ�����ֵ��add_to_left ��ʾ�Ƿ�����߲���
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_value_at(base_ptr x, const value_type& value, bool add_to_left)
	{
		node_ptr node = create_node(value);
		node->parent = x;
		auto base_node = node->get_base_ptr();
		if (x == header_)
		{
			root() = leftmost() = rightmost() = base_node;
		}
		else if (add_to_left)
		{
			x->left = base_node;
			if (leftmost() == x)
				leftmost() = base_node;
		}
		else
		{
			x->right = base_node;
			if (rightmost() == x)
				rightmost() = base_node;
		}
		rb_tree_insert_rebalance(base_node, root());
		++node_count_;
		return iterator(node);
	}

	// �� x �ڵ㴦�����µĽڵ�
	// x Ϊ�����ĸ��ڵ㣬 node ΪҪ����Ľڵ㣬add_to_left ��ʾ�Ƿ�����߲���
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_node_at(base_ptr x, node_ptr node, bool add_to_left)
	{
		node->parent = x;
		auto base_node = node->get_base_ptr();
		if (x == header_)
		{
			root() = leftmost() = rightmost() = base_node;
		}
		else if (add_to_left)
		{
			x->left = base_node;
			if (leftmost() == x)
				leftmost() = base_node;
		}
		else
		{
			x->right = base_node;
			if (rightmost() == x)
				rightmost() = base_node;
		}
		rb_tree_insert_rebalance(base_node, root());
		++node_count_;
		return iterator(node);
	}

	// ����Ԫ�أ���ֵ�����ظ���ʹ�� hint
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_multi_use_hint(iterator hint, key_type key, node_ptr node)
	{
		// �� hint ����Ѱ�ҿɲ����λ��
		auto np = hint.node;
		auto before = hint;
		--before;
		auto bnp = before.node;
		if (!key_comp_(key, value_traits::get_key(*before)) && !key_comp_(value_traits::get_key(*hint), key))
		{//before<=node<=hint
			if (bnp->right == nullptr)
			{
				return insert_node_at(bnp, node, false);
			}
			else if (np->left == nullptr)
			{
				return insert_node_at(np, node, true);
			}
		}
		auto pos = get_insert_multi_pos(key);
		return insert_node_at(pos.first, node, pos.second);
	}

	// ����Ԫ�أ���ֵ�������ظ���ʹ�� hint
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_unique_use_hint(iterator hint, key_type key, node_ptr node)
	{
		auto np = hint.node;
		auto before = hint;
		--before;
		auto bnp = before.node;
		if (key_comp_(key, value_traits::get_key(*hint)) && key_comp_(value_traits::get_key(*before), key))
		{//before<node<hint
			if (bnp->right == nullptr)
			{
				return insert_node_at(bnp, node, false);
			}
			else if (np->left == nullptr)
			{
				return insert_node_at(np, node, true);
			}
		}
		auto pos = get_insert_unique_pos(key);
		if (!pos.second)
		{
			destroy_node(node);
			return pos.first.first;
		}
		return insert_node_at(pos.first.first, node, pos.first.second);
	}
	template<class T, class Compare>
	inline typename rb_tree<T, Compare>::base_ptr rb_tree<T, Compare>::copy_from(base_ptr x, base_ptr p)
	{
		auto top = clone_node(x);
		top->parent = p;
		try
		{
			if (x->right)
			{
				top->right = copy_from(x->right, top);
			}
			p = top;
			x = x->left;
			while (x)
			{
				auto y = clone_node(x);
				p->left = y;
				y->parent = p;
				if (x->right)
					y->right = copy_from(x->right, y);
				p = y;
				x = x->left;
			}
		}
		catch (...)
		{
			erase_since(top);
			throw;
		}
		return top;
	}

	// erase_since ����
	// �� x �ڵ㿪ʼɾ���ýڵ㼰������
	template<class T, class Compare>
	inline void rb_tree<T, Compare>::erase_since(base_ptr x)
	{
		while (x != nullptr)
		{
			erase_since(x->right);
			auto y = x->left;
			destroy_node(x->get_node_ptr());
			x = y;
		}
	}

	// ���رȽϲ�����
	template <class T, class Compare>
	bool operator==(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
	{
		return lhs.size() == rhs.size() && mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <class T, class Compare>
	bool operator<(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
	{
		return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Compare>
	bool operator!=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class T, class Compare>
	bool operator>(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
	{
		return rhs < lhs;
	}

	template <class T, class Compare>
	bool operator<=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class T, class Compare>
	bool operator>=(const rb_tree<T, Compare>& lhs, const rb_tree<T, Compare>& rhs)
	{
		return !(lhs < rhs);
	}

	// ���� mystl �� swap
	template <class T, class Compare>
	void swap(rb_tree<T, Compare>& lhs, rb_tree<T, Compare>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}

#endif