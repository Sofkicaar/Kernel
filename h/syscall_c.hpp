//
// Created by os on 1/5/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP

#include "../lib/hw.h"

void* mem_alloc (size_t size);
int mem_free (void*);

//niti
class TCB;
typedef TCB* thread_t;
int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg

);

int thread_exit();
void thread_dispatch();

int start_thread(thread_t handle);
int create_thread(thread_t* handle,
                  void(*start_routine)(void*),
                  void* arg);

//semafori

class _sem;
typedef _sem* sem_t;
int sem_open (
        sem_t* handle,
        unsigned init
);

int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

void putc (char);
char getc();
#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
