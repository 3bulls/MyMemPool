#include "MyMemPool.hpp"
#include "MyBlock.hpp"
#include <iostream>

BlockSize MyMemPool::getBlockSize(std::size_t size)
{
    if (size <= 8) return BlockSize::SIZE_8;
    else if (size <= 16) return BlockSize::SIZE_16;
    else if (size <= 32) return BlockSize::SIZE_32;
    else if (size <= 64) return BlockSize::SIZE_64;
    else if (size <= 128) return BlockSize::SIZE_128;
    else if (size <= 256) return BlockSize::SIZE_256;
    else if (size <= 512) return BlockSize::SIZE_512;
    else if (size <= 1024) return BlockSize::SIZE_1024;
    else if (size <= 2048) return BlockSize::SIZE_2048;
    else if (size <= 4096) return BlockSize::SIZE_4096;
    else return BlockSize::SIZE_HUGE;
}

MyMemPool::MyMemPool()
{
    allocatedSize = 0;
    freeSize = 0;
    freeBlocks.clear();
    allocatedBlocks.clear();
    for (int sz = 8; sz <= 4096; sz *= 2) {
        freeBlocks[static_cast<BlockSize>(sz)] = nullptr;
    }
}

MyMemPool::~MyMemPool()
{
    for (auto& pair : freeBlocks) {
        MemBlock* block = pair.second;
        while (block != nullptr) {
            if (block->getSize() > 4096) {
                free(block->getStartAddr());
            }
            MemBlock* nextBlock = block->getNext();
            delete block;
            block = nextBlock;
        }
    }

    for (auto& pair : allocatedBlocks) {
        if (pair.second->getSize() > 4096) {
            free(pair.second->getStartAddr());
            delete pair.second;
        }
    }

    for (auto& memBlock : globalMemPoolArray) {
        free(memBlock->getMemPoolPtr());
        delete memBlock;
    }

    globalMemPoolArray.clear();
    freeBlocks.clear();
    allocatedBlocks.clear();
}

void *MyMemPool::allocate(std::size_t size)
{
    BlockSize bsize = getBlockSize(size);
    // Huge Size Allocation
    if (bsize == BlockSize::SIZE_HUGE) {
        void* ptr = malloc(size);
        if (ptr != nullptr) {
            allocatedSize += size;
            MemBlock* block = new MemBlock(ptr, size, BlockType::TYPE_Block);
            allocatedBlocks[ptr] = block;
        }
        return ptr;
    }

    // Check Free Blocks
    MemBlock* block = freeBlocks[bsize];
    if (block != nullptr) {
        freeBlocks[bsize] = block->getNext();
        allocatedBlocks[block->getStartAddr()] = block;
        allocatedSize += block->getSize();
        freeSize -= block->getSize();
        return block->getStartAddr();
    }

    // Allocate from Global Pool
    MemBlock* globalMemPool = nullptr;
    for (auto& memBlock : globalMemPoolArray) {
        if (memBlock->getSize() >= static_cast<std::size_t>(bsize)) {
            globalMemPool = memBlock;
            break;
        }
    }

    if (globalMemPool == nullptr) {
        // Create a new global memory block if none available
        std::size_t poolSize = 1024 * 1024; // 1 MB
        void* poolAddr = malloc(poolSize);
        if (poolAddr == nullptr) {
            // TODO: try catch
            return nullptr; // Allocation failed
        }
        globalMemPool = new MemBlock(poolAddr, poolSize, BlockType::TYPE_Pool, poolAddr);
        globalMemPoolArray.push_back(globalMemPool);
        freeSize += poolSize;
    }

    MemBlock *newBlock = new MemBlock(
        globalMemPool->getStartAddr(),
        static_cast<std::size_t>(bsize),
        BlockType::TYPE_Block
    );
    
    globalMemPool->setStartAddr(static_cast<char*>(globalMemPool->getStartAddr()) + static_cast<std::size_t>(bsize));
    globalMemPool->setSize(globalMemPool->getSize() - static_cast<std::size_t>(bsize));
    allocatedBlocks[newBlock->getStartAddr()] = newBlock;
    allocatedSize += newBlock->getSize();
    freeSize -= newBlock->getSize();
    return newBlock->getStartAddr();

}

void MyMemPool::deallocate(void *ptr)
{
    auto it = allocatedBlocks.find(ptr);
    if (it == allocatedBlocks.end()) {
        // Invalid pointer
        return;
    }

    MemBlock* block = it->second;
    BlockSize bsize = getBlockSize(block->getSize());
    allocatedSize -= block->getSize();
    allocatedBlocks.erase(it);

    // Return to Free Blocks
    block->setNext(freeBlocks[bsize]);
    freeBlocks[bsize] = block;
    freeSize += block->getSize();
}
