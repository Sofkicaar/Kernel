//
// Created by os on 2/5/23.
//
#include "../h/syscall_cpp.hpp"


void Thread::dispatch() {
    thread_dispatch();
}


Thread::~Thread() {
  //  thread_exit();
}

Thread::Thread(void (*body)(void *), void *arg) {
    create_thread(&myHandle, body, arg);
}

int Thread::start() {
    return start_thread(myHandle);
}

Thread::Thread() {
    create_thread(&myHandle, Thread::thread_wrapper, this);
    //pozivas create thread i prosledjujes kao drugi argument, thread_wrapper -> poziva run metodu
}

void Thread::thread_wrapper(void *nit) {
    return (((Thread*)nit)->run());
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char ch) {
    ::putc(ch);
}

