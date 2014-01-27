/*
 * Copyright (C) 2014
 * Jeff Kent <jeff@jkent.net>
 *
 * Configuation settings for FriendlyARM s5pv210 boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* High Level Configuration Options */
#define CONFIG_SAMSUNG		1	/* in a SAMSUNG core */
#define CONFIG_S5P		1	/* which is in a S5P Family */
#define CONFIG_S5PC110		1	/* which is in a S5PC110 */
#define CONFIG_MACH_MINI210	1	/* working with mini210 */

#include <asm/arch/cpu.h>		/* get chip and board defs */
#include <asm/sizes.h>

#define CONFIG_ARCH_CPU_INIT
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/* input clock of PLL: has 24MHz input clock at S5PC110 */
#define CONFIG_SYS_CLK_FREQ_C110	24000000

/* DRAM Base */
#define CONFIG_SYS_SDRAM_BASE		0x20000000

/* Text Base */
#define CONFIG_SYS_TEXT_BASE		0x20000000

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG
#define CONFIG_CMDLINE_EDITING

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + SZ_1M)
/*
 * select serial console configuration
 */
#define CONFIG_SERIAL0			1	/* use SERIAL0 */
#define CONFIG_BAUDRATE			115200

/* MMC */
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_S5P_SDHCI
#define CONFIG_SDHCI
#define CONFIG_MMC_SDMA

/* Timer/PWM */
#define CONFIG_PWM			1

/* It should define before config_cmd_default.h */
#define CONFIG_SYS_NO_FLASH		1

/* Command definition */
#include <config_cmd_default.h>

#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_XIMG
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_MMC

/* FAT */
#define CONFIG_CMD_FAT
#define CONFIG_FAT_WRITE

#define CONFIG_BOOTDELAY		3
#define CONFIG_ZERO_BOOTDELAY_CHECK

//#define CONFIG_BOOTCOMMAND	"run tftpboot"

#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#define CONFIG_EXTRA_ENV_SETTINGS \
	"ipaddr=192.168.0.2\0" \
	"serverip=192.168.0.1\0" \
	"nfsroot=192.168.0.1:/home/jkent/nfsroot\0" \
	"bootargs_base=console=ttySAC0,115200 init=/sbin/init\0" \
        "tftpboot=" \
		"set bootargs ${bootargs_base} root=/dev/nfs rw nfsroot=${nfsroot} ip=${ipaddr};" \
		"tftp mini210/uImage;" \
		"bootm"

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser	*/
#define CONFIG_SYS_PROMPT	"u-boot> "
#define CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x7FC0)

/* mini210 has 1 banks of SDRAM */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE	/* DDR DMC0 */
#define PHYS_SDRAM_1_SIZE	SZ_512M			/* 512 MB */

#define CONFIG_DOS_PARTITION		1

#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_TEXT_BASE + SZ_2M)

#define CONFIG_SYS_CACHELINE_SIZE       64

#include <asm/arch/gpio.h>

/* USB Gadget - currently broken*/
#if 0
#define CONFIG_USB_GADGET
#define CONFIG_USB_GADGET_S3C_UDC_OTG
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_VBUS_DRAW	2
#define CONFIG_USB_CABLE_CHECK

/* USB Composite download gadget - g_dnl_ums */
#define CONFIG_USBDOWNLOAD_GADGET

#define CONFIG_G_DNL_VENDOR_NUM		0x18d1
#define CONFIG_G_DNL_PRODUCT_NUM	0x0002
#define CONFIG_G_DNL_MANUFACTURER	"FriendlyARM"

#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#endif

/* Ethernet */
#define CONFIG_DRIVER_DM9000
#define CONFIG_DRIVER_DM9000_NO_EEPROM
#define CONFIG_RANDOM_MACADDR
#define CONFIG_DM9000_BASE		0x88000000
#define DM9000_IO			CONFIG_DM9000_BASE
#define DM9000_DATA			(CONFIG_DM9000_BASE + 8)

#define CONFIG_CMD_DHCP

/* NAND */
#define CONFIG_CMD_NAND
#define CONFIG_NAND_EXYNOS
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_SYS_NAND_USE_FLASH_BBT

#define CONFIG_SYS_NAND_BASE		S5PC110_NAND_BASE
#define CONFIG_SYS_MAX_NAND_DEVICE	1

/* MTD layout */
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define MTDIDS_DEFAULT			"nand0=s5p_nand.0"
#define	PART_BOOT			"1m(boot)ro,"
#define PART_ENV			"1m(env),"
#define PART_RECOVERY			"4m(recovery),"
#define PART_KERNEL			"5m(kernel),"
#define PART_RAMDISK			"3m(ramdisk),"
#define PART_SYSTEM			"-(system)"
#define MTDPARTS_DEFAULT		"mtdparts=s5p_nand.0:" PART_BOOT \
					PART_ENV PART_RECOVERY PART_KERNEL \
					PART_RAMDISK PART_SYSTEM

//#define CONFIG_MTD_DEBUG
//#define CONFIG_MTD_DEBUG_VERBOSE 7

/* Environment */
#define CONFIG_ENV_IS_IN_NAND		1
#define CONFIG_ENV_OFFSET		SZ_1M
#define CONFIG_ENV_SIZE			SZ_1M

#endif	/* __CONFIG_H */
