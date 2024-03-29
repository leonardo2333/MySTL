# MySTL

## 容器

vector、list、deque、map、set、multimap、multiset等

## 算法

排序、复制、赋值等

## 迭代器

对节点指针的封装，用于遍历容器里的对象

## 函数对象（仿函数）

用于参数比较

## 适配器

对于某些容器，没有指定具体的实现的数据结构。如优先队列，可以使用vector或红黑树进行实现，可以从大到小或从小到大排列。该特性基于适配器实现

## 内存分配器

用于内存分配

## 底层数据结构

### 数组（array、vector、priority_queue）

特点：O(1)时间复杂度的随机访问，在指定位置插入和删除元素时间复杂度为O(n)

扩容规则：在内存空间已满的情况下，向vector插入元素时，会申请一个2倍大的内存空间，将原先内存空间中的所有元素复制到新的内存空间中，并释放原先的内存空间，**此时原先的迭代器会失效**。同理，**在插入和删除元素后，在该元素之后的迭代器会失效**。

### 红黑树（map、set、multimap、multiset、priority_queue）

特点：插入、查找均为$O(log_2n)$的时间复杂度

红黑树本质上是一棵**二叉查找树**，它遵循以下特性：

* 若任意结点的左子树不空，则左子树上所有结点的值均小于它的根结点的值

* 若任意结点的右子树不空，则右子树上所有结点的值均大于它的根结点的值

* 任意结点的左、右子树也分别为二叉查找树

* ~~没有键值相等的结点~~（二叉搜索树的特性，multimap和multiset中允许相等）

在二叉搜索树的基础上，红黑树添加了以下特性：

* 每个结点为红色或者黑色

* 根和叶子结点为黑色，叶子结点为空值

* 红色结点的子结点为黑色

* 一个结点从不同路径到叶子结点的黑色结点个数相同

红黑树的**插入**：

1. 首先基于二叉查找树的性质找到要插入的位置，插入该节点并设为红色

2. 如果插入的为根节点，则将当前节点设为黑色，结束

3. 若插入节点的父节点为黑色，结束

4. 若父节点为红色：

    - 叔叔节点为红色：将父节点和叔叔节点设为黑色，祖父节点设为红色，当前节点设为祖父节点，继续

    - 叔叔节点为黑色：

        + 当前节点是父节点的右子节点：将父节点作为当前节点，以新的当前节点为支点左旋

        + 当前节点是父节点的左子节点：将父节点变为黑色、祖父节点变为红色，以祖父节点为支点右旋

5. 将根节点涂黑

二叉查找树的删除：

1. 待删除的节点为叶子节点，直接删除，结束

2. 只有一个儿子。将父节点的对应指针指向儿子节点，删除当前节点，结束

3. 有两个儿子。选择左子树上最大的节点放到待删除的位置，调整子树

红黑树的**删除**：

1. 从被删结点后来顶替它的那个结点开始调整，并认为它有额外的一重黑色

2. 当前节点为红+黑：将当前节点设为黑色，结束

3. 当前节点为黑+黑：

    - 当前节点为根节点：结束

    - 兄弟节点为红色：把父结点染成红色，把兄弟结点染成黑色，继续

    - 兄弟节点为黑色：
    
        + 其子节点为黑色：给父节点附加黑色，父节点为当前节点，继续

        + 其左子节点为红色，右子节点为黑色：兄弟节点设为红色，左子节点设为黑色，在兄弟节点右旋，继续

        + 其右子节点为红色：兄弟节点设为父节点的颜色，父节点设为黑色，兄弟的右子节点设为黑色，以父节点为支点左旋，结束

[参考链接](https://github.com/julycoding/The-Art-Of-Programming-By-July-2nd/blob/master/ebook/zh/03.01.md)

### 散列表（unordered_set、unordered_map）

### deque、stack、queue

deque 容器存储数据的空间是由一段一段等长的连续空间构成，各段空间之间并不一定是连续的，可以位于在内存的不同区域

![1677244823519](image/README/1677244823519.png)

deque中map数组存储着所有指向一段一段内存空间的指针，当map数组存满时，会申请一个更大的连续空间，将所有指针拷贝到新的map数组中（与vector一样申请2倍大的空间）