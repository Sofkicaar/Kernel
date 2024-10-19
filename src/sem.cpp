//
// Created by os on 2/4/23.
//
#include "../h/sem.hpp"

_sem::_sem(unsigned init) {
    value = init;
    closed = false;
}
int _sem::sem_close() {
    if(closed) return -1;
    value = 0;
    for(int i = 0; i<blokiraneNiti.size(); i++)
    {
        TCB* spremnaNit = blokiraneNiti.removeLast();
        spremnaNit->setReady(true);
        spremnaNit->setBlokiraan(false);
        Scheduler::put(spremnaNit);
    }

    closed = true;
    return 1;
}

int _sem::sem_signal() {
    if(closed) return -1;
    value++; //ovo uvek mozes
    if(value <= 0)  { //postoji neko blokiran
        TCB* nit = blokiraneNiti.removeFirst();
        nit->setBlokiraan(false);
        nit->setReady(true);
        Scheduler::put(nit);
    }
    return 1;
}

int _sem::sem_wait() {
    if(closed) return -1;
    value--;
    if(value < 0) {
        TCB* old = TCB::running;
        old->setBlokiraan(true);
        if(blokiraneNiti.size() == 0) {
            blokiraneNiti.addFirst(old);
        }
        else blokiraneNiti.addLast(old);
        TCB::dispatch();

    }
    return 1;
}

int _sem::sem_open(sem_t *handle, unsigned int init) {
    sem_t semafor = new _sem(init);
    if(semafor->isClosed()) return -1;
    *handle = semafor;
    return 1;
}


