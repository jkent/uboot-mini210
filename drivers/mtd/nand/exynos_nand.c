/*
 * Copyright (C) 2014
 * Jeff Kent <jeff@jkent.net>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>

#include <nand.h>
#include <asm/io.h>

struct exynos_nand {
	uint32_t nfconf;
	uint32_t nfcont;
	uint32_t nfcmd;
	uint32_t nfaddr;
	uint32_t nfdata;
	uint32_t nfeccd0;
	uint32_t nfeccd1;
	uint32_t nfeccd;
	uint32_t nfsblk;
	uint32_t nfeblk;
	uint32_t nfstat;
	uint32_t nfstat0;
	uint32_t nfstat1;
	uint32_t nfsecc;
	uint32_t nfmlcbitpt;
	uint32_t _reserved1[32753];
	uint32_t nfeccconf;
	uint32_t _reserved2[7];
	uint32_t nfecccont;
	uint32_t _reserved3[3];
	uint32_t nfeccstat;
	uint32_t _reserved4[3];
	uint32_t nfeccsecstat;
	uint32_t _reserved5[19];
	uint32_t nfeccprgecc[7];
	uint32_t _reserved6[5];
	uint32_t nfeccerl[8];
	uint32_t _reserved7[4];
	uint32_t nfeccerp[4];
	uint32_t _reserved8[4];
	uint32_t nfeccconecc[7];
};

#define NFCONF_ECC_MASK			(3<<23)
#define NFCONF_ECC_1BIT			(0<<23)
#define NFCONF_ECC_4BIT			(1<<23)
#define NFCONF_ECC_NONE			(3<<23)
#define NFCONF_TACLS(x)			(((x)-1)<<12)
#define NFCONF_TWRPH0(x)		(((x)-1)<<8)
#define NFCONF_TWRPH1(x)		(((x)-1)<<4)

#define NFCONT_nFCE3			(1<<23)
#define NFCONT_nFCE2			(1<<22)
#define NFCONT_LOCK			(1<<16)
#define NFCONT_MECCLOCK			(1<<7)
#define NFCONT_SECCLOCK			(1<<6)
#define NFCONT_INITMECC			(1<<5)
#define NFCONT_nFCE1			(1<<2)
#define NFCONT_nFCE0			(1<<1)
#define NFCONT_EN			(1<<0)

//#define NFSTAT_READY(x)			(1<<(28+x))
//#define NFSTAT_RnB_TRANS(x)		(1<<(24+x))
#define NFSTAT_ILLEGAL_ACCESS		(1<<5)
#define NFSTAT_RnB_CHANGE		(1<<4)
#define NFSTAT_RnB_PIN			(1<<0)

#define NFECCCONF_MSGLEN(x)		(((x)-1)<<16)
#define NFECCCONF_ECCTYPE_MASK		(15<<0)
#define NFECCCONF_ECCTYPE_NONE		(0<<0)
#define NFECCCONF_ECCTYPE_8BIT		(3<<0)
#define NFECCCONF_ECCTYPE_12BIT		(4<<0)
#define NFECCCONF_ECCTYPE_16BIT		(5<<0)

#define NFECCCONT_ENCODE		(1<<16)
#define NFECCCONT_INITMECC		(1<<2)
#define NFECCCONT_RESETECC		(1<<0)

#define NFECCSTAT_ECCBUSY		(1<<31)
#define NFECCSTAT_ENCDONE		(1<<25)
#define NFECCSTAT_DECDONE		(1<<24)
#define NFECCSTAT_USEDPAGE		(1<<8)

static struct nand_ecclayout nand_oob_512 = {
	.eccbytes = 442,
	.eccpos = {
		 36,  37,  38,  39,  40,  41,  42,  43,
		 44,  45,  46,  47,  48,  49,  50,  51,
		 52,  53,  54,  55,  56,  57,  58,  59,
		 60,  61,  64,  65,  66,  67,  68,  69,
		 70,  71,  72,  73,  74,  75,  76,  77,
		 78,  79,  80,  81,  82,  83,  84,  85,
		 86,  87,  88,  89,  92,  93,  94,  95,
		 96,  97,  98,  99, 100, 101, 102, 103,
		104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 120, 121,
		122, 123, 124, 125, 126, 127, 128, 129,
		130, 131, 132, 133, 134, 135, 136, 137,
		138, 139, 140, 141, 142, 143, 144, 145,
		148, 149, 150, 151, 152, 153, 154, 155,
		156, 157, 158, 159, 160, 161, 162, 163,
		164, 165, 166, 167, 168, 169, 170, 171,
		172, 173, 176, 177, 178, 179, 180, 181,
		182, 183, 184, 185, 186, 187, 188, 189,
		190, 191, 192, 193, 194, 195, 196, 197,
		198, 199, 200, 201, 204, 205, 206, 207,
		208, 209, 210, 211, 212, 213, 214, 215,
		216, 217, 218, 219, 220, 221, 222, 223,
		224, 225, 226, 227, 228, 229, 232, 233,
		234, 235, 236, 237, 238, 239, 240, 241,
		242, 243, 244, 245, 246, 247, 248, 249,
		250, 251, 252, 253, 254, 255, 256, 257,
		260, 261, 262, 263, 264, 265, 266, 267,
		268, 269, 270, 271, 272, 273, 274, 275,
		276, 277, 278, 279, 280, 281, 282, 283,
		284, 285, 288, 289, 290, 291, 292, 293,
		294, 295, 296, 297, 298, 299, 300, 301,
		302, 303, 304, 305, 306, 307, 308, 309,
		310, 311, 312, 313, 316, 317, 318, 319,
		320, 321, 322, 323, 324, 325, 326, 327,
		328, 329, 330, 331, 332, 333, 334, 335,
		336, 337, 338, 339, 340, 341, 344, 345,
		346, 347, 348, 349, 350, 351, 352, 353,
		354, 355, 356, 357, 358, 359, 360, 361,
		362, 363, 364, 365, 366, 367, 368, 369,
		372, 373, 374, 375, 376, 377, 378, 379,
		380, 381, 382, 383, 384, 385, 386, 387,
		388, 389, 390, 391, 392, 393, 394, 395,
		396, 397, 400, 401, 402, 403, 404, 405,
		406, 407, 408, 409, 410, 411, 412, 413,
		414, 415, 416, 417, 418, 419, 420, 421,
		422, 423, 424, 425, 428, 429, 430, 431,
		432, 433, 434, 435, 436, 437, 438, 439,
		440, 441, 442, 443, 444, 445, 446, 447,
		448, 449, 450, 451, 452, 453, 456, 457,
		458, 459, 460, 461, 462, 463, 464, 465,
		466, 467, 468, 469, 470, 471, 472, 473,
		474, 475, 476, 477, 478, 479, 480, 481,
		484, 485, 486, 487, 488, 489, 490, 491,
		492, 493, 494, 495, 496, 497, 498, 499,
		500, 501, 502, 503, 504, 505, 506, 507,
		508, 509 },
	.oobfree = {
		{.offset = 4,
		.length = 32 } }
};

static struct nand_ecclayout nand_oob_640 = {
	.eccbytes = 442,
	.eccpos = {
		 40,  41,  42,  43,  44,  45,  46,  47,
		 48,  49,  50,  51,  52,  53,  54,  55,
		 56,  57,  58,  59,  60,  61,  62,  63,
		 64,  65,  68,  69,  70,  71,  72,  73,
		 74,  75,  76,  77,  78,  79,  80,  81,
		 82,  83,  84,  85,  86,  87,  88,  89,
		 90,  91,  92,  93,  96,  97,  98,  99,
		100, 101, 102, 103, 104, 105, 106, 107,
		108, 109, 110, 111, 112, 113, 114, 115,
		116, 117, 118, 119, 120, 121, 124, 125,
		126, 127, 128, 129, 130, 131, 132, 133,
		134, 135, 136, 137, 138, 139, 140, 141,
		142, 143, 144, 145, 146, 147, 148, 149,
		152, 153, 154, 155, 156, 157, 158, 159,
		160, 161, 162, 163, 164, 165, 166, 167,
		168, 169, 170, 171, 172, 173, 174, 175,
		176, 177, 180, 181, 182, 183, 184, 185,
		186, 187, 188, 189, 190, 191, 192, 193,
		194, 195, 196, 197, 198, 199, 200, 201,
		202, 203, 204, 205, 208, 209, 210, 211,
		212, 213, 214, 215, 216, 217, 218, 219,
		220, 221, 222, 223, 224, 225, 226, 227,
		228, 229, 230, 231, 232, 233, 236, 237,
		238, 239, 240, 241, 242, 243, 244, 245,
		246, 247, 248, 249, 250, 251, 252, 253,
		254, 255, 256, 257, 258, 259, 260, 261,
		264, 265, 266, 267, 268, 269, 270, 271,
		272, 273, 274, 275, 276, 277, 278, 279,
		280, 281, 282, 283, 284, 285, 286, 287,
		288, 289, 292, 293, 294, 295, 296, 297,
		298, 299, 300, 301, 302, 303, 304, 305,
		306, 307, 308, 309, 310, 311, 312, 313,
		314, 315, 316, 317, 320, 321, 322, 323,
		324, 325, 326, 327, 328, 329, 330, 331,
		332, 333, 334, 335, 336, 337, 338, 339,
		340, 341, 342, 343, 344, 345, 348, 349,
		350, 351, 352, 353, 354, 355, 356, 357,
		358, 359, 360, 361, 362, 363, 364, 365,
		366, 367, 368, 369, 370, 371, 372, 373,
		376, 377, 378, 379, 380, 381, 382, 383,
		384, 385, 386, 387, 388, 389, 390, 391,
		392, 393, 394, 395, 396, 397, 398, 399,
		400, 401, 404, 405, 406, 407, 408, 409,
		410, 411, 412, 413, 414, 415, 416, 417,
		418, 419, 420, 421, 422, 423, 424, 425,
		426, 427, 428, 429, 432, 433, 434, 435,
		436, 437, 438, 439, 440, 441, 442, 443,
		444, 445, 446, 447, 448, 449, 450, 451,
		452, 453, 454, 455, 456, 457, 460, 461,
		462, 463, 464, 465, 466, 467, 468, 469,
		470, 471, 472, 473, 474, 475, 476, 477,
		478, 479, 480, 481, 482, 483, 484, 485,
		488, 489, 490, 491, 492, 493, 494, 495,
		496, 497, 498, 499, 500, 501, 502, 503,
		504, 505, 506, 507, 508, 509, 510, 511,
		512, 513 },
	.oobfree = {
		{.offset = 4,
		.length = 32 } }
};

struct exynos_chip {
	unsigned int ecc_msg_len;
	unsigned int ecc_mode;
	unsigned int ecc_oobfree;
};

static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE];
static struct exynos_chip exynos_chip_info[CONFIG_SYS_MAX_NAND_DEVICE];
static struct exynos_nand *regs;

#if 0
static void exynos_nand_fast_iow(struct mtd_info *mtd, const uint8_t *buf, int len)
{
        __raw_writesl((unsigned long)&regs->nfdata, buf, len >> 2);
        __raw_writesb((unsigned long)&regs->nfdata, buf + (len & ~0x3), len & 3);
}

static void exynos_nand_fast_ior(struct mtd_info *mtd, uint8_t *buf, int len)
{
        __raw_readsl((unsigned long)&regs->nfdata, buf, len >> 2);
        __raw_readsb((unsigned long)&regs->nfdata, buf + (len & ~0x3), len & 3);
}
#endif

static int exynos_nand_read_page(struct mtd_info *mtd, struct nand_chip *chip,
		uint8_t *buf, int oob_required, int page)
{
	struct exynos_chip *exynos_chip = chip->priv;
	struct nand_ecclayout *layout = chip->ecc.layout;
	struct nand_oobfree *oobfree = &layout->oobfree[0];
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	int ecctotal = chip->ecc.total;
	uint8_t *p = buf;
	uint8_t *ecc_code = chip->buffers->ecccode;
	uint32_t *eccpos = layout->eccpos;
	uint8_t *ecc_calc = chip->buffers->ecccalc;

	/* Read the OOB area first */
	chip->cmdfunc(mtd, NAND_CMD_RNDOUT, mtd->writesize, page);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);
	chip->cmdfunc(mtd, NAND_CMD_RNDOUT, 0, page);

	for (i = 0; i < ecctotal; i++)
		ecc_code[i] = chip->oob_poi[eccpos[i]];

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		int stat;

		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);

		stat = chip->ecc.correct(mtd, p, &ecc_code[i], NULL);
		if (stat < 0)
			mtd->ecc_stats.failed++;
		else
			mtd->ecc_stats.corrected += stat;
	}

	/* correct first oobfree region, if possible */
	if ( exynos_chip->ecc_oobfree ) {
		int stat;
		int last_ecc_msg_len = exynos_chip->ecc_msg_len;

		exynos_chip->ecc_msg_len = oobfree->length;

		for (i = ecctotal; i < ecctotal + eccbytes; i++)
			ecc_code[i] = chip->oob_poi[eccpos[i]];

		p = chip->oob_poi + oobfree->offset;

		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->write_buf(mtd, p, oobfree->length);
		chip->ecc.calculate(mtd, p, &ecc_calc[ecctotal]);
		stat = chip->ecc.correct(mtd, p, &ecc_code[ecctotal], NULL);

		if (stat < 0)
			mtd->ecc_stats.failed++;
		else
			mtd->ecc_stats.corrected += stat;

		exynos_chip->ecc_msg_len = last_ecc_msg_len;
	}
	return 0;
}

