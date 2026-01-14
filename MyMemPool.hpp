// MyMemPool提供的接口：
// 1. 默认构造函数 MyMemPool()
// 2. 析构函数 ~MyMemPool()
// 3. 分配内存 void* allocate(std::size_t size)
// 4. 释放内存 void deallocate(void* ptr, std::size_t size)
// 5. 获取当前池中已分配的内存总量 std::size_t getAllocatedSize() const
// 6. 按照block size分类管理内存块，支持多种大小的内存分配请求，链表管理空闲内存块

// MemBlock类表示内存块的基本单元，包含必要的成员变量和方法以支持内存池的管理。
// 1. 默认构造函数 MemBlock()
// 2. 单位：8/16/32/64/128/256/512/1024/2048/4096 字节的内存块，超出4096字节的请求直接使用系统分配
#pragma once

#include <cstddef> // for std::size_t
#include <unordered_map>
#include <vector>
#include "MyBlock.hpp"

enum class BlockSize {
    SIZE_8 = 8,
    SIZE_16 = 16,
    SIZE_32 = 32,
    SIZE_64 = 64,
    SIZE_128 = 128,
    SIZE_256 = 256,
    SIZE_512 = 512,
    SIZE_1024 = 1024,
    SIZE_2048 = 2048,
    SIZE_4096 = 4096,
    SIZE_HUGE = 0
};

class MyMemPool {
public:
    static MyMemPool& getInstance() {
        static MyMemPool instance;
        return instance;
    }
    void* allocate(std::size_t size);
    void deallocate(void* ptr);

    std::size_t getAllocatedSize() const {return allocatedSize;};
    std::size_t getFreeSize() const  {return freeSize;};

private:
    std::size_t allocatedSize;
    std::size_t freeSize;
    std::unordered_map<BlockSize, MemBlock*> freeBlocks;
    std::vector<MemBlock*> globalMemPoolArray;

    std::unordered_map<void*, MemBlock*> allocatedBlocks;

    BlockSize getBlockSize(std::size_t size);
    MyMemPool();
    ~MyMemPool();
};
