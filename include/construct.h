
// construct与destroy的模板函数
// 负责对象的构造以及析构
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

namespace MySTL {
#include <new> // 调用placement new

template <typename T1, typename T2>
inline void construct(T1 *p, const T2& value) //偏特化
{
    // 调用placement new--在以p为起始地址的内存块构建按对象
    new (p) T1(value);
}

// 接受指针，析构对象的版本
template <typename T>
inline void destroy(T* p) {
    p->~T();
}

// 此处可以根据传入参数的类型判断实例化(调用)哪个版本的destroy
template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    typedef typename type_trait<typename ForwardIterator::value_type>::is_POD_type is_POD_type;
    __destroy(first, last, is_POD_type());
}

// 析构迭代器范围内对象的版本
template <typename ForwardIterator>
inline void __destroy(ForwardIterator first, ForwardIterator last, __false_type)
{
    // 逐个析构对象
    for (; first < last; ++first)
        destroy(&*first);
}

template <typename ForwardIterator>
inline void __destroy(ForwardIterator first, ForwardIterator last, __true_type)
{
    ;//POD类型只有一个trival析构函数，可以不调用
}

}
#endif
