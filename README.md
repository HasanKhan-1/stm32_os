**Overview**
-This project implements a custom Real-Time Operating System (RTOS) for STM32 microcontrollers. The RTOS features a multithreading kernel that supports pre-emptive multitasking and round-robin scheduling.

**Key Features**

    -Custom multithreading kernel
  
    -Pre-emptive multitasking
  
    -Round-robin scheduling
  
    -Context switching implementation
  
    -Task Control Block (TCB) management
  
    -Custom stack management
  
    -Register setup for task switching
  
    -SVC and PendSV interrupt handling
  
**Implementation Details**

    -Developed in C for STM32 microcontrollers
  
    -Utilizes ARM Cortex-M architecture features
  
    -Implements a priority-based scheduler
  
    -Manages task states (Ready, Running, Blocked)
  
    -Provides efficient context switching mechanism
  
**Context Switching**

  -The RTOS implements context switching using:
  
    -Custom Task Control Blocks (TCBs) for task management
  
    -Efficient stack management for each task
  
    -Proper register setup to preserve task state
    
    -SVC (Supervisor Call) for system calls
    
    -PendSV (Pendable Service Call) for scheduling
