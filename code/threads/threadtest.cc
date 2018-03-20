// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "elevatortest.h"

// testnum is set in main.cc
int testnum = 3;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 10; num++) 
    {
	    printf("*** thread %d looped %d times\n", which, num);
        /* lab1 begin */
        PrintThreadStates();
        /* lab1 end */
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1\n");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, (void*)1);
    SimpleThread(0);
}
/* lab1 begin */
//----------------------------------------------------------------------
// ThreadTest2
// 	New 128 threads.
//----------------------------------------------------------------------

void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2\n");
    for (int i = 0; i < 127; ++i)
    {
        Thread *t = new Thread("forked thread");
        if (i == 126)
            PrintThreadStates();
    }
}
/* lab1 end */
/* lab2 begin */
//----------------------------------------------------------------------
// ThreadTest3
//  2 threads, round robin.
//----------------------------------------------------------------------
void RRtestY(int dummy)
{
    while(currentThread->getRuntime() < 200) 
    { 
        currentThread->Yield();
        interrupt->SetLevel(IntOn);
        interrupt->SetLevel(IntOff);
    }
}
void RRtest(int dummy)
{
    while(currentThread->getRuntime() < 200) 
    {
        interrupt->SetLevel(IntOn);
        interrupt->SetLevel(IntOff);
    };
}
void
ThreadTest3()
{
    DEBUG('t', "Entering ThreadTest3\n");
    Thread *t1 = new Thread("forked thread 1");
    t1->Fork(RRtest, (void*)1);
    Thread *t2 = new Thread("forked thread 2");
    t2->Fork(RRtestY, (void*)2);
    RRtest(0);
}
/* lab2 end */
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) 
    {
    case 1:
	{
        ThreadTest3();
        break;
    }
    case 2:
    {
        ThreadTest2();
        break;
    }
    case 3:
    {
        ThreadTest3();
        break;
    }
    default:
	printf("No test specified.\n");
	break;
    }
}