static int exynos_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
		const uint8_t *buf, int oob_required)
{
	struct exynos_chip *exynos_chip = chip->priv;
	struct nand_ecclayout *layout = chip->ecc.layout;
	struct nand_oobfree *oobfree = &layout->oobfree[0];
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	int ecctotal = chip->ecc.total;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	const uint8_t *p = buf;
	uint32_t *eccpos = chip->ecc.layout->eccpos;

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, p, eccsize);
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);
	}

	for (i = 0; i < ecctotal; i++)
		chip->oob_poi[eccpos[i]] = ecc_calc[i];

	/* calculate ecc for first oobfree region, if possible */
	if ( exynos_chip->ecc_oobfree ) {
		int last_ecc_msg_len = exynos_chip->ecc_msg_len;
		exynos_chip->ecc_msg_len = oobfree->length;

		p = chip->oob_poi;
		chip->write_buf(mtd, p, oobfree->offset);

		p += oobfree->offset;
		chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, p, oobfree->length);
		chip->ecc.calculate(mtd, p, &ecc_calc[ecctotal]);

		for (i = ecctotal; i < ecctotal + eccbytes; i++)
			chip->oob_poi[eccpos[i]] = ecc_calc[i];

		p += oobfree->length;
		chip->write_buf(mtd, p, mtd->oobsize - (p - chip->oob_poi));

		exynos_chip->ecc_msg_len = last_ecc_msg_len;
	}
	else
		chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);

	return 0;
}

