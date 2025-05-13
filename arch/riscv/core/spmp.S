/*
 * Copyright (c) 2022 BayLibre, SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/cpu.h>
#include "asm_macros.inc"

#define CSR_SPMPCFG_BASE    0x500
#define CSR_SPMPADDR_BASE   0x510
#define CSR_SPMPSWITCH_BASE 0x550

/*
 * Prototype:
 *
 * void z_riscv_write_spmp_entries(unsigned int start,		// a0
 *                                unsigned int end,		// a1
 *                                bool clear_trailing_entries,	// a2
 *                                unsigned long *spmp_addr,		// a3
 *                                unsigned long *spmp_cfg,		// a4
 *                                unsigned long *spmp_switch)	// a5
 *
 * Called from spmp.c to write a range of SPMP entries.
 *
 * SPMP registers are accessed with the csr instruction which only takes an
 * immediate value as the actual register. In order to avoid traversing
 * the whole register list, we use the start index to jump directly to the
 * location corresponding to the start of the wanted range. For this to work
 * we disallow compressed instructions so the update block sizes are easily
 * known (luckily they're all power-of-2's simplifying the code further).
 *
 * start < end && end <= CONFIG_SPMP_SLOTS must be true.
 */

GTEXT(z_riscv_write_spmp_entries)
SECTION_FUNC(TEXT, z_riscv_write_spmp_entries)

	la t0, spmpaddr_store
	slli t1, a0, 4  /* 16-byte instruction blocks */
	add t0, t0, t1
	jr t0

spmpaddr_store:

	.option push
	.option norvc
	.set _index, 0
	.rept CONFIG_SPMP_SLOTS
	lr t0, (RV_REGSIZE * _index)(a3)
	li t1, _index + 1
	csrw (CSR_SPMPADDR_BASE + _index), t0
	beq t1, a1, spmpaddr_done
	.set _index, _index + 1
	.endr
	.option pop

spmpaddr_done:

	/*
	 * Move to the spmpcfg space:
	 * a0 = a0 / RV_REGSIZE
	 * a1 = (a1 + RV_REGSIZE - 1) / RV_REGSIZE
	 */
	la t0, spmpcfg_store
	srli a0, a0, RV_REGSHIFT
	slli t1, a0, 4  /* 16-byte instruction blocks */
	add t0, t0, t1
	addi a1, a1, RV_REGSIZE - 1
	srli a1, a1, RV_REGSHIFT
	jr t0

spmpcfg_store:

	.option push
	.option norvc
	.set _index, 0
	.rept (CONFIG_SPMP_SLOTS / RV_REGSIZE)
	lr t0, (RV_REGSIZE * _index)(a4)
	addi a0, a0, 1
	csrw (CSR_SPMPCFG_BASE + RV_REGSIZE/4 * _index), t0
	beq a0, a1, spmpcfg_done
	.set _index, _index + 1
	.endr
	.option pop

spmpcfg_done:

	beqz a2, spmpswitch_store

	la t0, spmpcfg_zerotail
	slli a0, a0, 2  /* 4-byte instruction blocks */
	add t0, t0, a0
	jr t0

spmpcfg_zerotail:

	.option push
	.option norvc
	.set _index, 0
	.rept (CONFIG_SPMP_SLOTS / RV_REGSIZE)
	csrw (CSR_SPMPCFG_BASE + RV_REGSIZE/4 * _index), zero
	.set _index, _index + 1
	.endr
	.option pop

spmpswitch_store:

	.option push
	.option norvc
	lr t0, 0(a5)
	csrr t1, CSR_SPMPSWITCH_BASE
	or t0, t0, t1
	csrw CSR_SPMPSWITCH_BASE, t0
#ifndef CONFIG_64BIT
	lr t0, 4(a5)
	csrr t1, (CSR_SPMPSWITCH_BASE + 1)
	or t0, t0, t1
	csrw (CSR_SPMPSWITCH_BASE + 1), t0
#endif
	.option pop

done:	ret
