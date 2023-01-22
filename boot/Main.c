#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "stdlib.h"
#include "task.h"
#include "Kernel.h"
#include "event.h"
#include "msg.h"
#include "synch.h"

void User_task0(void);
void User_task1(void);
void User_task2(void);

static void Hw_init(void);
static void Printf_test(void);
static void Kernel_init(void);
static void Test_critical_section(uint32_t p, uint32_t taskId);

static uint32_t shared_value;

void main(void)
{
    Hw_init();

    uint32_t i = 100;
    while(i--)
    {
        Hal_uart_put_char('N');
    }

    Hal_uart_put_char('\n');
    
    putstr("Hello World!\n");

    Printf_test();

   Kernel_init();

    while(true);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}

static void Printf_test(void)
{
    char* str = "printf pointer test";
    char* nullptr = 0;
    uint32_t i = 5;
    uint32_t* sysctrl0 = (uint32_t*)0x10001000;

    debug_printf("%s\n", "Hello printf");
    debug_printf("output string pointer: %s\n", str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n", i);
    debug_printf("dec=%u hex=%x\n", 0xff, 0xff);
    debug_printf("print zero %u\n", 0);
    debug_printf("SYSCTRL0 = %u\n", *sysctrl0);
}

static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();
    Kernel_event_flag_init();
    Kernel_sem_init(1);

    taskId = Kernel_task_create(User_task0);
    if(taskId == NOT_ENOUGH_TASK_NUM)
    {
        putstr("Task creation fail!\n");
    }

    taskId = Kernel_task_create(User_task1);
    if(taskId == NOT_ENOUGH_TASK_NUM)
    {
        putstr("Task creation fail!\n");
    }

    taskId = Kernel_task_create(User_task2);
    if(taskId == NOT_ENOUGH_TASK_NUM)
    {
        putstr("Task creation fail!\n");
    }

    Kernel_start();

}

static void Test_critical_section(uint32_t p, uint32_t taskId)
{
    Kernel_lock_mutex();

    debug_printf("User Task #%u Send=%u\n", taskId, p);
    shared_value = p;
    Kernel_yield();
    delay(1000);
    debug_printf("User Task #%u Shared Value=%u\n", taskId, shared_value);

    Kernel_unlock_mutex();
}

void User_task0(void)
{
    uint32_t local = 0;

    debug_printf("User Task #0 SP=0x%x\n", &local);
    uint8_t cmdBuf[16];
    uint32_t cmdBufIdx = 0;
    uint8_t  uartch = 0;

    while(true)
    {
        // delay(1000);
        // debug_printf("Enter Task0\n");
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn|KernelEventFlag_CmdOut);
        switch(handle_event)
        {
            case KernelEventFlag_UartIn:
                Kernel_recv_msg(KernelMsgQ_Task0, &uartch, 1);
                if(uartch == '\r')
                {
                    cmdBuf[cmdBufIdx] = '\0';

                    Kernel_send_msg(KernelMsgQ_Task1, &cmdBufIdx, 1);
                    Kernel_send_msg(KernelMsgQ_Task1, cmdBuf, cmdBufIdx);
                    Kernel_send_events(KernelEventFlag_CmdIn);

                    cmdBufIdx = 0;
                }
                else
                {
                    cmdBuf[cmdBufIdx] = uartch;
                    cmdBufIdx++;
                    cmdBufIdx %16;
                }
                break;
            case KernelEventFlag_CmdOut:
                Test_critical_section(5, 0);
                break;
        }
        Kernel_yield();
    }
}

void User_task1(void)
{
    uint32_t local = 0;

    debug_printf("User Task #1 SP=0x%x\n", &local);

    uint8_t cmdlen = 0;
    uint8_t cmd[16] = {0};

    while(true)
    {
        // delay(1000);
        // debug_printf("Enter Task1\n");
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_CmdIn|KernelEventFlag_Unlock);
        switch(handle_event)
         {
            case KernelEventFlag_CmdIn:
                memclr(cmd, 16);
                Kernel_recv_msg(KernelMsgQ_Task1, &cmdlen,1);
                Kernel_recv_msg(KernelMsgQ_Task1, cmd,cmdlen);
                debug_printf("\nRecv Cmd: %s\n",cmd);
                break;
            case KernelEventFlag_Unlock:
                // Kernel_unlock_mutex();
                break;
         }
         Kernel_yield();
    }
}

void User_task2(void)
{
    uint32_t local = 0;

    debug_printf("User Task #2 SP=0x%x\n", &local);

    while(true)
    {
        // delay(1000);
        // debug_printf("Enter Task2\n");
        Test_critical_section(3, 2);
        Kernel_yield();
    }
}