static void exynos_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	uint32_t tmp;

	if (ctrl & NAND_CTRL_CHANGE) {
		tmp = readl(&regs->nfcont);
		if (ctrl & NAND_NCE)
			tmp &= ~NFCONT_nFCE0;
		else
			tmp |= NFCONT_nFCE0;
		writel(tmp, &regs->nfcont);
	}

	if (cmd != NAND_CMD_NONE) {
		if (ctrl & NAND_CLE)
			writeb(cmd, &regs->nfcmd);
		if (ctrl & NAND_ALE)
			writeb(cmd, &regs->nfaddr);
	}
}

static int exynos_dev_ready(struct mtd_info *mtd)
{
	return readl(&regs->nfstat) & NFSTAT_RnB_PIN;
}

static void exynos_nand_hwecc_16bit(struct mtd_info *mtd, int mode)
{
	struct nand_chip *chip = mtd->priv;
	struct exynos_chip *exynos_chip = chip->priv;
	uint32_t tmp;

	exynos_chip->ecc_mode = mode;

	/* set data length and ecc strength */
	tmp = NFECCCONF_MSGLEN(exynos_chip->ecc_msg_len);
	switch (chip->ecc.strength) {
	case 8:
		tmp |= NFECCCONF_ECCTYPE_8BIT;
		break;
	case 12:
		tmp |= NFECCCONF_ECCTYPE_12BIT;
		break;
	case 16:
		tmp |= NFECCCONF_ECCTYPE_16BIT;
		break;
	}
	writel(tmp, &regs->nfeccconf);

	/* set encode/decode and initialize encoder/decoder */
	tmp = readl(&regs->nfecccont);
	if (mode == NAND_ECC_WRITE)
		tmp |= NFECCCONT_ENCODE;
	else
		tmp &= ~NFECCCONT_ENCODE;
	//tmp |= NFECCCONT_INITMECC;
	writel(tmp, &regs->nfecccont);

	/* clear RnB transition and illegal access bit */
	tmp = readl(&regs->nfstat);
	tmp |= NFSTAT_RnB_CHANGE | NFSTAT_ILLEGAL_ACCESS;
	writel(tmp, &regs->nfstat);

	/* clear encode and decode done flags */
	tmp = readl(&regs->nfeccstat);
	tmp |= NFECCSTAT_ENCDONE | NFECCSTAT_DECDONE;
	writel(tmp, &regs->nfeccstat);

	/* unlock ecc encode/decode */
	tmp = readl(&regs->nfcont);
	tmp &= ~NFCONT_MECCLOCK;
	writel(tmp, &regs->nfcont);

	/* initialize ecc encoder/decoder */
	tmp = readl(&regs->nfecccont);
	tmp |= NFECCCONT_INITMECC;
	writel(tmp, &regs->nfecccont);
}

