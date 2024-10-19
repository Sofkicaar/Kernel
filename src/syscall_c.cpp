//
// Created by os on 1/5/23.
//

#include "../h/syscall_c.hpp"
#include "../h/codes.hpp"


void* mem_alloc (size_t size) {
    if(size == 0) return nullptr;
    //da poravnam na velicinu bloka

    __asm__ volatile("mv a1, %0" :: "r" (size));
    __asm__ volatile("mv a0, %0" :: "r" (memalloc_code));

    __asm__ volatile("ecall");

    uint64* ret;
    asm volatile ("mv %0, a0" : "=r"(ret));

    return ret;
}

int mem_free (void* addr) {
    __asm__ volatile("mv a1, %0" :: "r" (addr));
    __asm__ volatile("mv a0, %0" :: "r" (memfree_code));

    __asm__ volatile("ecall");
    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;

}
//niti
void thread_dispatch () {
    __asm__ volatile("mv a0, %0" :: "r" (thread_dispatch_code));
    __asm__ volatile("ecall");
}

int thread_exit () {
    __asm__ volatile("mv a0, %0" :: "r" (thread_exit_code));
    __asm__ volatile("ecall");

    uint64 ret;

    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_create(thread_t* handle, void(*start_routine)(void*),
                  void* arg) {
    if (handle == nullptr) return -1;
    if (start_routine == nullptr) return -2;
   void* stack_space = mem_alloc(DEFAULT_STACK_SIZE * sizeof(unsigned long));
    if (stack_space == nullptr) return -3;

    uint64 args[] = {(uint64) handle, (uint64) start_routine, (uint64) arg, (uint64) stack_space};

    __asm__ volatile("mv a1, %0"::"r" (args));

    __asm__ volatile("mv a0, %0"::"r" (thread_create_code));
    __asm__ volatile("ecall");


    uint64 ret;

    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int start_thread(thread_t handle) {
    if(handle == nullptr) return -1;
    __asm__ volatile("mv a1, %0"::"r" (handle));

    __asm__ volatile("mv a0, %0"::"r" (start_code));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int create_thread(thread_t* handle, void(*start_routine)(void*),
                  void* arg) {
    if (handle == nullptr) return -1;
    if (start_routine == nullptr) return -2;
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE * sizeof(unsigned long));
    if (stack_space == nullptr) return -3;

    uint64 args[] = {(uint64) handle, (uint64) start_routine, (uint64) arg, (uint64) stack_space};

    __asm__ volatile("mv a1, %0"::"r" (args));

    __asm__ volatile("mv a0, %0"::"r" (create_thread_code));
    __asm__ volatile("ecall");


    uint64 ret;

    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}
int sem_open (
        sem_t* handle,
        unsigned init
){
    uint64 args[] = {(uint64) handle, (uint64) init};
    __asm__ volatile("mv a1, %0"::"r" (args));
    __asm__ volatile("mv a0, %0"::"r" (sem_open_code));
    __asm__ volatile("ecall");

    uint64 ret;

    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_close(sem_t handle){
    __asm__ volatile("mv a1, %0"::"r" (handle));

    __asm__ volatile("mv a0, %0"::"r" (sem_close_code));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}
int sem_wait(sem_t id) {
    __asm__ volatile("mv a1, %0"::"r" (id));

    __asm__ volatile("mv a0, %0"::"r" (sem_wait_code));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_signal(sem_t id) {
    __asm__ volatile("mv a1, %0"::"r" (id));

    __asm__ volatile("mv a0, %0"::"r" (sem_signal_code));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r"(ret));
    return ret;
}


void putc(char c) {
    __asm__ volatile("mv a1, %0"::"r" (c));

    __asm__ volatile("mv a0, %0"::"r" (console_put));
    __asm__ volatile("ecall");
}

char getc() {

    __asm__ volatile("mv a0, %0"::"r" (console_get));
    __asm__ volatile("ecall");

    uint64 c;
    __asm__ volatile ("mv %0, a0" : "=r"(c));

    return c;
}