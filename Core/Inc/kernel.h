#ifndef KERNEL_H
#define KERNEL_H
#define FIRST_THREAD 1
#define CHOOSE_PSP 2
#define STACK_SIZE 0x200
#define YIELD 4
#define SHPR2 *(uint32_t*)0xE000ED1C //for setting SVC priority, bits 31-24
#define SHPR3 *(uint32_t*)0xE000ED20 // PendSV is bits 23-16
#define _ICSR *(uint32_t*)0xE000ED04 //This lets us trigger PendSV
#define default_runtimeslice 150
#include "main.h"

int stackAllocater();

extern void runFirstThread(void);

void threadSetup(void* threadFunc, void* args);

int osCreateThread(void* threadFunc, void*args);

void osThreadCreateWithDeadline(void* threadFunc, void*args, int runtimeslice);

void SVC_Handler_Main( unsigned int *svc_args );

void osKernelInitialize();

void osKernelStart();

void osSched();

void osYield(void);

typedef struct k_thread{
	uint32_t* sp;
	void(*thread_function)(void*);
	uint32_t timeslice;
	uint32_t runtime;
}thread;

#endif
