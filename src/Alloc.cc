#include <cstdlib>
#include "Alloc.h"
namespace MySTL {

void* Alloc::allocate(size_t bytes)
{
    if (bytes > MAX_BYTES)
        return malloc(MAX_BYTES);
    // 确定分配合适内存大小的链表索引
    size_t idx = freeIndex(bytes);
    obj* list = freeList_[idx];
    if (list) 
    { // 该自由链表还有剩余块 
        freeIndex[bytes] = list->next;
        return list;
    }
    else
    {
        void *result = refill(roudUp(bytes));
        return result;
    }
}
// 将内存块返回到自由链表或堆
void Alloc::deallocate(void *p, size_t bytes)
{
    if (bytes > MAX_BYTES)
    {
        free(p);
    }
    else
    { // 将该块内存头插到自由链表中
        size_t index = freeListIndex(bytes);
        obj *pobj = static_cast<obj*>(p);
        pobj->next = freeList_[index];
        freeList_[index] = pobj;
    }
    return;
}

// 重新分配内存大小
void Alloc::reallocate(void *p, size_t oldBytes, size_t newBytes)
{
    if (oldBytes > MAX_BYTES)
    {
        return realloc(p, newBytes);
    }
    else
    {
        // 将该内存块返还给自由链表后，重新分配一个合适的块
        deallocate(p, oldBytes);
        return allocate(newBytes);
    }
}

// 返回一个内存大小为bytes的块
// 通常用于当freeList中没有对应的块时，向内存池申请块
void* Alloc::refill(size_t bytes)
{
    size_t nobjs = NOBJS;
    // 调用chunk_alloc向内存池申请链表节点
    char *chunk = chunkAlloc(bytes, nobjs);
    // 若只获取到一个区块,只能返还给调用refill者，无法向自由链表中增添新节点
    if (1 == nobjs) 
        return chunk;
    int idx = freeListIndex(bytes);
    obj *&myFreeList = freeList_[idx];
    void *result = static_cast<void*> (chunk);
    obj *pcurr, *pnext;
    myFreeList = pnext = static_cast<obj*>(chunk + bytes);
    // 将剩余节点并入到自由链表中
    for (int i = 1; ;++i)
    {
        pcurr = pnext;
        pnext = static_cast<obj*> (static_cast<char*> (pnext) + bytes);
        if (i == nobjs-1) 
        {
            // 最后一个节点
            pcurr->next = nullptr;
            break;
        } 
        else
        {
            pcurr->next = pnext;
        }
    }
    return result;
}

char* Alloc::chunkAlloc(size_t bytes, size_t &nobjs)
{
    char *result;
    size_t totalBytes = bytes * nobjs;// 总共要分配的内存大小
    size_t leftBytes = endFree_ - startFree_;

    if (leftBytes >= totalBytes) 
    {
        // 若内存池的余量足以满足需求
        result = startFree_;
        startFree_ += totalBytes;
        return result;
    } else if (leftBytes >= bytes) {
        // 能够满足至少一个区块的需求
        nobjs = leftBytes/bytes;
        totalBytes = bytes * nobjs;
        result = startFree_;
        startFree += totalBytes;
        return result;
    } else {
        // 当前内存池剩余容量不足以分配最小的区块
        size_t getBytes = 2 * totalBytes + roundUp(heapSize_ >> 4);
        // 尝试将当前内存池剩余的部分编入自由链表
        if (leftByte > 0) 
        {
            obj **myFreeList = freeList_ + freeListIndex(leftBytes);
            static_cast<obj*> (startFree) -> freeListLink = *myFreeList;
            *myFreeList = static_cast<obj*> (startFree);
        }

        // 从堆上重新分配内存
        startFree = static_cast<char *> malloc(getBytes);
        if (nullptr == startFree) 
        {
            // 堆上内存分配失败
            obj ** myFreeList, *p;
            for (int i = size; i <= MAX_BYTES; i += ALIGN) 
            {
                myFreeList = freeList_ + freeListIndex(i);
                p = *myFreeList;
                if (p) 
                {
                    *myFreeList = p->next;
                    startFree_ = static_cast<char *> (p);
                    endFree_ = startFree_ + i;
                    return chunckAlloc(bytes, nobjs);
                }
            }
        }
    }
}

};
































