//
// Created by os on 2/4/23.
//

#ifndef PROJECT_BASE_V1_1_SEM_HPP
#define PROJECT_BASE_V1_1_SEM_HPP

#include "../lib/hw.h"
#include "../h/syscall_c.hpp"
#include "../h/list.hpp"
#include "../h/tcb.hpp"
#include "../h/MemoryAllocator.hpp"

class _sem {
private:
    int value;
    bool closed;
    List<TCB> blokiraneNiti;

public:

    _sem(unsigned init);
    ~_sem() {
        sem_close();
    }
    int sem_open (sem_t* handle, unsigned init);

    int sem_close();
    int sem_wait();
    int sem_signal();

    bool isClosed() const {return closed;}
    void setClosed(bool val) {closed = val;}



    friend class Riscv;

};



#endif //PROJECT_BASE_V1_1_SEM_HPP
