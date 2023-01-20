#include "stdio.h"

void Kernel_sem_init(int32_t max);
bool Kernel_sem_test(void);
void Kernel_sem_release(void);