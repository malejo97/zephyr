/*
 * Copyright (c) 2022 BayLibre SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/arch/riscv/sbi.h>
#include <zephyr/arch/riscv/syscall.h>
#include <zephyr/irq_offload.h>

void arch_irq_offload(irq_offload_routine_t routine, const void *parameter)
{
    sbi_irq_offload((uintptr_t)routine, (uintptr_t)parameter, RV_ECALL_IRQ_OFFLOAD);
}

void arch_irq_offload_init(void)
{
}
