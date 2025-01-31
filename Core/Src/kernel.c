#include "kernel.h"
#include "main.h"


volatile uint32_t* stackptr;
volatile uint32_t numStacks;
volatile uint32_t* MSP_INIT_VAL;
volatile uint32_t PSP;
volatile uint32_t currentThread;
volatile int osStarted = 0;



thread threadArray[31];


void SVC_Handler_Main( unsigned int *svc_args ){
	unsigned int svc_number;
/*
* Stack contains:
* r0, r1, r2, r3, r12, r14, the return address and xPSR
* First argument (r0) is svc_args[0]
*/
	svc_number = ( ( char * )svc_args[ 6 ] )[ -2 ] ;
	switch( svc_number )
	{
	case 17: //17 is sort of arbitrarily chosen
		printf("Success!\r\n");
		break;
	case 18:
		printf("System call 1\r\n");
		break;
	case 19:
		printf("Nolen\r\n");
		break;
	case FIRST_THREAD:
		__set_PSP(stackptr);
		runFirstThread();
		break;
	default: /* unknown SVC */
		break;
	case YIELD:
		_ICSR |= 1<<28;
		__asm("isb");
		break;
	}
}




void osKernelInitialize(){
	numStacks = 0;
	currentThread = 0;
	MSP_INIT_VAL = *(uint32_t**)0x0;
	PSP = (uint32_t)MSP_INIT_VAL - 0x200;
	//set the priority of PendSV to almost the weakest
	SHPR3 |= 0xFE << 16; //shift the constant 0xFE 16 bits to set PendSV priority
	SHPR2 |= 0xFDU << 24; //Set the priority of SVC higher than PendSV
};

int stackAllocater(){
	if(numStacks * STACK_SIZE + STACK_SIZE > 0x3E00){
		return NULL;
	}
	else{
		numStacks += 1;
		return (int*)((uint32_t)MSP_INIT_VAL - STACK_SIZE * numStacks);
	}
};
//added args argument
void threadSetup(void* threadFunc, void* args){
	stackptr = stackAllocater();
	//stackptr = (uint32_t*)threadArray[numStacks-1].sp;
	*(--stackptr) = 1<<24;
	*(--stackptr) = (uint32_t)threadFunc;
	  for(int i = 0; i < 5; i++)
	  {
	  	*(--stackptr) = 0xA;
	  }
	  //changed the for loop to put stuff in r0
	  *(--stackptr) = args;
	  for(int i = 0; i < 8; i ++)
		{
		 *(--stackptr) == 0xA;
		}

}
// added args argument
int osCreateThread(void* threadFunc, void* args){
	threadSetup(threadFunc, args);
	if(stackptr == NULL){
		return 0;
	}
	else{
		thread newThread;
		newThread.sp = stackptr;
		newThread.thread_function = threadFunc;
		newThread.timeslice = default_runtimeslice;
		newThread.runtime = default_runtimeslice;
		threadArray[numStacks-1] = newThread;
		return 1;
	}
}

void osThreadCreateWithDeadline(void* threadFunc, void*args, int runtimeslice){
	threadSetup(threadFunc, args);
		if(stackptr == NULL){
			return 0;
		}
		else{
			thread newThread;
			newThread.sp = stackptr;
			newThread.thread_function = threadFunc;
			newThread.timeslice = runtimeslice;
			newThread.runtime = runtimeslice;
			threadArray[numStacks-1] = newThread;
			return 1;
		}
}

void osYield(void){
	__asm("SVC #4");
}

void osSched(){
	threadArray[currentThread].sp = (uint32_t*)(__get_PSP() - 8*4);
	currentThread = (++currentThread)%numStacks;
	__set_PSP(threadArray[currentThread].sp);

}

void osKernelStart(){
	osStarted = 1;
	stackptr = threadArray[0].sp;

	__asm("SVC #1");
}

