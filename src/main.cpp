//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"

void userMain();


void userMainWrapper(void* arg) {
    userMain();
}
void idleWrapper(void* arg) {
    while(true) {
        thread_dispatch();
    }
}

int main()
{
  


    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
  

    TCB* kernel = TCB::createThread(nullptr, nullptr, nullptr);
    kernel->setSistemska(true);
    TCB::running = kernel;

    TCB* userMain;
    void* stack1 = mem_alloc(DEFAULT_STACK_SIZE* sizeof(unsigned long));
    userMain= TCB::createThread(userMainWrapper, nullptr, (uint64*)stack1);


    void* stack = mem_alloc(DEFAULT_STACK_SIZE* sizeof(unsigned long));
    TCB* idle = TCB::createThreadPomoc(idleWrapper, nullptr,(uint64*)stack);
    idle ->setSistemska(true);


    while(!userMain->isFinished()) {
        thread_dispatch();
    }
//    printString("caos");

    return 0;
}
