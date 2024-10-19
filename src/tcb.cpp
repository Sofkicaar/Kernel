//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* arg, uint64* stack_space)
{
    TCB* nit = new TCB(body,arg, stack_space);
    nit->setReady(true);
    Scheduler::put(nit);
    return nit;

}

void TCB::yield() {

    TCB::dispatch();

}

int TCB::exit() {
    TCB* tekuca = running;
    if(!tekuca->isRunning()) return -1; //znaci da je vec gotova
    tekuca->setRunning(false); //nit je gotova
    tekuca->setFinished(true);

    TCB::dispatch();
    return 1; //ok je ugasila si je

}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlokirana()) { Scheduler::put(old); }
    running = Scheduler::get();
    running->setRunning(true);
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie(running->isSistemska());
    running->body(running->arg);
    thread_exit();
}

TCB *TCB::createThreadPomoc(TCB::Body body, void *arg, uint64 *stack_space) {
    TCB* nit = new TCB(body,arg, stack_space);
  //  nit->setReady(false);
    return nit;
}

int TCB::start_thread() {
    TCB* nit = this;
    nit->setReady(true);
    if(nit->isRunning() || nit->isBlokirana() || nit->isFinished()) return -1;
   // nit->setRunning(true);
    Scheduler::put(nit);
    return 1;
}


