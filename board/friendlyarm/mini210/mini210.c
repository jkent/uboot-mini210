/*
 * Copyright (C) 2014
 * Jeff Kent <jeff@jkent.net>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc.h>
#include <power/pmic.h>
#include <usb/s3c_udc.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sromc.h>
#include <netdev.h>
#include <usb.h>

DECLARE_GLOBAL_DATA_PTR;

static struct s5pc110_gpio *s5pc110_gpio = (struct s5pc110_gpio *)S5PC110_GPIO_BASE;
static int pcb_id;

#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}
#endif

static void dm9000_pre_init(void)
{
	int i;
	uint32_t srom_bw_conf, srom_bc_conf;

	/* set gpio chip select function */
	s5p_gpio_cfg_pin(&s5pc110_gpio->mp0_1, 1, GPIO_FUNC(2));

	/* Ethernet needs bus width of 16 bits */
	srom_bw_conf = SMC_DATA16_WIDTH(1);
	srom_bc_conf = SMC_BC_TACS(0) | SMC_BC_TCOS(0) | SMC_BC_TACC(5)
			| SMC_BC_TCOH(0) | SMC_BC_TAH(0)
			| SMC_BC_TACP(0) | SMC_BC_PMC(0);

	s5p_config_sromc(1, srom_bw_conf, srom_bc_conf);

	/* configure GPIO for NAND */
	s5p_gpio_cfg_pin(&s5pc110_gpio->mp0_1, 2, GPIO_FUNC(3));
	s5p_gpio_cfg_pin(&s5pc110_gpio->mp0_1, 3, GPIO_FUNC(3));
	for (i = 0; i <= 5; i++)
		s5p_gpio_cfg_pin(&s5pc110_gpio->mp0_3, i, GPIO_FUNC(2));
}

int board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_MINI210;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	dm9000_pre_init();

	return 0;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	s5p_gpio_cfg_pin(&s5pc110_gpio->j3, 2, GPIO_INPUT);
	s5p_gpio_cfg_pin(&s5pc110_gpio->j3, 3, GPIO_INPUT);
	s5p_gpio_cfg_pin(&s5pc110_gpio->j3, 4, GPIO_INPUT);

	pcb_id = s5p_gpio_get_value(&s5pc110_gpio->j3, 2);
	pcb_id |= s5p_gpio_get_value(&s5pc110_gpio->j3, 3) << 1;
	pcb_id |= s5p_gpio_get_value(&s5pc110_gpio->j3, 4) << 2;

	puts("Board: ");
	switch (pcb_id) {
		case 4:
			printf("Mini210s (ID %d)\n", pcb_id);
			break;
		default:
			printf("unknown (ID %d)\n", pcb_id);
			break;
	}

	return 0;
}
#endif

#ifdef CONFIG_GENERIC_MMC
int board_mmc_init(bd_t *bis)
{
	int i, ret = 0;

	/* MMC0 (uSD slot) */
	s5p_gpio_cfg_pin(&s5pc110_gpio->g0, 2, GPIO_INPUT);
	if (!s5p_gpio_get_value(&s5pc110_gpio->g0, 2)) {
		for (i = 0; i < 7; i++) {
			if (i == 2)
				continue;
			/* GPG0[0:6] special function 2 */
			s5p_gpio_cfg_pin(&s5pc110_gpio->g0, i, 0x2);
			/* GPG0[0:6] pull disable */
			s5p_gpio_set_pull(&s5pc110_gpio->g0, i, GPIO_PULL_NONE);
			/* GPG0[0:6] drv 4x */
			s5p_gpio_set_drv(&s5pc110_gpio->g0, i, GPIO_DRV_4X);
		}
		ret = s5p_mmc_init(0, 4);
		if (ret)
			error("MMC: Failed to init SD card (MMC:0).\n");
	}

	return ret;
}
#endif

#ifdef CONFIG_USB_GADGET
struct s3c_plat_otg_data s5pc110_otg_data = {
	.regs_phy	= S5PC110_PHY_BASE,
	.regs_otg	= S5PC110_OTG_BASE,
	.usb_phy_ctrl	= S5PC110_USB_PHY_CONTROL,
	.usb_flags	= PHY0_SLEEP,
};

int board_usb_init(int index, enum usb_init_type init)
{
	debug("USB_udc_probe\n");
	return s3c_udc_probe(&s5pc110_otg_data);
}

#ifdef CONFIG_USB_CABLE_CHECK
int usb_cable_connected(void) {
	return 1;
}
#endif
#endif

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_DRIVER_DM9000
	return dm9000_initialize(bis);
#else
	return 0;
#endif
}
