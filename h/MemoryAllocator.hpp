//
// Created by os on 1/12/23.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/mem.h"

struct BlockHeader {
    BlockHeader* next;
    size_t size;
};


class MemoryAllocator {
private:

public:
    static BlockHeader* freeMemHead;

    inline static void init() {
        freeMemHead = (BlockHeader*) HEAP_START_ADDR;
        freeMemHead->size = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof (BlockHeader));
    }

   static void* malloc(size_t size);
   static void free(void* addr);
};



#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
