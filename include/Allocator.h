#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_
namespace MySTL {
// 空间配置器--以对象T的大小为单位分配内存空间 
template <typename T>
Allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;// 指针的差值
private:
};
}
#endif
