/* 第二级配置器
 * 若申请空间小于128bytes，则由Alloc向从自由链表中分配空间;
 * 否则，需要调用malloc()向堆申请空间。
*/
#ifndef _ALLOC_H_
#define _ALLOC_H_
namespace MySTL {
class Alloc {
public:
    static void* allocate(size_t bytes);
    static void deallocate(void *p, size_t bytes);
    static void reallocate(void *p, size_t oldBytes, size_t newBytes);
private:
    enum { ALIGN = 8};// 小型区块的上调边界--即申请区块不足时需要填充空白块
    enum { MAX_BYTES = 128};// 小型区块的上限，第一级配置能够申请最大区块
    enum { NFREELISTS = MAX_BYTES/ALIGN};// 自由链表的个数
    enum { NOBJS = 20};// 默认缺省值：每次向内存池申请自由链表节点的数目
    
    union obj {
        obj * next;
        char client[1];
    };
    // 调整申请的区块大小
    static size_t roudUp(size_t bytes) 
    {
        return ((bytes + ALIGN-1) & ~(ALIGN-1));
    }

    // 自由链表(volatile用于阻止编译器进行优化，使得每次取数据都从内存处提取)
    static obj * volatile freeList_[NFREELISTS];
    // 根据区块的大小，决定提取内存块的链表，标号从0开始
    static size_t freeListIndex(size_t bytes)
    {
        return ((bytes + ALIGN-1)/ALIGN-1);
    }
    static void* refill(size_t);
    // 通过malloc向系统的堆申请最多size * nobjs大小的块
    // nobjs是值-结果参数，返回实际分配的块数
    static char* chunkAlloc(size_t size, int &nobjs);

    // 标记内存池区块的信息
    static char *startFree_;// 起始位置
    static char *endFree_;// 结束位置
    static size_t heapSize_;// 
};
// 定义Alloc中的static数据成员
Alloc::obj* volatile freeList_[NFREELISTS] = 
{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
char* Alloc::startFree_ = nullptr;
char* Alloc::endFree_ = nullptr;
size_t heapSize_ = 0;
}
#endif
