#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_
#include "Algorithm.h"
#include "TypeTrait.h"
#include "Iterator.h"
#include "construct.h"

namespace MySTL {

// 函数uninitialized_copy()
template <typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last,
                                            ForwardIterator result, __true_type )
{
    memcpy(result, first, (last-first)*sizeof(*first));
    return result + (last-first);
}
template <typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last,
                                            ForwardIterator result, __false_type )
{
    ForwardIterator cur = result;
    while (first != last) 
    {
        construct(&*cur, *first);
        ++first, ++cur;
    }
    return cur;
}
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                   ForwardIterator result);
{
    typedef typename type_trait<typename ForwardIterator::value_type>::is_POD_type is_POD_type;
    return __uninitialized_copy(first, last, result, is_POD_type());
}

// 函数uninitialized_fill()
template <typename ForwardIterator typename T>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                const T &x, __true_type)
{
    fill(first, last, x);
}
template <typename ForwardIterator typename T>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                const T &x, __false_type) 
{
    ForwardIterator cur = first;
    while (cur != last) 
    {
        construct(&*cur, x);
        ++cur;
    }
}
template <typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
                        const T &x)
{
    typedef typename type_trait<typename ForwardIterator::value_type>::is_POD_type is_POD_type;
    __uninitialized_fill(first, last, x, is_POD_type());
}

// 函数uninitialized_fill_n()
template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, __true_type)
{
    return fill_n(first, n, x);
}
template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, __false_type)
{
    ForwardIterator cur = first;
    while (n > 0) 
    {
        construct(&*cur, x);
        --n, ++cur;
    }
    return cur;
}

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
{
    typedef typename type_trait<typename ForwardIterator::value_type>::is_POD_type is_POD_type;
    return __uninitialized_fill_n(first, n, x, is_POD_type ());
}

}
#endif
