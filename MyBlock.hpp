#pragma once

#include <cstddef> // for std::size_t

enum BlockType {
    TYPE_Block,
    TYPE_Pool
};

class MemBlock {
public:
    MemBlock(void *addr, std::size_t sz, BlockType t, void *memPool = nullptr)
        : startAddr(addr), size(sz), type(t), memPoolPtr(memPool) {}
    MemBlock(void *addr, std::size_t sz, BlockType t, MemBlock* p, MemBlock* n,  void *memPool = nullptr)
        : startAddr(addr), size(sz),  type(t), prev(p), next(n), memPoolPtr(memPool) {}
    MemBlock* getPrev() const { return prev; }
    MemBlock* getNext() const { return next; }
    std::size_t getSize() const { return size; }
    void setSize(std::size_t sz) { size = sz; }
    void *getStartAddr() const { return startAddr; }
    void setStartAddr(void* addr) { startAddr = addr; }

    MemBlock& setPrev(MemBlock* p) {
        prev = p;
        return *this;
    }
    MemBlock& setNext(MemBlock* n) {
        next = n;
        return *this;
    }

    void *getMemPoolPtr() const { return memPoolPtr; }

    ~MemBlock();

private:
    MemBlock();// 默认构造函数 不应该被调用
    BlockType type;
    MemBlock* prev;
    MemBlock* next;
    std::size_t size;
    void *startAddr;
    void *memPoolPtr; 

};