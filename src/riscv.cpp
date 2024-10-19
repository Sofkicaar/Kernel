//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"
#include "../h/syscall_c.hpp"
#include "../h/sem.hpp"
#include "../h/codes.hpp"

void Riscv::popSppSpie(bool privilege)
{
   // ms_sstatus(SSTATUS_SPP);
   if (privilege) {
        ms_sstatus(SSTATUS_SPP); // 1 znaci kernel rezim
    } else {
        mc_sstatus(SSTATUS_SPP);
    }
  //  ms_sstatus(SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        uint64 a0;
        __asm__ volatile("mv %0, a0"  : "=r" (a0));

        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        switch(a0) {
            //mem_alloc
            case memalloc_code:
              //  void* pok;
                size_t size;
                __asm__ volatile ("mv %0, a1" : "=r" (size));
               // MemoryAllocator::malloc(size);
               MemoryAllocator::malloc(size);
                __asm__ volatile ("sd a0, 80(s0)");
                break;
            // mem_free
            case memfree_code:
                void *adddr;
                __asm__ volatile ("mv %0, a1" : "=r" (adddr));
                MemoryAllocator::free(adddr);
                __asm__ volatile ("sd a0, 80(s0)");
                break;
            case thread_create_code: {
                uint64 *args;
                __asm__ volatile ("mv %0, a1" : "=r" (args));
                TCB::Body body;
                body = (TCB::Body)args[1];
                thread_t* handle = (thread_t*)args[0];
                *handle = TCB::createThread(body, (void*)args[2], (uint64*)args[3]);
                break;
            }
            case thread_dispatch_code:
                TCB::dispatch();
                break;
            case thread_exit_code:
                TCB::exit();
                break;
            case create_thread_code:{
                uint64 *args;
                __asm__ volatile ("mv %0, a1" : "=r" (args));
                TCB::Body body;
                body = (TCB::Body)args[1];
                thread_t* handle = (thread_t*)args[0];
                *handle = TCB::createThreadPomoc(body, (void*)args[2], (uint64*)args[3]);
                break;
                }
            case start_code:{
                thread_t handle;
                __asm__ volatile ("mv %0, a1" : "=r" (handle));
                handle->start_thread();
                break;
            }
            case sem_open_code:
            {
                uint64 *args;
                __asm__ volatile ("mv %0, a1" : "=r" (args));
                sem_t* handle = (sem_t*)args[0];
                unsigned init = (unsigned)args[1];
                *handle = new _sem(init);
                break;
            }
            case sem_close_code:{
                sem_t id;
                __asm__ volatile ("mv %0, a1" : "=r" (id));
                id->sem_close();
                break;  }
            case sem_wait_code:
            {
                sem_t id;
                __asm__ volatile ("mv %0, a1" : "=r" (id));
               id->sem_wait();
                break; }
            case sem_signal_code:
            {
                sem_t id;
                __asm__ volatile ("mv %0, a1" : "=r" (id));
                id->sem_signal();
                break;  }
            case console_get: {
                char c = __getc();
                __asm__ volatile ("mv a0, %0"::"r" (c));
                __asm__ volatile ("sd a0, 80(s0)");
                break;
            }
            case console_put: {
                char caaar;
                __asm__ volatile ("mv %0, a1" : "=r" (caaar));
                __putc(caaar);
                break;
            }
        }
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
       // uint64 volatile sepc = r_sepc() + 4;
        //uint64 volatile sstatus = r_sstatus();
      //  TCB::timeSliceCounter = 0;
       // TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL) //prekid doso od tajmera
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP); //mask cear, bitovi(maska) ti odredjeni se brisu tj clearuju
   /*     TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc(); //eksplicitno cuvanje sepca, tj gde cemo da se vratimo iz prekidne rutine
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc); //vracas sepc (posto se prvo desava dispatch, imas novu nit koja krece izvrsavanje,
            // ovim w_sepc ces zapravo vratiti sepc ONE PRETHODNE NITI! (old) )
        } */
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        uint64 sepc = r_sepc();
        uint64 stval = r_stval();
        // unexpected trap cause
        printString("scause:");
        printInt(scause);
        printString("\n");
        printString("stval:");
        printInt(stval);
        printString("\n");
        printString("sepc:");
        printInt(sepc);
        printString("\n");
    }
}