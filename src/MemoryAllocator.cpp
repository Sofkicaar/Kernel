//
// Created by os on 1/29/23.
//
#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"

BlockHeader *MemoryAllocator:: freeMemHead = nullptr;
//klk avgust 2021.
void* MemoryAllocator::malloc(size_t size) {
    if(size == 0) return nullptr;
    if(size % MEM_BLOCK_SIZE != 0) {
       size = (size/MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
    }
    BlockHeader *blk = freeMemHead, *prev = nullptr;
    for (; blk!=nullptr; prev=blk, blk=blk->next)
        if (blk->size>=size) break;
    if (blk == nullptr) {
        return nullptr;
    }
    size_t remainingSize = blk->size - size;
    if (remainingSize >= sizeof(BlockHeader) + MEM_BLOCK_SIZE) {
        // A fragment remains
        blk->size = size;
        size_t offset = sizeof(BlockHeader) + size;
        BlockHeader* newBlk = (BlockHeader*)((char*)blk + offset);
        if (prev) prev->next = newBlk;
        else freeMemHead = newBlk;
        newBlk->next = blk->next;
        newBlk->size = remainingSize - sizeof(BlockHeader);
    } else {

        if (prev) prev->next = blk->next;
        else freeMemHead = blk->next;
    }
    blk->next = nullptr;
    return (char*)blk + sizeof(BlockHeader);
}
void MemoryAllocator::free(void *addr) {
    BlockHeader* freeBlock = (BlockHeader*)((char*)addr - sizeof(BlockHeader)); // ili -MEM_BLOCK_SIZE
    freeBlock->next = freeMemHead;
    freeMemHead = freeBlock;
}