static inline void exynos_nand_decode_ecc_16bit(struct mtd_info *mtd,
		uint8_t *read_ecc)
{
	struct nand_chip *chip = mtd->priv;
	int i = 0;

	/* write ecc */
	chip->write_buf(mtd, read_ecc, chip->ecc.bytes);

	/* start decoding */
	writel(readl(&regs->nfcont) | NFCONT_MECCLOCK,
			&regs->nfcont);

	/* wait for completion */
	while (!(readl(&regs->nfeccstat) & NFECCSTAT_DECDONE)) {
		if (i++ >= 100) {
			printf("ecc decoder timeout\n");
			break;
		}
		udelay(1);
	}

	/* clear bit */
	writel(readl(&regs->nfeccstat) | NFECCSTAT_DECDONE,
			&regs->nfeccstat);
}

static inline void exynos_nand_encode_ecc_16bit(struct mtd_info *mtd)
{
	int i = 0;

	/* start encoding */
	writel(readl(&regs->nfcont) | NFCONT_MECCLOCK,
			&regs->nfcont);

	/* wait for completion */
	while (!(readl(&regs->nfeccstat) & NFECCSTAT_ENCDONE)) {
		if (i++ >= 100) {
			printf("ecc encoder timeout\n");
			break;
		}
		udelay(1);
	}

	/* clear bit */
	writel(readl(&regs->nfeccstat) | NFECCSTAT_ENCDONE,
			&regs->nfeccstat);
}

