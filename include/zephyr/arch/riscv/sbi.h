#ifndef ZEPHYR_INCLUDE_ARCH_RISCV_SBI_H_
#define ZEPHYR_INCLUDE_ARCH_RISCV_SBI_H_

/**
 * @file
 * @brief RISC-V specific sbi header
 *
 * This header contains the RISCV specific sbi interface which can be invoked for S/U mode to communicate with M mode.
 */

#include <stdint.h>

/* SBI Extension IDs */
#define SBI_EXT_REDIRECT 0x11
#define SBI_EXT_0_1_SET_TIMER 0x0
#define SBI_EXT_0_1_CONSOLE_PUTCHAR 0x1
#define SBI_EXT_0_1_CONSOLE_GETCHAR 0x2
#define SBI_EXT_0_1_CLEAR_IPI 0x3
#define SBI_EXT_0_1_SEND_IPI 0x4
#define SBI_EXT_0_1_REMOTE_FENCE_I 0x5
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA 0x6
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA_ASID 0x7
#define SBI_EXT_0_1_SHUTDOWN 0x8
#define SBI_EXT_BASE 0x10
#define SBI_EXT_TIME 0x54494D45
#define SBI_EXT_IPI 0x735049
#define SBI_EXT_RFENCE 0x52464E43
#define SBI_EXT_HSM 0x48534D
#define SBI_EXT_SRST 0x53525354
#define SBI_EXT_PMU 0x504D55
#define SBI_EXT_DBCN 0x4442434E
#define SBI_EXT_SUSP 0x53555350
#define SBI_EXT_CPPC 0x43505043

/* SBI function IDs for BASE extension*/
#define SBI_EXT_BASE_GET_SPEC_VERSION 0x0
#define SBI_EXT_BASE_GET_IMP_ID 0x1
#define SBI_EXT_BASE_GET_IMP_VERSION 0x2
#define SBI_EXT_BASE_PROBE_EXT 0x3
#define SBI_EXT_BASE_GET_MVENDORID 0x4
#define SBI_EXT_BASE_GET_MARCHID 0x5
#define SBI_EXT_BASE_GET_MIMPID 0x6

/* SBI function IDs for TIME extension*/
#define SBI_EXT_TIME_SET_TIMER 0x0

/* SBI function IDs for IPI extension*/
#define SBI_EXT_IPI_SEND_IPI 0x0

/* SBI function IDs for RFENCE extension*/
#define SBI_EXT_RFENCE_REMOTE_FENCE_I 0x0
#define SBI_EXT_RFENCE_REMOTE_SFENCE_VMA 0x1
#define SBI_EXT_RFENCE_REMOTE_SFENCE_VMA_ASID 0x2
#define SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA_VMID 0x3
#define SBI_EXT_RFENCE_REMOTE_HFENCE_GVMA 0x4
#define SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA_ASID 0x5
#define SBI_EXT_RFENCE_REMOTE_HFENCE_VVMA 0x6

/* SBI function IDs for HSM extension */
#define SBI_EXT_HSM_HART_START 0x0
#define SBI_EXT_HSM_HART_STOP 0x1
#define SBI_EXT_HSM_HART_GET_STATUS 0x2
#define SBI_EXT_HSM_HART_SUSPEND 0x3

#define SBI_HSM_STATE_STARTED 0x0
#define SBI_HSM_STATE_STOPPED 0x1
#define SBI_HSM_STATE_START_PENDING 0x2
#define SBI_HSM_STATE_STOP_PENDING 0x3
#define SBI_HSM_STATE_SUSPENDED 0x4
#define SBI_HSM_STATE_SUSPEND_PENDING 0x5
#define SBI_HSM_STATE_RESUME_PENDING 0x6

#define SBI_HSM_SUSP_BASE_MASK 0x7fffffff
#define SBI_HSM_SUSP_NON_RET_BIT 0x80000000
#define SBI_HSM_SUSP_PLAT_BASE 0x10000000

