//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isRunning() const {return runn;}

    void setRunning(bool value) {runn = value;}

    bool isReady() const {return ready;}

    void setReady(bool value) {ready = value;}

    bool isSistemska() const {return sistemska;}

    void setSistemska(bool value) {sistemska = value;}

    bool isBlokirana() const {return blokirana;}

    void setBlokiraan(bool value) {blokirana = value;

    }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body, void* arg, uint64* stack_space);
    //nit koja ce da se pokrene sa start metodom, ne stavljas je u red spremnih odma
    static TCB *createThreadPomoc(Body body, void* arg, uint64* stack_space);



    int start_thread();

    static void yield();

    static TCB *running;



private:
    TCB(Body body, void* arg, uint64* stack) :
            body(body),
            stack(stack),
            arg(arg),
            context({body!=nullptr ?(uint64) &threadWrapper : 0,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
                    }),
            timeSlice(DEFAULT_TIME_SLICE),
            finished(false),
            runn(false),
            ready(true),
            sistemska(false),
            blokirana(false)
    {

       //if(body != nullptr) { setRunning(true);}
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    void* arg;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool runn;
    bool ready;
    bool sistemska;
    bool blokirana;


    friend class Riscv;
    friend class _sem;
    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();
    static int exit();



    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