static int exynos_nand_calculate_16bit(struct mtd_info *mtd, const uint8_t *dat,
		uint8_t *ecc_calc)
{
	struct nand_chip *chip = mtd->priv;
	struct exynos_chip *exynos_chip = chip->priv;
	uint32_t buf[7];
	int i;

	if (exynos_chip->ecc_mode == NAND_ECC_WRITE) {
		exynos_nand_encode_ecc_16bit(mtd);
		for (i = 0; i < (chip->ecc.bytes + 3) / 4; i++)
			buf[i] = readl(&regs->nfeccprgecc[i]);
		memcpy(ecc_calc, buf, chip->ecc.bytes);
	}
	return 0;
}

static int exynos_nand_correct_16bit(struct mtd_info *mtd, uint8_t *dat,
		uint8_t *read_ecc, uint8_t *calc_ecc)
{
	struct nand_chip *chip = mtd->priv;
	struct exynos_chip *exynos_chip = chip->priv;
	int strength = chip->ecc.strength;
	int i, err_count;
	uint32_t eccerl[8];
	uint32_t eccerp[4];
	uint16_t *err_loc = (uint16_t *)eccerl;
	uint8_t *err_pat = (uint8_t *)eccerp;

	exynos_nand_decode_ecc_16bit(mtd, read_ecc);

	err_count = readl(&regs->nfeccsecstat) & 0x1f;
	if (err_count == 0)
		return 0;

	if (err_count > strength)
		return -1;

	for (i = 0; i < (err_count + 1) / 2; i++)
		eccerl[i] = readl(regs->nfeccerl);

	for (i = 0; i < (err_count + 3) / 4; i++)
		eccerp[i] = readl(regs->nfeccerp);

	for (i = 0; i < err_count; i++) {
		if (err_loc[i] >= exynos_chip->ecc_msg_len)
			continue;
		dat[err_loc[i]] ^= err_pat[i];
	}

	return err_count;
}

