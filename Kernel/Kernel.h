#ifndef KERNEL_NERNEL_H_
#define KERNEL_NERNEL_H_

#include "task.h"
#include "event.h"

void Kernel_yield(void);
void Kernel_start(void);
void Kernel_send_events(uint32_t event_list);
KernelEventFlag_t Kernel_wait_events(uint64_t waiting_list);
#endif /*KERNEL_NERNEL_H_*/