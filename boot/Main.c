#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "stdlib.h"
#include "task.h"
#include "Kernel.h"

void User_task0(void);
void User_task1(void);
void User_task2(void);

static void Hw_init(void);
static void Printf_test(void);
static void Kernel_init(void);

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

void User_task0(void)
{
    uint32_t local = 0;

    while(true)
    {
         debug_printf("User Task #0 SP=0x%x\n", &local);
         Kernel_yield();
    }
}

void User_task1(void)
{
    uint32_t local = 0;

    while(true)
    {
         debug_printf("User Task #1 SP=0x%x\n", &local);
         Kernel_yield();
    }
}

void User_task2(void)
{
    uint32_t local = 0;

    while(true)
    {
         debug_printf("User Task #2 SP=0x%x\n", &local);
         Kernel_yield();
    }
}