static int exynos_hwecc_init_param(struct nand_chip *chip, struct mtd_info *mtd)
{
	chip->ecc.mode = 0;

	switch (mtd->oobsize) {
	case 512:
		chip->ecc.layout = &nand_oob_512;
		break;
	case 640:
		chip->ecc.layout = &nand_oob_640;
		break;
	}

	switch (mtd->writesize) {
	case 8192:
		chip->ecc.mode = NAND_ECC_HW_OOB_FIRST;
		chip->ecc.size = 512;
		chip->ecc.bytes = 26;
		chip->ecc.strength = 16;
		chip->ecc.hwctl = exynos_nand_hwecc_16bit;
		chip->ecc.calculate = exynos_nand_calculate_16bit;
		chip->ecc.correct = exynos_nand_correct_16bit;
		chip->ecc.read_page = exynos_nand_read_page;
		chip->ecc.write_page = exynos_nand_write_page;
		break;
	}

	/* fall back to software ECC */
	if (!chip->ecc.mode) {
		chip->ecc.mode = NAND_ECC_SOFT;
		chip->ecc.size = 0;
		chip->ecc.strength = 0;
		chip->ecc.prepad = 0;
		chip->ecc.postpad = 0;
		chip->ecc.layout = 0;
		chip->ecc.hwctl = NULL;
		chip->ecc.calculate = NULL;
		chip->ecc.correct = NULL;
		chip->ecc.read_page = NULL;
		chip->ecc.write_page = NULL;
	}

	return 0;
}

static int exynos_nand_init(int devnum, ulong base_addr)
{
	struct mtd_info *mtd = &nand_info[devnum];
	struct nand_chip *chip = &nand_chip[devnum];
	struct exynos_chip *exynos_chip = &exynos_chip_info[devnum];
	uint32_t cfg;
	int ret;

	regs = (void *)base_addr;

	mtd->priv = chip;
	chip->priv = exynos_chip;

#ifdef CONFIG_SYS_NAND_USE_FLASH_BBT
	chip->bbt_options	  |= NAND_BBT_USE_FLASH;
#endif
	chip->IO_ADDR_R = (void __iomem *)&regs->nfdata;
	chip->IO_ADDR_W = (void __iomem *)&regs->nfdata;

	chip->cmd_ctrl = exynos_hwcontrol;
	chip->dev_ready = exynos_dev_ready;
        //chip->read_buf = exynos_nand_fast_ior;
        //chip->write_buf = exynos_nand_fast_iow;

	/* initialize hardware */
	cfg = NFCONF_ECC_NONE;
	cfg |= NFCONF_TACLS(4);
	cfg |= NFCONF_TWRPH0(8);
	cfg |= NFCONF_TWRPH1(8);
	cfg |= 0xA;
	writel(cfg, &regs->nfconf);

	cfg = NFCONT_nFCE3;
	cfg |= NFCONT_nFCE2;
	cfg |= NFCONT_nFCE1;
	cfg |= NFCONT_nFCE0;
	cfg |= NFCONT_EN;
	writel(cfg, &regs->nfcont);

	ret = nand_scan_ident(mtd, CONFIG_SYS_NAND_MAX_CHIPS, NULL);
	if (ret)
		return ret;

	ret = exynos_hwecc_init_param(chip, mtd);
	if (ret)
		return ret;

	ret = nand_scan_tail(mtd);
	if (ret)
		return ret;

	exynos_chip->ecc_msg_len = chip->ecc.size;
	if (chip->ecc.layout) {
		struct nand_ecclayout *layout = chip->ecc.layout;
		int ecctotal = chip->ecc.total;
		int eccbytes = chip->ecc.bytes;

		if ((layout->oobfree->length) &&
				(layout->eccbytes >= ecctotal + eccbytes))
			exynos_chip->ecc_oobfree = 1;
	}

	ret = nand_register(devnum);
	return ret;
}

#ifndef CONFIG_SYS_NAND_BASE_LIST
#define CONFIG_SYS_NAND_BASE_LIST { CONFIG_SYS_NAND_BASE }
#endif
static ulong base_addr[CONFIG_SYS_MAX_NAND_DEVICE] = CONFIG_SYS_NAND_BASE_LIST;

/* Only one controller is supported */
void board_nand_init(void)
{
	if (exynos_nand_init(0, base_addr[0]))
		dev_err(host->dev, "exynos_nand: Failed to initialize.");
}