#define SBI_HSM_SUSPEND_RET_DEFAULT 0x00000000
#define SBI_HSM_SUSPEND_RET_PLATFORM SBI_HSM_SUSP_PLAT_BASE
#define SBI_HSM_SUSPEND_RET_LAST SBI_HSM_SUSP_BASE_MASK
#define SBI_HSM_SUSPEND_NON_RET_DEFAULT SBI_HSM_SUSP_NON_RET_BIT
#define SBI_HSM_SUSPEND_NON_RET_PLATFORM (SBI_HSM_SUSP_NON_RET_BIT | \
                                          SBI_HSM_SUSP_PLAT_BASE)
#define SBI_HSM_SUSPEND_NON_RET_LAST (SBI_HSM_SUSP_NON_RET_BIT | \
                                      SBI_HSM_SUSP_BASE_MASK)

/* SBI function IDs for SRST extension */
#define SBI_EXT_SRST_RESET 0x0

#define SBI_SRST_RESET_TYPE_SHUTDOWN 0x0
#define SBI_SRST_RESET_TYPE_COLD_REBOOT 0x1
#define SBI_SRST_RESET_TYPE_WARM_REBOOT 0x2
#define SBI_SRST_RESET_TYPE_LAST SBI_SRST_RESET_TYPE_WARM_REBOOT

#define SBI_SRST_RESET_REASON_NONE 0x0
#define SBI_SRST_RESET_REASON_SYSFAIL 0x1

/* SBI function IDs for PMU extension */
#define SBI_EXT_PMU_NUM_COUNTERS 0x0
#define SBI_EXT_PMU_COUNTER_GET_INFO 0x1
#define SBI_EXT_PMU_COUNTER_CFG_MATCH 0x2
#define SBI_EXT_PMU_COUNTER_START 0x3
#define SBI_EXT_PMU_COUNTER_STOP 0x4
#define SBI_EXT_PMU_COUNTER_FW_READ 0x5
#define SBI_EXT_PMU_COUNTER_FW_READ_HI 0x6

struct sbiret {
    long error;
    long value;
};

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
                        unsigned long arg1, unsigned long arg2,
                        unsigned long arg3, unsigned long arg4,
                        unsigned long arg5);
int sbi_set_timer(uint64_t time);
int sbi_hsm_hart_start(unsigned long long hartid, unsigned long long saddr);
int sbi_send_ipi(unsigned long long hart_mask, unsigned long long hart_mask_base);
int sbi_irq_offload(uint64_t routine, uint64_t parameter, unsigned long id);
int sbi_runtime_except(uint64_t reason, unsigned long id);
int sbi_schedule(uint64_t new_one, uint64_t old, unsigned long id);

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2
#define SBI_CLEAR_IPI 3
#define SBI_SEND_IPI 4
#define SBI_REMOTE_FENCE_I 5
#define SBI_REMOTE_SFENCE_VMA 6
#define SBI_REMOTE_SFENCE_VMA_ASID 7
#define SBI_SHUTDOWN 8

#define SBI_ECALL(__num, __a0, __a1, __a2)                                \
    ({                                                                    \
        register unsigned long a0 __asm__("a0") = (unsigned long)(__a0);  \
        register unsigned long a1 __asm__("a1") = (unsigned long)(__a1);  \
        register unsigned long a2 __asm__("a2") = (unsigned long)(__a2);  \
        register unsigned long a7 __asm__("a7") = (unsigned long)(__num); \
        __asm__ volatile("ecall"                                          \
                         : "+r"(a0)                                       \
                         : "r"(a1), "r"(a2), "r"(a7)                      \
                         : "memory");                                     \
        a0;                                                               \
    })

#define SBI_ECALL_0(__num) SBI_ECALL(__num, 0, 0, 0)
#define SBI_ECALL_1(__num, __a0) SBI_ECALL(__num, __a0, 0, 0)
#define SBI_ECALL_2(__num, __a0, __a1) SBI_ECALL(__num, __a0, __a1, 0)

#define SBI_PUTCHAR(__a0) SBI_ECALL_1(SBI_CONSOLE_PUTCHAR, __a0)
#define SBI_TIMER(__a0) SBI_ECALL_1(SBI_SET_TIMER, __a0)

#endif
