/*
 * Copyright (C) 2014
 * Jeff Kent <jeff@jkent.net>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>

	.globl mem_ctrl_asm_init
mem_ctrl_asm_init:
	ldr	r0, =S5PC110_DMC0_BASE			@ 0xF0000000

	/* DLL parameter setting */
	ldr	r2, =0x00101000
	str	r2, [r0, #0x018]			@ PHYCONTROL0_OFFSET
	ldr	r2, =0x00000086
	str	r2, [r0, #0x01C]			@ PHYCONTROL1_OFFSET

	/* DLL on */
	ldr	r2, =0x00101002
	str	r2, [r0, #0x018]			@ PHYCONTROL0_OFFSET

	/* DLL start */
	ldr	r2, =0x00101003
	str	r2, [r0, #0x018]			@ PHYCONTROL0_OFFSET

	mov	r3, #0x4000
wait:	subs	r3, r3, #0x1
	cmp	r3, #0x0
	bne	wait

	/* Force value locking for DLL off */
	str	r2, [r0, #0x018]			@ PHYCONTROL0_OFFSET

	/* DLL off */
	ldr	r2, =0x00101009
	str	r2, [r0, #0x018]			@ PHYCONTROL0_OFFSET

	/* auto refresh off */
	ldr	r2, =0x0fff2090
	str	r2, [r0, #0x000]			@ CONCONTROL_OFFSET

	/*
	 * Burst Length 4, 1 chip, 32-bit, DDR2
	 */
	ldr	r2, =0x00202400
	str	r2, [r0, #0x004]			@ MEMCONTROL_OFFSET

	/*
	 * DMC0
	 * 0x20 -> 0x20000000
	 * 0xe0 -> 0x3FFFFFFF
	 * [15:12] 0: Linear
	 * [11:8 ] 3: 10 bits
	 * [ 7:4 ] 2: 14 bits
	 * [ 3:0 ] 3: 8 banks
	 */
	ldr	r2, =0x20e00323
	ldr	r3, =0x40e00323
	str	r2, [r0, #0x008]			@ MEMCONFIG0_OFFSET
	str	r3, [r0, #0x00C]			@ MEMCONFIG1_OFFSET (dummy)

	ldr	r2, =0x20000000
	str	r2, [r0, #0x014]			@ PRECHCONFIG_OFFSET

	/*
	 * DMC0: CS0: 200MHz
	 *
	 * 7.8us * 200MHz %LE %LONG1560(0x618)
	 */
	ldr	r2, =0x00000618
	str	r2, [r0, #0x030]			@ TIMINGAREF_OFFSET

	ldr	r2, =0x1A233309
	str	r2, [r0, #0x034]			@ TIMINGROW_OFFSET

	ldr	r2, =0x23250000
	str	r2, [r0, #0x038]			@ TIMINGDATA_OFFSET

	ldr	r2, =0x09C80232
	str	r2, [r0, #0x03C]			@ TIMINGPOWER_OFFSET

	/* chip0 Deselect */
	ldr	r2, =0x07000000
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 PALL */
	ldr	r2, =0x01000000
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 EMRS2 */
	ldr	r2, =0x00020000
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 EMRS3 */
	ldr	r2, =0x00030000
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 EMRS1 (DLL on) */
	ldr	r2, =0x00010400
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 MRS (DLL reset) */
	ldr	r2, =0x00000B52
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 PALL */
	ldr	r2, =0x01000000
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 REFA */
	ldr	r2, =0x05000000
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET
	/* chip0 REFA */
	ldr	r2, =0x05000000
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 MRS (DLL unreset) */
	ldr	r2, =0x00000A52
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 EMRS1 (OCD default) */
	ldr	r2, =0x00010780
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* chip0 EMRS1 (OCD exit) */
	ldr	r2, =0x00010400
	str	r2, [r0, #0x010]			@ DIRECTCMD_OFFSET

	/* auto refresh on */
	ldr	r2, =0x0fff20B0
	str	r2, [r0, #0x000]			@ CONCONTROL_OFFSET

	/* PwrdnConfig */
	ldr	r2, =0x00100002
	str	r2, [r0, #0x028]			@ PWRDNCONFIG_OFFSET

	/*
	 * Burst Length 4, 1 chip, 32-bit, DDR2
	 * dynamic clock, dynamic power down, timeout precharge
	 */
	ldr	r2, =0x00202413
	str	r2, [r0, #0x004]			@ MEMCONTROL_OFFSET

	mov	pc, lr

	.ltorg
