#include <zephyr/arch/riscv/sbi.h>

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
                        unsigned long arg1, unsigned long arg2,
                        unsigned long arg3, unsigned long arg4,
                        unsigned long arg5) {
    struct sbiret ret;

    register unsigned long a0 __asm__("a0") = (unsigned long)(arg0);
    register unsigned long a1 __asm__("a1") = (unsigned long)(arg1);
    register unsigned long a2 __asm__("a2") = (unsigned long)(arg2);
    register unsigned long a3 __asm__("a3") = (unsigned long)(arg3);
    register unsigned long a4 __asm__("a4") = (unsigned long)(arg4);
    register unsigned long a5 __asm__("a5") = (unsigned long)(arg5);
    register unsigned long a6 __asm__("a6") = (unsigned long)(fid);
    register unsigned long a7 __asm__("a7") = (unsigned long)(ext);
    __asm__ volatile("ecall"
                     : "+r"(a0), "+r"(a1)
                     : "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
                     : "memory");
    ret.error = a0;
    ret.value = a1;

    return ret;
}


struct sbiret sbi_ecall_syscall(int ext, int fid,unsigned long id, unsigned long arg0,
			unsigned long arg1, unsigned long arg2,
			unsigned long arg3, unsigned long arg4)
{
	struct sbiret ret;

	register unsigned long t0 __asm__ ("t0") = (unsigned long)(id);
	register unsigned long a0 __asm__ ("a0") = (unsigned long)(arg0);
	register unsigned long a1 __asm__ ("a1") = (unsigned long)(arg1);
	register unsigned long a2 __asm__ ("a2") = (unsigned long)(arg2);
	register unsigned long a3 __asm__ ("a3") = (unsigned long)(arg3);
	register unsigned long a4 __asm__ ("a4") = (unsigned long)(arg4);
	register unsigned long a6 __asm__ ("a6") = (unsigned long)(fid);
	register unsigned long a7 __asm__ ("a7") = (unsigned long)(ext);
	__asm__ volatile ("ecall"
		      : "+r" (t0), "+r" (a0), "+r" (a1)
		      : "r" (a2), "r" (a3), "r" (a4), "r" (a6), "r" (a7)
		      : "memory");
	ret.error = a0;
	ret.value = a1;

	return ret;
}


int sbi_set_timer(uint64_t time) {
    struct sbiret ret;

    ret = sbi_ecall(SBI_EXT_TIME_SET_TIMER, 0, time, 0, 0, 0, 0, 0);
    if (ret.error)
        return ret.error;
    else
        return 0;
}

int sbi_irq_offload(uint64_t routine,uint64_t parameter, unsigned long id){
	struct sbiret ret;

	ret = sbi_ecall_syscall(SBI_EXT_REDIRECT, 0 ,id,routine, parameter, 0, 0, 0);
	if (ret.error)
		return ret.error;
	else
		return 0;
}

int sbi_runtime_except(uint64_t reason, unsigned long id){
	struct sbiret ret;

	ret = sbi_ecall_syscall(SBI_EXT_REDIRECT, 0 ,id,reason, 0, 0, 0, 0);
	if (ret.error)
		return ret.error;
	else
		return 0;
}

int sbi_schedule(uint64_t new_one,uint64_t old, unsigned long id){
	struct sbiret ret;

	ret = sbi_ecall_syscall(SBI_EXT_REDIRECT, 0 ,id,new_one,old, 0, 0, 0);
	if (ret.error)
		return ret.error;
	else
		return 0;
}
