/*
 * drivers/spi/spi-sun7i.c
 * Copyright (C) 2012 - 2016 Reuuimlla Limited
 * Pan Nan <pannan@reuuimllatech.com>
 * James Deng <csjamesdeng@reuuimllatech.com>
 *
 * SUN7I SPI Controller Driver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <mach/dma.h>
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <plat/sys_config.h>
#include <mach/spi.h>

#define SPI_DEBUG_LEVEL 2

#if (SPI_DEBUG_LEVEL == 1)
    #define spi_dbg(format,args...)     do {} while (0)
    #define spi_inf(format,args...)     do {} while (0)
    #define spi_err(format,args...)     printk(KERN_ERR "[spi-err] "format,##args)
#elif (SPI_DEBUG_LEVEL == 2)
    #define spi_dbg(format,args...)     do {} while (0)
    #define spi_inf(format,args...)     printk(KERN_INFO"[spi-inf] "format,##args)
    #define spi_err(format,args...)     printk(KERN_ERR "[spi-err] "format,##args)
#elif (SPI_DEBUG_LEVEL == 3)
    #define spi_dbg(format,args...)     printk(KERN_INFO"[spi-dbg] "format,##args)
    #define spi_inf(format,args...)     printk(KERN_INFO"[spi-inf] "format,##args)
    #define spi_err(format,args...)     printk(KERN_ERR "[spi-err] "format,##args)
#endif

enum spi_duplex_flag {
    HALF_DUPLEX_RX,                     // half duplex read
    HALF_DUPLEX_TX,                     // half duplex write
    FULL_DUPLEX_RX_TX,                  // full duplex read and write
    DUPLEX_NULL,
};

enum spi_dma_dir {
    SPI_DMA_NULL = 0,
    SPI_DMA_RDEV = 1,
    SPI_DMA_WDEV = 2,
};

#define SYS_SPI_PIN
#ifndef SYS_SPI_PIN
static void* __iomem gpio_addr = NULL;

/* gpio base address */
#define _PIO_BASE_ADDRESS    (0x01c20800)

/* gpio spi1 */
#define _Pn_CFG1(n) ( (n)*0x24 + 0x04 + gpio_addr )
#define _Pn_DRV1(n) ( (n)*0x24 + 0x14 + gpio_addr )
#define _Pn_PUL1(n) ( (n)*0x24 + 0x1C + gpio_addr )
#endif

struct sun7i_spi {
    struct platform_device *pdev;
    struct spi_master *master;          /* kzalloc */
    void __iomem *base_addr;            /* register */
    struct clk *hclk;                   /* ahb spi gating bit */
    struct clk *mclk;                   /* ahb spi gating bit */
    unsigned long gpio_hdle;
    dma_chan_type_e dma_type;
    int dma_id_tx;
    int dma_id_rx;
    dma_hdl_t dma_hdle_tx;
    dma_hdl_t dma_hdle_rx;
    enum spi_duplex_flag duplex_flag;
    unsigned int irq;                   /* irq NO. */
    int busy;
#define SPI_FREE   (1<<0)
#define SPI_SUSPND (1<<1)
#define SPI_BUSY   (1<<2)
    int result;                         /* 0: succeed, -1: fail */
    struct workqueue_struct *workqueue;
    struct work_struct work;
    struct list_head queue;             /* spi messages */
    spinlock_t lock;
    struct completion done;             /* wakup another spi transfer */
    /* keep select during one message */
    void (*cs_control)(struct spi_device *spi, bool on);
    /* (1) enable cs1,    cs_bitmap = SPI_CHIP_SELECT_CS1;
     * (2) enable cs0&cs1,cs_bitmap = SPI_CHIP_SELECT_CS0|SPI_CHIP_SELECT_CS1;
     */
#define SPI_CHIP_SELECT_CS0 (0x01)
#define SPI_CHIP_SELECT_CS1 (0x02)
    int cs_bitmap;                      /* cs0- 0x1, cs1-0x2, cs0 & cs1-0x3. */
};

/* config chip select */
s32 spi_set_cs(u32 chipselect, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    if (chipselect < 4) {
        reg_val &= ~SPI_CTL_SS_MASK;/* SS-chip select, clear two bits */
        reg_val |= chipselect << SPI_SS_BIT_POS;/* set chip select */

        writel(reg_val, base_addr + SPI_CTL_REG);

        return 0;
    } else {
        spi_err("%s: chip select set fail, cs = %d\n", __func__, chipselect);
        return -1;
    }
}

/* select dma type */
s32 spi_sel_dma_type(u32 dma_type, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    reg_val &= ~SPI_CTL_DMAMOD;
    if (dma_type) {
        reg_val |= 1 << 5;
    }
    writel(reg_val, base_addr + SPI_CTL_REG);

    return 0;
}

/* config spi */
void spi_config(u32 master, u32 config, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    /* 1. POL */
    if (config & SPI_POL_ACTIVE_) {
        reg_val |= SPI_CTL_POL;
    } else {
        reg_val &= ~SPI_CTL_POL; /* default POL = 0 */
    }

    /*2. PHA */
    if (config & SPI_PHA_ACTIVE_) {
        reg_val |= SPI_CTL_PHA;
    } else {
        reg_val &= ~SPI_CTL_PHA; /* default PHA = 0 */
    }

    /* 3. SSPOL, chip select signal polarity */
    if (config & SPI_CS_HIGH_ACTIVE_) {
        reg_val &= ~SPI_CTL_SSPOL;
    } else {
        reg_val |= SPI_CTL_SSPOL; /* default SSPOL = 1,Low level effective */
    }

    /* 4. LMTF-LSB/MSB transfer first select */
    if (config & SPI_LSB_FIRST_ACTIVE_) {
        reg_val |= SPI_CTL_LMTF;
    } else {
        reg_val &= ~SPI_CTL_LMTF; /* default LMTF = 0, MSB first */
    }

    /* master mode: set DDB,DHB,SMC,SSCTL */
    if (master == 1) {
        /* 5. dummy burst type */
        if (config & SPI_DUMMY_ONE_ACTIVE_) {
            reg_val |= SPI_CTL_DDB;
        } else {
            reg_val &= ~SPI_CTL_DDB; /* default DDB =0, ZERO */
        }

        /* 6. discard hash burst-DHB */
        if (config & SPI_RECEIVE_ALL_ACTIVE_) {
            reg_val &= ~SPI_CTL_DHB;
        } else {
            reg_val |= SPI_CTL_DHB; /* default DHB =1, discard unused burst */
        }

        /* 7. set SMC = 1, SSCTL = 0, TPE = 1 */
        reg_val &= ~SPI_CTL_SSCTL;
        reg_val |=  SPI_CTL_T_PAUSE_EN;
    } else {
        /* tips for slave mode config */
        spi_inf("%s: slave mode configurate control register\n", __func__);
    }

    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* restore reg data after transfer complete */
void spi_restore_state(u32 master, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    /*
     * config spi control register
     * | 15 |  14  |  13  |  12  |  11  |  10  |  9  |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
     * | DHB|  DDB |     SS      | SMC  |  XCH |TFRST|RFRST|SSCTL| MSB | TBW |SSPOL| POL | PHA | MOD | EN  |
     * | 1  |   0  |     00      |  1   |   0  |  0  |  0  |  0  |  0  |  0  |  1  |  1  |  1  |  1  |  1  |
     */
    /* master mode */
    if (master) {
        reg_val |= (SPI_CTL_DHB | SPI_CTL_SSPOL | SPI_CTL_POL | SPI_CTL_PHA | SPI_CTL_FUNC_MODE | SPI_CTL_EN);

        /* new bit,transmit pause enable,stop smart dummy when rxfifo full */
        reg_val |= SPI_CTL_T_PAUSE_EN;

        /* |SPI_CTL_TBW); //deleted SPI_CTL_TBW bit for aw1623, this bit is defined for dma mode select, 2011-5-26 19:55:32 */
        reg_val &= ~(SPI_CTL_DDB | SPI_CTL_SS_MASK | SPI_CTL_XCH | SPI_CTL_SSCTL | SPI_CTL_LMTF);
    } else { /* slave mode */
        reg_val |= (SPI_CTL_SSPOL | SPI_CTL_POL | SPI_CTL_PHA || SPI_CTL_EN);

        /* |SPI_CTL_TBW); //deleted SPI_CTL_TBW bit for aw1623, this bit is defined for dma mode select, 2011-5-26 19:55:32 */
        reg_val &= ~(SPI_CTL_DHB | SPI_CTL_FUNC_MODE | SPI_CTL_DDB | SPI_CTL_SS_MASK | SPI_CTL_XCH | SPI_CTL_SSCTL | SPI_CTL_LMTF);
    }

    spi_dbg("control register set default value: 0x%08x\n", reg_val);
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* set spi clock */
void spi_set_clk(u32 spi_clk, u32 ahb_clk, void *base_addr)
{
    u32 reg_val = 0;
    u32 N = 0;
    u32 div_clk = (ahb_clk >> 1) / spi_clk;

    reg_val = readl(base_addr + SPI_CLK_RATE_REG);

    if (div_clk <= SPI_CLK_SCOPE) {
        if (div_clk != 0) {
            div_clk--;
        }

        reg_val &= ~SPI_CLKCTL_CDR2;
        reg_val |= (div_clk | SPI_CLKCTL_DRS);
    } else {
        while (1) {
            if (div_clk == 1) {
                break;
            }
            div_clk >>= 1;
            N++;
        };

        reg_val &= ~(SPI_CLKCTL_CDR1 | SPI_CLKCTL_DRS);
        reg_val |= (N << 8);
    }

    writel(reg_val, base_addr + SPI_CLK_RATE_REG);
}

/* start spi transfer */
void spi_start_xfer(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val |= SPI_CTL_XCH;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* query tranfer is completed in smc mode */
u32 spi_query_xfer(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    return (reg_val & SPI_CTL_XCH);
}

/* enable spi bus */
void spi_enable_bus(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val |= SPI_CTL_EN;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* disbale spi bus */
void spi_disable_bus(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val &= ~SPI_CTL_EN;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* set master mode */
void spi_set_master(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val |= SPI_CTL_FUNC_MODE;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* set slave mode */
void spi_set_slave(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val &= ~SPI_CTL_FUNC_MODE;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* disable irq type */
void spi_disable_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_INT_CTL_REG);
    bitmap &= SPI_INTEN_MASK;
    reg_val &= ~bitmap;
    writel(reg_val, base_addr + SPI_INT_CTL_REG);
}

/* enable irq type */
void spi_enable_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_INT_CTL_REG);
    bitmap &= SPI_INTEN_MASK;
    reg_val |= bitmap;
    writel(reg_val, (base_addr + SPI_INT_CTL_REG));
}

/* disable dma irq */
void spi_disable_dma_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_DMA_CTL_REG);
    bitmap &= SPI_DRQEN_MASK;
    reg_val &= ~bitmap;
    writel(reg_val, base_addr + SPI_DMA_CTL_REG);
}

/* enable dma irq */
void spi_enable_dma_irq(u32 bitmap, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_DMA_CTL_REG);
    bitmap &= SPI_DRQEN_MASK;
    reg_val |= bitmap;
    writel(reg_val, base_addr + SPI_DMA_CTL_REG);
}

/* query irq pending */
u32 spi_qry_irq_pending(void *base_addr)
{
    return (SPI_STAT_MASK & readl(base_addr + SPI_STATUS_REG));
}

/* clear irq pending */
void spi_clr_irq_pending(u32 pending_bit, void *base_addr)
{
    pending_bit &= SPI_STAT_MASK;
    writel(pending_bit, base_addr + SPI_STATUS_REG);
}

/* query txfifo bytes */
u32 spi_query_txfifo(void *base_addr)
{
    u32 reg_val = (SPI_FIFO_TXCNT & readl(base_addr + SPI_FIFO_STA_REG));
    reg_val >>= SPI_TXCNT_BIT_POS;
    return reg_val;
}

/* query rxfifo bytes */
u32 spi_query_rxfifo(void *base_addr)
{
    u32 reg_val = (SPI_FIFO_RXCNT & readl(base_addr + SPI_FIFO_STA_REG));
    reg_val >>= SPI_RXCNT_BIT_POS;
    return reg_val;
}

/* reset fifo */
void spi_reset_fifo(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val |= (SPI_CTL_RST_RXFIFO | SPI_CTL_RST_TXFIFO);
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* set transfer total length BC and transfer length WTC */
void spi_set_bc_wtc(u32 tx_len, u32 rx_len, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_BC_REG);
    reg_val &= ~SPI_BC_BC_MASK;
    reg_val |= (SPI_BC_BC_MASK & (tx_len + rx_len));
    writel(reg_val, base_addr + SPI_BC_REG);
	spi_dbg("%s: bc: %u\n", __func__, readl(base_addr + SPI_BC_REG));

    reg_val = readl(base_addr + SPI_TC_REG);
    reg_val &= ~SPI_TC_WTC_MASK;
    reg_val |= (SPI_TC_WTC_MASK & tx_len);
    writel(reg_val, base_addr + SPI_TC_REG);
	spi_dbg("%s: tc: %u\n", __func__, readl(base_addr + SPI_TC_REG));
}

/* set ss control */
void spi_ss_ctrl(void *base_addr, u32 on_off)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    on_off &= 0x1;
    if (on_off) {
        reg_val |= SPI_CTL_SS_CTRL;
    } else {
        reg_val &= ~SPI_CTL_SS_CTRL;
    }

    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* set ss level */
void spi_ss_level(void *base_addr, u32 hi_lo)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);

    hi_lo &= 0x1;
    if (hi_lo) {
        reg_val |= SPI_CTL_SS_LEVEL;
    } else {
        reg_val &= ~SPI_CTL_SS_LEVEL;
    }

    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* set wait clock counter */
void spi_set_waitclk_cnt(u32 waitclk_cnt, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_WAIT_REG);
    reg_val &= ~SPI_WAIT_CLK_MASK;
    waitclk_cnt &= SPI_WAIT_CLK_MASK;
    reg_val |= waitclk_cnt;
    writel(reg_val, base_addr + SPI_WAIT_REG);
}

/* set sample delay in high speed mode */
void spi_set_sample_delay(u32 on_off, void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val &= ~SPI_CTL_MASTER_SDC;
    reg_val |= on_off;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

/* keep unused burst */
void spi_clear_dhb(void *base_addr)
{
    u32 reg_val = readl(base_addr + SPI_CTL_REG);
    reg_val &= ~SPI_CTL_DHB;
    writel(reg_val, base_addr + SPI_CTL_REG);
}

static int sun7i_spi_get_cfg_csbitmap(int bus_num);

/* flush d-cache */
static void sun7i_spi_cleanflush_dcache_region(void *addr, __u32 len)
{
    __cpuc_flush_dcache_area(addr, len/*len + (1 << 5) * 2 - 2*/);
}

static char *spi_dma_rx[] = {"spi0_rx", "spi1_rx", "spi2_rx", "spi3_rx"};
static char *spi_dma_tx[] = {"spi0_tx", "spi1_tx", "spi2_tx", "spi3_tx"};

static void sun7i_spi_dma_cb_rx(dma_hdl_t dma_hdl, void *parg)
{
    struct sun7i_spi *aw_spi = (struct sun7i_spi *)parg;
    unsigned long flags;

    spi_dbg("%s: spi%d dma read data callback\n", __func__, aw_spi->master->bus_num);

    spin_lock_irqsave(&aw_spi->lock, flags);
    spi_disable_dma_irq(SPI_DRQEN_RR, aw_spi->base_addr);
    spin_unlock_irqrestore(&aw_spi->lock, flags);
}

static void sun7i_spi_dma_cb_tx(dma_hdl_t dma_hdl, void *parg)
{
    struct sun7i_spi *aw_spi = (struct sun7i_spi *)parg;
    unsigned long flags;

    spi_dbg("%s: spi%d dma write data callback\n", __func__, aw_spi->master->bus_num);

    spin_lock_irqsave(&aw_spi->lock, flags);
    spi_disable_dma_irq(SPI_DRQEN_TE, aw_spi->base_addr);
    spin_unlock_irqrestore(&aw_spi->lock, flags);
}

/* request dma channel and set callback function */
static int sun7i_spi_prepare_dma(struct sun7i_spi *aw_spi, enum spi_dma_dir dma_dir)
{
    int ret = 0;
    int bus_num = aw_spi->master->bus_num;
    dma_cb_t done_cb;

	spi_dbg("%s: enter\n", __func__);
    if (SPI_DMA_RDEV == dma_dir) {
        aw_spi->dma_hdle_rx = sw_dma_request(spi_dma_rx[bus_num], aw_spi->dma_type);
        if (!aw_spi->dma_hdle_rx) {
            spi_err("%s: spi%d request dma rx failed\n", __func__, bus_num);
            return -EINVAL;
        }

        done_cb.func = sun7i_spi_dma_cb_rx;
        done_cb.parg = aw_spi;
        ret = sw_dma_ctl(aw_spi->dma_hdle_rx, DMA_OP_SET_FD_CB, (void *)&done_cb);
    } else if (SPI_DMA_WDEV == dma_dir) {
        aw_spi->dma_hdle_tx = sw_dma_request(spi_dma_tx[bus_num], aw_spi->dma_type);
        if (!aw_spi->dma_hdle_tx) {
            spi_err("%s: spi%d request dma tx failed\n", __func__, bus_num);
            return -EINVAL;
        }
 
        done_cb.func = sun7i_spi_dma_cb_tx;
        done_cb.parg = aw_spi;
        ret = sw_dma_ctl(aw_spi->dma_hdle_tx, DMA_OP_SET_FD_CB, (void *)&done_cb);
    } else {
        return -1;
    }

    return ret;
}

static int sun7i_spi_config_dma(struct sun7i_spi *aw_spi, enum spi_dma_dir dma_dir, void *buf, unsigned int len)
{
    int bus_num = aw_spi->master->bus_num;
    unsigned char spi_rx_ndrq[] = {N_SRC_SPI0_RX, N_SRC_SPI1_RX, N_SRC_SPI2_RX, N_SRC_SPI3_RX};
    unsigned char spi_tx_ndrq[] = {N_DST_SPI0_TX, N_DST_SPI1_TX, N_DST_SPI2_TX, N_DST_SPI3_TX};
    unsigned char spi_rx_ddrq[] = {D_SRC_SPI0_RX, D_SRC_SPI1_RX, D_SRC_SPI2_RX, D_SRC_SPI3_RX};
    unsigned char spi_tx_ddrq[] = {D_DST_SPI0_TX, D_DST_SPI1_TX, D_DST_SPI2_TX, D_DST_SPI3_TX};
    unsigned long spi_phyaddr[] = {SPI0_BASE_ADDR, SPI1_BASE_ADDR, SPI2_BASE_ADDR, SPI3_BASE_ADDR}; /* physical address */
    dma_config_t spi_hw_conf;
    dma_hdl_t dma_hdle;
    u32 src_addr, dst_addr;
    u32 security = SRC_SECU_DST_SECU;

	spi_dbg("%s: enter\n", __func__);
    spi_hw_conf.xfer_type.src_data_width    = DATA_WIDTH_8BIT;
    spi_hw_conf.xfer_type.src_bst_len       = DATA_BRST_1;
    spi_hw_conf.xfer_type.dst_data_width    = DATA_WIDTH_8BIT;
    spi_hw_conf.xfer_type.dst_bst_len       = DATA_BRST_1;
    spi_hw_conf.bconti_mode                 = false;
    spi_hw_conf.irq_spt                     = CHAN_IRQ_FD;
    switch (aw_spi->dma_type) {
        case CHAN_NORMAL:
            if (SPI_DMA_RDEV == dma_dir) {
                spi_hw_conf.address_type.src_addr_mode  = NDMA_ADDR_NOCHANGE;
                spi_hw_conf.address_type.dst_addr_mode  = NDMA_ADDR_INCREMENT;
                spi_hw_conf.src_drq_type                = spi_rx_ndrq[bus_num];
                spi_hw_conf.dst_drq_type                = N_DST_SDRAM;
            } else if (SPI_DMA_WDEV == dma_dir) {
                spi_hw_conf.address_type.src_addr_mode  = NDMA_ADDR_INCREMENT;
                spi_hw_conf.address_type.dst_addr_mode  = NDMA_ADDR_NOCHANGE;
                spi_hw_conf.src_drq_type                = N_SRC_SDRAM;
                spi_hw_conf.dst_drq_type                = spi_tx_ndrq[bus_num];
            } else {
                return -1;
            }
            break;

        case CHAN_DEDICATE:
            if (SPI_DMA_RDEV == dma_dir) {
                spi_hw_conf.address_type.src_addr_mode  = DDMA_ADDR_IO;
                spi_hw_conf.address_type.dst_addr_mode  = DDMA_ADDR_LINEAR;
                spi_hw_conf.src_drq_type                = spi_rx_ddrq[bus_num];
                spi_hw_conf.dst_drq_type                = D_DST_SDRAM;
            } else if (SPI_DMA_WDEV == dma_dir) {
                spi_hw_conf.address_type.src_addr_mode  = DDMA_ADDR_LINEAR;
                spi_hw_conf.address_type.dst_addr_mode  = DDMA_ADDR_IO;
                spi_hw_conf.src_drq_type                = D_SRC_SDRAM;
                spi_hw_conf.dst_drq_type                = spi_tx_ddrq[bus_num];
            } else {
                return -1;
            }
            break;
    }

    if (SPI_DMA_RDEV == dma_dir) {
        dma_hdle = aw_spi->dma_hdle_rx;
        src_addr = spi_phyaddr[bus_num] + SPI_RXDATA_REG;
        dst_addr = virt_to_phys(buf);
    } else if (SPI_DMA_WDEV == dma_dir) {
        dma_hdle = aw_spi->dma_hdle_tx;
        src_addr = virt_to_phys(buf);
        dst_addr = spi_phyaddr[bus_num] + SPI_TXDATA_REG;
    } else {
        return -1;
    }

    /* set src, dst, drq type, configuration */
    if (sw_dma_config(dma_hdle, &spi_hw_conf)) {
        spi_err("%s: spi%d config failed\n", __func__, bus_num);
        return -1;
    }

    if (sw_dma_ctl(dma_hdle, DMA_OP_SET_SECURITY, &security)) {
        spi_err("%s: spi%d set dma SRC_SECU_DST_SECU failed\n", __func__,
                bus_num);
        return -1;
    }

    if (CHAN_DEDICATE == aw_spi->dma_type) {
        dma_para_t para = {
            .src_wait_cyc = 0x07,
            .src_blk_sz   = 0x1f,
            .dst_wait_cyc = 0x07,
            .dst_blk_sz   = 0x1f
        };

        if (sw_dma_ctl(dma_hdle, DMA_OP_SET_PARA_REG, &para)) {
            spi_err("%s: spi%d set dma para failed\n", __func__, bus_num);
            return -1;
        }
    }

    /* 1. flush d-cache */
    sun7i_spi_cleanflush_dcache_region((void *)buf, len);

    /* 2. enqueue dma transfer */
    return sw_dma_enqueue(dma_hdle, src_addr, dst_addr, len);
}

/* set dma start flag, if queue, it will auto restart to transfer next queue */
static int sun7i_spi_start_dma(struct sun7i_spi *aw_spi, enum spi_dma_dir dma_dir)
{
    int ret = 0;

	spi_dbg("%s: enter\n", __func__);
    if (SPI_DMA_RDEV == dma_dir) {
        ret = sw_dma_ctl(aw_spi->dma_hdle_rx, DMA_OP_START, NULL);
    } else if (SPI_DMA_WDEV == dma_dir) {
        ret = sw_dma_ctl(aw_spi->dma_hdle_tx, DMA_OP_START, NULL);
    } else {
        return -1;
    }

    return ret;
}

/* release dma channel, and set queue status to idle. */
static int sun7i_spi_release_dma(struct sun7i_spi *aw_spi, enum spi_dma_dir dma_dir)
{
    int ret = 0;
    unsigned long flags;

	spi_dbg("%s: enter\n", __func__);
    if (SPI_DMA_RDEV == dma_dir) {
        spin_lock_irqsave(&aw_spi->lock, flags);
        ret  = sw_dma_ctl(aw_spi->dma_hdle_rx, DMA_OP_STOP, NULL);
        ret += sw_dma_release(aw_spi->dma_hdle_rx);
        aw_spi->dma_hdle_rx = NULL;
        spin_unlock_irqrestore(&aw_spi->lock, flags);
    } else if (SPI_DMA_WDEV == dma_dir) {
        spin_lock_irqsave(&aw_spi->lock, flags);
        ret  = sw_dma_ctl(aw_spi->dma_hdle_tx, DMA_OP_STOP, NULL);
        ret += sw_dma_release(aw_spi->dma_hdle_tx);
        aw_spi->dma_hdle_tx = NULL;
        spin_unlock_irqrestore(&aw_spi->lock, flags);
    } else {
        return -1;
    }

    return ret;
}

/* check the valid of cs id */
static int sun7i_spi_check_cs(int cs_id, struct sun7i_spi *aw_spi)
{
    int ret = -1;
    switch (cs_id) {
        case 0:
            ret = (aw_spi->cs_bitmap & SPI_CHIP_SELECT_CS0) ? 0 : -1;
            break;
        case 1:
            ret = (aw_spi->cs_bitmap & SPI_CHIP_SELECT_CS1) ? 0 : -1;
            break;
        default:
            spi_err("%s: spi%d chip select not support, cs = %d\n",
                    __func__, aw_spi->master->bus_num, cs_id);
            break;
    }

    return ret;
}

/* spi device on or off control */
static void sun7i_spi_cs_control(struct spi_device *spi, bool on)
{
    struct sun7i_spi *aw_spi = spi_master_get_devdata(spi->master);
    unsigned int cs = 0;
    if (aw_spi->cs_control) {
        if (on) {
            /* set active */
            cs = (spi->mode & SPI_CS_HIGH) ? 1 : 0;
        } else {
            /* set inactive */
            cs = (spi->mode & SPI_CS_HIGH) ? 0 : 1;
        }
        spi_ss_level(aw_spi->base_addr, cs);
    }
}

/*
 * change the properties of spi device with spi transfer.
 * every spi transfer must call this interface to update the master to the excute transfer
 * set clock frequecy, bits per word, mode etc...
 * return:  >= 0 : succeed;    < 0: failed.
 */
static int sun7i_spi_xfer_setup(struct spi_device *spi, struct spi_transfer *t)
{
    /* get at the setup function, the properties of spi device */
    struct sun7i_spi *aw_spi = spi_master_get_devdata(spi->master);
    struct sun7i_spi_config *config = spi->controller_data; // allocate in the setup, and free in the cleanup
    void *__iomem base_addr = aw_spi->base_addr;

    spi_dbg("%s: enter\n", __func__);
    config->max_speed_hz  = (t && t->speed_hz) ? t->speed_hz : spi->max_speed_hz;
    config->bits_per_word = (t && t->bits_per_word) ? t->bits_per_word : spi->bits_per_word;
    config->bits_per_word = ((config->bits_per_word + 7) / 8) * 8;

    if (config->bits_per_word != 8) {
        spi_err("%s: spi%d just support 8bits per word\n", __func__, spi->master->bus_num);
        return -EINVAL;
    }

    if (spi->chip_select >= spi->master->num_chipselect) {
        spi_err("%s: spi%d device's chip select = %d exceeds the master supported cs_num[%d] \n",
                __func__, spi->master->bus_num, spi->chip_select, spi->master->num_chipselect);
        return -EINVAL;
    }

    /* check again board info */
    if (sun7i_spi_check_cs(spi->chip_select, aw_spi)) {
        spi_err("%s: sun7i_spi_check_cs failed, spi_device cs =%d\n", __func__, spi->chip_select);
        return -EINVAL;
    }

    /* set cs */
    spi_set_cs(spi->chip_select, base_addr);
    /*
     *  master: set spi module clock;
     *  set the default frequency   10MHz
     */
    spi_set_master(base_addr);
    if (config->max_speed_hz > SPI_MAX_FREQUENCY) {
        return -EINVAL;
    }

#ifndef CONFIG_AW_FPGA_PLATFORM
    spi_set_clk(config->max_speed_hz, clk_get_rate(aw_spi->mclk), base_addr);
#else
    spi_set_clk(config->max_speed_hz, 24000000, base_addr);
#endif

    /*
     *  master : set POL,PHA,SSOPL,LMTF,DDB,DHB; default: SSCTL=0,SMC=1,TBW=0.
     *  set bit width-default: 8 bits
     */
    spi_config(1, spi->mode, base_addr);

    return 0;
}

/*
 * <= 64 : cpu ;  > 64 : dma
 * wait for done completion in this function, wakup in the irq hanlder
 */
static int sun7i_spi_xfer(struct spi_device *spi, struct spi_transfer *t)
{
    struct sun7i_spi *aw_spi = spi_master_get_devdata(spi->master);
    void __iomem *base_addr = aw_spi->base_addr;
    unsigned long flags = 0;
    unsigned tx_len = t->len;
    unsigned rx_len = t->len;
    unsigned char *rx_buf = (unsigned char *)t->rx_buf;
    unsigned char *tx_buf = (unsigned char *)t->tx_buf;
    enum spi_dma_dir dma_dir = SPI_DMA_NULL;
    int ret = 0;

    spi_dbg("%s: spi%d begin transfer, txbuf %p, rxbuf %p, len %d\n",
            __func__, spi->master->bus_num, tx_buf, rx_buf, t->len);

    if ((!t->tx_buf && !t->rx_buf) || !t->len) {
        spi_err("%s: spi%d invalid buffer or len\n", __func__, spi->master->bus_num);
        return -EINVAL;
    }

    /* write 1 to clear 0 */
    spi_clr_irq_pending(SPI_STAT_MASK, base_addr);
    /* disable all DRQ */
    spi_disable_dma_irq(SPI_DRQEN_MASK, base_addr);
    /* reset tx/rx fifo */
    spi_reset_fifo(base_addr);

    if (aw_spi->duplex_flag != DUPLEX_NULL) {
        spi_err("%s: spi%d duplex flag not null\n", __func__, spi->master->bus_num);
        return -EINVAL;
    }

    /* set the Burst Counter and Write Transmit Counter, auto put dummy data into the txFIFO! */
    /* check if use full duplex or half duplex */
    if (tx_buf && rx_buf) {
        spin_lock_irqsave(&aw_spi->lock, flags);
        aw_spi->duplex_flag = FULL_DUPLEX_RX_TX;
        spi_set_bc_wtc(tx_len, 0, base_addr);
        spin_unlock_irqrestore(&aw_spi->lock, flags);
    } else {
        if (tx_buf) {
            spin_lock_irqsave(&aw_spi->lock, flags);
            aw_spi->duplex_flag = HALF_DUPLEX_TX;
            spi_set_bc_wtc(tx_len, 0, base_addr);
            spin_unlock_irqrestore(&aw_spi->lock, flags);
        } else if (rx_buf) {
            spin_lock_irqsave(&aw_spi->lock, flags);
            aw_spi->duplex_flag = HALF_DUPLEX_RX;
            spi_set_bc_wtc(0, rx_len, base_addr);
            spin_unlock_irqrestore(&aw_spi->lock, flags);
        }
    }

    /*
     * 1. Tx/Rx error irq, process in IRQ;
     * 2. Transfer Complete Interrupt Enable
     */
    spi_enable_irq(SPI_INTEN_TC | SPI_INTEN_ERR, base_addr);

    /* > 64 use DMA transfer, or use cpu */
    if (t->len > BULK_DATA_BOUNDARY) {
        spi_sel_dma_type(aw_spi->dma_type, base_addr);

        switch (aw_spi->duplex_flag) {
            case HALF_DUPLEX_RX: {
                spi_enable_dma_irq(SPI_DRQEN_RR, base_addr);

                dma_dir |= SPI_DMA_RDEV;
                ret = sun7i_spi_prepare_dma(aw_spi, SPI_DMA_RDEV);
                if (ret < 0) {
                    spi_disable_dma_irq(SPI_DRQEN_RR, base_addr);
                    spi_disable_irq(SPI_INTEN_TC | SPI_INTEN_ERR, base_addr);
                    return -EINVAL;
                }

                sun7i_spi_config_dma(aw_spi, SPI_DMA_RDEV, (void *)rx_buf, rx_len);
                sun7i_spi_start_dma(aw_spi, SPI_DMA_RDEV);

                spi_start_xfer(base_addr);
                break;
            }

            case HALF_DUPLEX_TX: {
                spi_start_xfer(base_addr);
                spi_enable_dma_irq(SPI_DRQEN_TE, base_addr);

                dma_dir |= SPI_DMA_WDEV;
                ret = sun7i_spi_prepare_dma(aw_spi, SPI_DMA_WDEV);
                if (ret < 0) {
                    spi_disable_irq(SPI_INTEN_TC | SPI_INTEN_ERR, base_addr);
                    spi_disable_dma_irq(SPI_DRQEN_TE, base_addr);
                    return -EINVAL;
                }

                sun7i_spi_config_dma(aw_spi, SPI_DMA_WDEV, (void *)tx_buf, tx_len);
                sun7i_spi_start_dma(aw_spi, SPI_DMA_WDEV);
                break;
            }

            case FULL_DUPLEX_RX_TX: {
                spi_enable_dma_irq(SPI_DRQEN_RR, base_addr);

                dma_dir |= SPI_DMA_RDEV;
                ret = sun7i_spi_prepare_dma(aw_spi, SPI_DMA_RDEV);
                if (ret < 0) {
                    spi_disable_dma_irq(SPI_DRQEN_RR, base_addr);
                    spi_disable_irq(SPI_INTEN_TC | SPI_INTEN_ERR, base_addr);
                    return -EINVAL;
                }

                sun7i_spi_config_dma(aw_spi, SPI_DMA_RDEV, (void *)rx_buf, rx_len);
                sun7i_spi_start_dma(aw_spi, SPI_DMA_RDEV);

                spi_start_xfer(base_addr);
                spi_enable_dma_irq(SPI_DRQEN_TE, base_addr);

                dma_dir |= SPI_DMA_WDEV;
                ret = sun7i_spi_prepare_dma(aw_spi, SPI_DMA_WDEV);
                if (ret < 0) {
                    spi_disable_irq(SPI_INTEN_TC | SPI_INTEN_ERR, base_addr);
                    spi_disable_dma_irq(SPI_DRQEN_TE, base_addr);
                    return -EINVAL;
                }

                sun7i_spi_config_dma(aw_spi, SPI_DMA_WDEV, (void *)tx_buf, tx_len);
                sun7i_spi_start_dma(aw_spi, SPI_DMA_WDEV);
                break;
            }

            default:
                return -1;
        }
    } else {
        switch (aw_spi->duplex_flag) {
            case HALF_DUPLEX_RX: {
                unsigned int poll_time = 0x7ffff;
                /* SMC=1,XCH trigger the transfer */
                spi_start_xfer(base_addr);
                while (rx_len && (--poll_time > 0)) {
                    /* rxFIFO counter */
                    if (spi_query_rxfifo(base_addr)) {
                        *rx_buf++ = readb(base_addr + SPI_RXDATA_REG);
                        --rx_len;
                    }
                }
                if (poll_time <= 0) {
                    spi_err("%s: spi%d cpu receive data time out\n", 
                            __func__, spi->master->bus_num);
                }
                break;
            }
            case HALF_DUPLEX_TX: {
                unsigned int poll_time = 0xfffff;
                spi_start_xfer(base_addr);

                spin_lock_irqsave(&aw_spi->lock, flags);
                for (; tx_len > 0; --tx_len) {
                    writeb(*tx_buf++, base_addr + SPI_TXDATA_REG);
                }
                spin_unlock_irqrestore(&aw_spi->lock, flags);

                while (spi_query_txfifo(base_addr) && (--poll_time > 0)); /* txFIFO counter */
                if (poll_time <= 0) {
                    spi_err("%s: spi%d cpu transfer data time out\n",
                            __func__, spi->master->bus_num);
                }
                break;
            }
            case FULL_DUPLEX_RX_TX: {
                unsigned int poll_time_tx = 0xfffff;
                unsigned int poll_time_rx = 0x7ffff;

                if ((rx_len == 0) || (tx_len == 0))
                    return -EINVAL;

                spi_start_xfer(base_addr);

                spin_lock_irqsave(&aw_spi->lock, flags);
                for (; tx_len > 0; --tx_len) {
                    writeb(*tx_buf++, base_addr + SPI_TXDATA_REG);
                }
                spin_unlock_irqrestore(&aw_spi->lock, flags);

                while (spi_query_txfifo(base_addr) && (--poll_time_tx > 0)); /* txFIFO counter */
                if (poll_time_tx <= 0) {
                    spi_err("%s: spi%d cpu transfer data time out\n",
                            __func__, spi->master->bus_num);
                    break;
                }

                while (rx_len && (--poll_time_rx > 0)) {
                    /* rxFIFO counter */
                    if (spi_query_rxfifo(base_addr)) {
                        *rx_buf++ =  readb(base_addr + SPI_RXDATA_REG);
                        --rx_len;
                    }
                }
                if (poll_time_rx <= 0) {
                    spi_err("%s: spi%d cpu receive data time out\n",
                            __func__, spi->master->bus_num);
                }
                break;
            }
            default:
                return -1;
        }
    }

    /* wait for xfer complete in the isr. */
    spi_dbg("%s: spi%d wait for xfer complete\n", __func__, spi->master->bus_num);
    wait_for_completion(&aw_spi->done);
    /* get the isr return code */
    if (aw_spi->result != 0) {
        spi_err("%s: spi%d xfer failed\n", __func__, spi->master->bus_num);
        ret = -1;
    }

    /* release dma resource if neccessary */
    if (SPI_DMA_RDEV & dma_dir) {
        sun7i_spi_release_dma(aw_spi, SPI_DMA_RDEV);
    }
    if (SPI_DMA_WDEV & dma_dir) {
        sun7i_spi_release_dma(aw_spi, SPI_DMA_WDEV);
    }
    
    if (aw_spi->duplex_flag != DUPLEX_NULL) {
        aw_spi->duplex_flag = DUPLEX_NULL;
    }

    return ret;
}

/* spi core xfer process */
static void sun7i_spi_work(struct work_struct *work)
{
    struct sun7i_spi *aw_spi = container_of(work, struct sun7i_spi, work);
    spin_lock_irq(&aw_spi->lock);
    aw_spi->busy = SPI_BUSY;

    spi_dbg("%s: enter\n", __func__);
    /*
     * get from messages queue, and then do with them,
     * if message queue is empty ,then return and set status to free,
     * otherwise process them.
     */
    while (!list_empty(&aw_spi->queue)) {
        struct spi_message *msg = NULL;
        struct spi_device  *spi = NULL;
        struct spi_transfer *t  = NULL;
        unsigned int cs_change = 0;
        int status;
        /* get message from message queue in sun7i_spi. */
        msg = container_of(aw_spi->queue.next, struct spi_message, queue);
        /* then delete from the message queue,now it is alone.*/
        list_del_init(&msg->queue);
        spin_unlock_irq(&aw_spi->lock);
        /* get spi device from this message */
        spi = msg->spi;
        /* set default value,no need to change cs,keep select until spi transfer require to change cs. */
        cs_change = 1;
        /* set message status to succeed. */
        status = -1;
        /* search the spi transfer in this message, deal with it alone. */
        list_for_each_entry(t, &msg->transfers, transfer_list) {
            if ((status == -1) || t->bits_per_word || t->speed_hz) { /* if spi transfer is zero,use spi device value. */
                status = sun7i_spi_xfer_setup(spi, t); /* set the value every spi transfer */
                if (status < 0)
                    break; /* fail, quit */
                spi_dbg("[spi-%d]: xfer setup \n", aw_spi->master->bus_num);
            }
            
            /* first active the cs */
            if (cs_change)
                aw_spi->cs_control(spi, 1);
            /* update the new cs value */
            cs_change = t->cs_change;
            
            /* full duplex mode */
            if (t->rx_buf && t->tx_buf)
                spi_clear_dhb(aw_spi->base_addr);
            
            /*
             * do transfer
             * > 64 : dma ;  <= 64 : cpu
             * wait for done completion in this function, wakup in the irq hanlder
             */
            status = sun7i_spi_xfer(spi, t);
            if (status)
                break; /* fail quit, zero means succeed */
            
            /* accmulate the value in the message */
            msg->actual_length += t->len;
            /* may be need to delay */
            if (t->delay_usecs)
                udelay(t->delay_usecs);
            
            /* if zero, keep active, otherwise deactived. */
            if (cs_change)
                aw_spi->cs_control(spi, 0);
        }
        
        /*
         * spi message complete,succeed or failed
         * return value
         */
        msg->status = status;
        /* wakup the uplayer caller,complete one message */
        msg->complete(msg->context);
        /* fail or need to change cs */
        if (status || !cs_change) {
            aw_spi->cs_control(spi, 0);
        }
        
        /* restore default value. */
        //sun7i_spi_xfer_setup(spi, NULL);
        spin_lock_irq(&aw_spi->lock);
    }
    
    /* set spi to free */
    aw_spi->busy = SPI_FREE;
    spin_unlock_irq(&aw_spi->lock);
}

/* wake up the sleep thread, and give the result code */
static irqreturn_t sun7i_spi_handler(int irq, void *dev_id)
{
    struct sun7i_spi *aw_spi = (struct sun7i_spi *)dev_id;
    void *base_addr = aw_spi->base_addr;

    unsigned int status = spi_qry_irq_pending(base_addr);
    spi_clr_irq_pending(status, base_addr);

    aw_spi->result = 0; /* assume succeed */
    /* master mode, Transfer Complete Interrupt */
    if (status & SPI_STAT_TC) {
        spi_dbg("%s: spi%d TC comes, irq status = 0x%08x\n",
			    __func__, aw_spi->master->bus_num, status);
        spi_disable_irq(SPI_STAT_TC | SPI_STAT_ERR, base_addr);
        /*
         * just check dma+callback receive, skip other condition.
         * dma+callback receive: when TC comes, dma may be still not complete fetch data from rxFIFO.
         * other receive: cpu or dma+poll, just skip this.
         */
        if (aw_spi->dma_hdle_rx) {
            unsigned int poll_time = 0xffff;
            /* during poll, dma maybe complete rx, rx_dma_used is 0. then return.*/
            while (spi_query_rxfifo(base_addr) && (--poll_time > 0));
            if (poll_time <= 0) {
                spi_err("%s: spi%d dma callback method, rx data time out in irq\n",
                        __func__, aw_spi->master->bus_num);
                aw_spi->result = -1;
                complete(&aw_spi->done);
                return -1;
            } else if (poll_time < 0xffff) {
                spi_dbg("%s: spi%d rx irq comes first, dma last. wait = 0x%x\n",
                        __func__, aw_spi->master->bus_num, poll_time);
            }
        }

        /* wakup uplayer, by the sem */
        complete(&aw_spi->done);
        return IRQ_HANDLED;
    }
    /* master mode: err */
    else if (status & SPI_STAT_ERR) {
        spi_err("%s: spi%d ERR comes, irq status = 0x%08x\n",
			    __func__, aw_spi->master->bus_num, status);
        /* error process, release dma in the workqueue,should not be here */
        spi_disable_irq(SPI_STAT_TC | SPI_STAT_ERR, base_addr);
        spi_restore_state(1, base_addr);
        aw_spi->result = -1;
        complete(&aw_spi->done);
        spi_err("%s: spi%d master mode error: txFIFO overflow/rxFIFO underrun or overflow\n",
                __func__, aw_spi->master->bus_num);
        return IRQ_HANDLED;
    }

    spi_dbg("%s: spi%d NONE comes\n", __func__, aw_spi->master->bus_num);
    return IRQ_NONE;
}

/* interface 1 */
static int sun7i_spi_transfer(struct spi_device *spi, struct spi_message *msg)
{
    struct sun7i_spi *aw_spi = spi_master_get_devdata(spi->master);
    unsigned long flags;
    msg->actual_length = 0;
    msg->status = -EINPROGRESS;

    spi_dbg("%s: enter\n", __func__);
    spin_lock_irqsave(&aw_spi->lock, flags);
    /* add msg to the sun7i_spi queue */
    list_add_tail(&msg->queue, &aw_spi->queue);
    /* add work to the workqueue,schedule the cpu. */
    queue_work(aw_spi->workqueue, &aw_spi->work);
    spin_unlock_irqrestore(&aw_spi->lock, flags);

    /* return immediately and wait for completion in the uplayer caller. */
    return 0;
}

/* interface 2, setup the frequency and default status */
static int sun7i_spi_setup(struct spi_device *spi)
{
    struct sun7i_spi *aw_spi = spi_master_get_devdata(spi->master);
    struct sun7i_spi_config *config = spi->controller_data; /* general is null. */
    unsigned long flags;

    spi_dbg("%s: enter\n", __func__);
    /* just support 8 bits per word */
    if (spi->bits_per_word != 8)
        return -EINVAL;
    /* first check its valid,then set it as default select,finally set its */
    if (sun7i_spi_check_cs(spi->chip_select, aw_spi)) {
        spi_err("%s: spi%d not support cs-%d \n", __func__,
                spi->master->bus_num, spi->chip_select);
        return -EINVAL;
    }

    if (spi->max_speed_hz > SPI_MAX_FREQUENCY)
        return -EINVAL;

    if (!config) {
        config = kzalloc(sizeof * config, GFP_KERNEL);
        if (!config) {
            spi_err("%s: spi%d alloc memory for config failed\n",
                    __func__, spi->master->bus_num);
            return -ENOMEM;
        }
        spi->controller_data = config;
    }

    /*
     * set the default vaule with spi device
     * can change by every spi transfer
     */
    config->bits_per_word = spi->bits_per_word;
    config->max_speed_hz  = spi->max_speed_hz;
    config->mode          = spi->mode;

    spin_lock_irqsave(&aw_spi->lock, flags);
    /* if aw16xx spi is free, then deactived the spi device */
    if (aw_spi->busy & SPI_FREE) {
        /* set chip select number */
        spi_set_cs(spi->chip_select, aw_spi->base_addr);
        /* deactivate chip select */
        aw_spi->cs_control(spi, 0);
    }
    spin_unlock_irqrestore(&aw_spi->lock, flags);

    return 0;
}

/* interface 3 */
static void sun7i_spi_cleanup(struct spi_device *spi)
{
    if (spi->controller_data) {
        kfree(spi->controller_data);
        spi->controller_data = NULL;
    }
}

static int sun7i_spi_set_gpio(struct sun7i_spi *aw_spi, bool on)
{
    if(on) {
        if(aw_spi->master->bus_num == 0) {
            aw_spi->gpio_hdle = gpio_request_ex("spi0_para", NULL);
            if(!aw_spi->gpio_hdle) {
                spi_err("spi0 request gpio fail!\n");
                return -1;
            }
            //hex_dump("gpio regs:", (void __iomem*)SW_VA_PORTC_IO_BASE, 0x200, 2);
        }
        else if(aw_spi->master->bus_num == 1) {
			/**
			 * PI8		SPI1_CS0
			 * PI9		SPI1_CS1
			 * PI10    	SPI1_CLK
			 * PI11    	SPI1_MOSI
			 * PI12	    SPI1_MISO
			 */
            #ifndef SYS_SPI_PIN
		    unsigned int  reg_val = readl(_Pn_CFG1(8));
			/* set spi function */
		    reg_val &= ~0x77777;
		    reg_val |=  0x22222;
		    writel(reg_val, _Pn_CFG1(8));
            /* set pull up */
    		reg_val = readl(_Pn_PUL1(8));
    		reg_val &= ~(0x3ff<<16);
    		reg_val |=  (0x155<<16);
    		writel(reg_val, _Pn_PUL1(8));
    		/* no need to set driver,default is driver 1. */
    		#else
            aw_spi->gpio_hdle = gpio_request_ex("spi1_para", NULL);
            if(!aw_spi->gpio_hdle) {
                spi_err("spi1 request gpio fail!\n");
                return -1;
            }
    		#endif
        }
        else if(aw_spi->master->bus_num == 2) {
            aw_spi->gpio_hdle = gpio_request_ex("spi2_para", NULL);
            if(!aw_spi->gpio_hdle) {
                spi_err("spi2 request gpio fail!\n");
                return -1;
            }
        }

        #ifdef AW1623_FPGA
        {
            #include <mach/platform.h>
            void __iomem* pi_cfg0 = (void __iomem*)(SW_VA_PORTC_IO_BASE+0x48);
            u32 rval = readl(pi_cfg0) & (~(0x70777));
            writel(rval|(0x30333), pi_cfg0);
        }
        #endif
    }
    else {
        if(aw_spi->master->bus_num == 0) {
            gpio_release(aw_spi->gpio_hdle, 0);
        }
        else if(aw_spi->master->bus_num == 1) {
        	#ifndef SYS_SPI_PIN
		    unsigned int  reg_val = readl(_Pn_CFG1(8));
			/* set default */
		    reg_val &= ~0x77777;
		    writel(reg_val, _Pn_CFG1(8));
            #else
		    gpio_release(aw_spi->gpio_hdle, 0);
		    #endif
        }
        else if(aw_spi->master->bus_num == 2) {
            gpio_release(aw_spi->gpio_hdle, 0);
        }
    }
	return 0;
}

static int sun7i_spi_set_mclk(struct sun7i_spi *aw_spi, u32 mod_clk)
{
    struct clk *source_clock = NULL;
    char *name = NULL;
#ifdef CONFIG_AW_FPGA_PLATFORM
//    u32 source = 0;
#elif defined CONFIG_AW_ASIC_PLATFORM
//	u32 source = 2;
#endif
	u32 source = 1;
    int ret = 0;

    switch (source) {
        case 0:
            source_clock = clk_get(NULL, "hosc");
            name = "hosc";
            break;
        case 1:
            source_clock = clk_get(NULL, "sdram_pll_p");
            name = "sdram_pll_p";
            break;
        case 2:
            source_clock = clk_get(NULL, "sata_pll");
            name = "sata_pll";
            break;
        default:
            return -1;
    }

    if (IS_ERR(source_clock)) {
        ret = PTR_ERR(source_clock);
        spi_err("%s: Unable to get spi%d source clock resource\n",
                __func__, aw_spi->master->bus_num);
        return -1;
    }

    if (clk_set_parent(aw_spi->mclk, source_clock)) {
        spi_err("%s: spi%d clk_set_parent failed\n", __func__,
                aw_spi->master->bus_num);
        ret = -1;
        goto out;
    }

    if (clk_set_rate(aw_spi->mclk, mod_clk)) {
        spi_err("%s: spi%d clk_set_rate failed\n", __func__,
                aw_spi->master->bus_num);
        ret = -1;
        goto out;
    }

    spi_inf("%s: spi%d source = %s, src_clk = %u, mclk %u\n",
            __func__, aw_spi->master->bus_num,  name,
            (unsigned)clk_get_rate(source_clock), (unsigned)clk_get_rate(aw_spi->mclk));

    if (clk_enable(aw_spi->mclk)) {
        spi_err("%s: spi%d couldn't enable module clock 'spi'\n",
                __func__, aw_spi->master->bus_num);
        ret = -EBUSY;
        goto out;
    }

    ret = 0;
out:
    clk_put(source_clock);
    return ret;
}

static int sun7i_spi_hw_init(struct sun7i_spi *aw_spi)
{
    void *base_addr = aw_spi->base_addr;
    unsigned long sclk_freq = 0;
    char *mclk_name[] = {"spi0", "spi1", "spi2", "spi3"};

    aw_spi->mclk = clk_get(&aw_spi->pdev->dev, mclk_name[aw_spi->pdev->id]);
    if (IS_ERR(aw_spi->mclk)) {
        spi_err("%s: spi%d unable to acquire module clock 'spi'\n",
                __func__, aw_spi->master->bus_num);
        return -1;
    }
    if (sun7i_spi_set_mclk(aw_spi, 100000000)) {
        spi_err("%s: spi%d sun7i_spi_set_mclk 'spi'\n",
                __func__, aw_spi->master->bus_num);
        clk_put(aw_spi->mclk);
        return -1;
    }

    /* 1. enable the spi module */
    spi_enable_bus(base_addr);

    /* 2. set the default chip select */
    if (sun7i_spi_check_cs(0, aw_spi)) {
        spi_set_cs(1, base_addr);
    } else {
        spi_set_cs(0, base_addr);
    }

    /*
     * 3. master: set spi module clock;
     * 4. set the default frequency 10MHz
     */
    spi_set_master(base_addr);
    sclk_freq  = clk_get_rate(aw_spi->mclk);
    spi_set_clk(10000000, sclk_freq, base_addr);

    /*
     * 5. master : set POL,PHA,SSOPL,LMTF,DDB,DHB; default: SSCTL=0,SMC=1,TBW=0.
     * 6. set bit width-default: 8 bits
     */
    spi_config(1, SPI_MODE_0, base_addr);

    /* 7. manual control the chip select */
    spi_ss_ctrl(base_addr, 1);
    return 0;
}

static int sun7i_spi_hw_exit(struct sun7i_spi *aw_spi)
{
    /* disable the spi controller */
    spi_disable_bus(aw_spi->base_addr);
    /* disable module clock */
    clk_disable(aw_spi->mclk);
    clk_put(aw_spi->mclk);

    return 0;
}

static int __init sun7i_spi_probe(struct platform_device *pdev)
{
    struct resource *mem_res, *dma_res_rx, *dma_res_tx;
    struct sun7i_spi *aw_spi;
    struct sun7i_spi_platform_data *pdata;
    struct spi_master *master;
    int ret = 0, err = 0, irq;
    int cs_bitmap = 0;

    if (pdev->id < 0) {
        spi_err("%s: invalid platform device id: %d\n", __func__, pdev->id);
        return -ENODEV;
    }

    if (pdev->dev.platform_data == NULL) {
        spi_err("%s: platform_data missing\n", __func__);
        return -ENODEV;
    }

    pdata = pdev->dev.platform_data;
    if (!pdata->clk_name) {
        spi_err("%s: platform data must initialize\n", __func__);
        return -EINVAL;
    }

    /* Check for availability of necessary resource */
    dma_res_rx = platform_get_resource(pdev, IORESOURCE_DMA, 0);
    if (dma_res_rx == NULL) {
        spi_err("%s: Unable to get spi DMA RX resource\n", __func__);
        return -ENXIO;
    }

    dma_res_tx = platform_get_resource(pdev, IORESOURCE_DMA, 1);
    if (dma_res_tx == NULL) {
        spi_err("%s: Unable to get spi DMA TX resource\n", __func__);
        return -ENXIO;
    }

    mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (mem_res == NULL) {
        spi_err("%s: Unable to get spi MEM resource\n", __func__);
        return -ENXIO;
    }

    irq = platform_get_irq(pdev, 0);
    if (irq < 0) {
        spi_err("%s: NO SPI IRQ specified\n", __func__);
        return -ENXIO;
    }

    /* create spi master */
    master = spi_alloc_master(&pdev->dev, sizeof(struct sun7i_spi));
    if (master == NULL) {
        spi_err("%s: Unable to allocate SPI Master\n", __func__);
        return -ENOMEM;
    }

    platform_set_drvdata(pdev, master);
    aw_spi = spi_master_get_devdata(master);
    memset(aw_spi, 0, sizeof(struct sun7i_spi));

    aw_spi->master          = master;
    aw_spi->irq             = irq;
#ifdef CONFIG_SUN7I_SPI_NDMA
	aw_spi->dma_type        = CHAN_NORMAL;
#else
	aw_spi->dma_type        = CHAN_DEDICATE;
#endif
	spi_inf("%s: spi%d dma type: %s\n", __func__, pdev->id,
			aw_spi->dma_type == CHAN_NORMAL ? "normal" : "dedicate");
    aw_spi->dma_id_tx       = dma_res_tx->start;
    aw_spi->dma_id_rx       = dma_res_rx->start;
    aw_spi->dma_hdle_rx     = 0;
    aw_spi->dma_hdle_tx     = 0;
    aw_spi->cs_control      = sun7i_spi_cs_control;
    aw_spi->cs_bitmap       = pdata->cs_bitmap; /* cs0-0x1; cs1-0x2; cs0&cs1-0x3. */
    aw_spi->busy            = SPI_FREE;
    aw_spi->duplex_flag     = DUPLEX_NULL;

    master->bus_num         = pdev->id;
    master->setup           = sun7i_spi_setup;
    master->cleanup         = sun7i_spi_cleanup;
    master->transfer        = sun7i_spi_transfer;
    master->num_chipselect  = pdata->num_cs;
    /* the spi->mode bits understood by this driver: */
    master->mode_bits       = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH | SPI_LSB_FIRST;

    /* update the cs bitmap */
    cs_bitmap = sun7i_spi_get_cfg_csbitmap(pdev->id);
    if (cs_bitmap & 0x3) {
        aw_spi->cs_bitmap = cs_bitmap & 0x3;
        spi_inf("%s: spi%d cs bitmap: 0x%x\n",
                __func__, master->bus_num, cs_bitmap);
    }

    err = request_irq(aw_spi->irq, sun7i_spi_handler, IRQF_DISABLED, pdev->name, aw_spi);
    if (err) {
        spi_err("%s: Cannot claim IRQ\n", __func__);
        goto err0;
    }

    if (request_mem_region(mem_res->start,
                           resource_size(mem_res), pdev->name) == NULL) {
        spi_err("%s: Req mem region failed\n", __func__);
        ret = -ENXIO;
        goto err1;
    }

    aw_spi->base_addr = ioremap(mem_res->start, resource_size(mem_res));
    if (aw_spi->base_addr == NULL) {
        spi_err("%s: Unable to remap IO\n", __func__);
        ret = -ENXIO;
        goto err2;
    }

    /* Setup clocks */
    aw_spi->hclk = clk_get(&pdev->dev, pdata->clk_name);
    if (IS_ERR(aw_spi->hclk)) {
        spi_err("%s: Unable to get clock 'spi'\n", __func__);
        ret = PTR_ERR(aw_spi->hclk);
        goto err3;
    }

    if (clk_enable(aw_spi->hclk)) {
        spi_err("%s: Couldn't enable clock 'spi'\n", __func__);
        ret = -EBUSY;
        goto err4;
    }

    aw_spi->workqueue = create_singlethread_workqueue(dev_name(master->dev.parent));
    if (aw_spi->workqueue == NULL) {
        spi_err("%s: Unable to create workqueue\n", __func__);
        ret = -ENOMEM;
        goto err5;
    }

    aw_spi->pdev = pdev;

    /* Setup Deufult Mode */
    sun7i_spi_hw_init(aw_spi);

#ifndef SYS_SPI_PIN
    /* set gpio */
    gpio_addr = ioremap(_PIO_BASE_ADDRESS, 0x1000);
#endif
    sun7i_spi_set_gpio(aw_spi, 1);

    spin_lock_init(&aw_spi->lock);
    init_completion(&aw_spi->done);
    INIT_WORK(&aw_spi->work, sun7i_spi_work); /* banding the process handler */
    INIT_LIST_HEAD(&aw_spi->queue);

    if (spi_register_master(master)) {
        spi_err("%s: Cannot register SPI master\n", __func__);
        ret = -EBUSY;
        goto err6;
    }

    spi_inf("%s: reuuimlla's SoC SPI Driver loaded for Bus SPI%d with %d Slaves at most\n",
            __func__, pdev->id, master->num_chipselect);
    spi_inf("%s: spi%d driver probe succeed, base %p, irq %d, dma_id_rx %d, dma_id_tx %d\n",
            __func__, master->bus_num, aw_spi->base_addr, aw_spi->irq,
            aw_spi->dma_id_rx, aw_spi->dma_id_tx);
    return 0;

err6:
    destroy_workqueue(aw_spi->workqueue);
err5:
    iounmap((void *)aw_spi->base_addr);
err4:
err3:
err2:
    release_mem_region(mem_res->start, resource_size(mem_res));
err1:
    free_irq(aw_spi->irq, aw_spi);
err0:
    platform_set_drvdata(pdev, NULL);
    spi_master_put(master);
    return ret;
}

static int sun7i_spi_remove(struct platform_device *pdev)
{
    struct spi_master *master = spi_master_get(platform_get_drvdata(pdev));
    struct sun7i_spi *aw_spi = spi_master_get_devdata(master);
    struct resource *mem_res;
    unsigned long flags;

    spin_lock_irqsave(&aw_spi->lock, flags);
    aw_spi->busy |= SPI_FREE;
    spin_unlock_irqrestore(&aw_spi->lock, flags);

    while (aw_spi->busy & SPI_BUSY) {
        spi_inf("%s: spi%d busy\n", __func__, master->bus_num);
        msleep(10);
    }

    sun7i_spi_hw_exit(aw_spi);
    spi_unregister_master(master);
    destroy_workqueue(aw_spi->workqueue);

    clk_disable(aw_spi->hclk);
    clk_put(aw_spi->hclk);

    iounmap((void *) aw_spi->base_addr);
    mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (mem_res != NULL)
        release_mem_region(mem_res->start, resource_size(mem_res));

    platform_set_drvdata(pdev, NULL);
    spi_master_put(master);

    return 0;
}

#ifdef CONFIG_PM
static int sun7i_spi_suspend(struct platform_device *pdev, pm_message_t state)
{
    struct spi_master *master = spi_master_get(platform_get_drvdata(pdev));
    struct sun7i_spi *aw_spi = spi_master_get_devdata(master);
    unsigned long flags;

    spin_lock_irqsave(&aw_spi->lock, flags);
    aw_spi->busy |= SPI_SUSPND;
    spin_unlock_irqrestore(&aw_spi->lock, flags);

    while (aw_spi->busy & SPI_BUSY) {
        spi_inf("%s: spi%d busy\n", __func__, master->bus_num);
        msleep(10);
    }

    /* Disable the clock */
    clk_disable(aw_spi->hclk);

    spi_inf("%s: spi%d suspend okay...\n", __func__, master->bus_num);
    return 0;
}

static int sun7i_spi_resume(struct platform_device *pdev)
{
    struct spi_master *master = spi_master_get(platform_get_drvdata(pdev));
    struct sun7i_spi  *aw_spi = spi_master_get_devdata(master);
    unsigned long flags;

    /* Enable the clock */
    clk_enable(aw_spi->hclk);
    sun7i_spi_hw_init(aw_spi);

    spin_lock_irqsave(&aw_spi->lock, flags);
    aw_spi->busy = SPI_FREE;
    spin_unlock_irqrestore(&aw_spi->lock, flags);

    spi_inf("%s: spi%d resume okay...\n", __func__, master->bus_num);
    return 0;
}
#else
#define sun7i_spi_suspend   NULL
#define sun7i_spi_resume    NULL
#endif /* CONFIG_PM */

static struct platform_driver sun7i_spi_driver = {
    .probe   = sun7i_spi_probe,
    .remove  = sun7i_spi_remove,
    .suspend = sun7i_spi_suspend,
    .resume  = sun7i_spi_resume,
    .driver = {
        .name   = "sun7i-spi",
        .owner = THIS_MODULE,
    },
};

/* spi resouce and platform data start */
struct sun7i_spi_platform_data sun7i_spi0_pdata = {
    .cs_bitmap  = 0x3,
    .num_cs     = 2,
    .clk_name   = "ahb_spi0",
};

static struct resource sun7i_spi0_resources[] = {
    [0] = {
        .start  = SPI0_BASE_ADDR,
        .end    = SPI0_BASE_ADDR + 1024,
        .flags  = IORESOURCE_MEM,
    },
#ifdef CONFIG_SUN7I_SPI_NDMA
    [1] = {
        .start  = N_SRC_SPI0_RX,
        .end    = N_SRC_SPI0_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = N_DST_SPI0_TX,
        .end    = N_DST_SPI0_TX,
        .flags  = IORESOURCE_DMA,
    },
#else
    [1] = {
        .start  = D_SRC_SPI0_RX,
        .end    = D_SRC_SPI0_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = D_DST_SPI0_TX,
        .end    = D_DST_SPI0_TX,
        .flags  = IORESOURCE_DMA,
    },
#endif
    [3] = {
        .start	= SW_INT_IRQNO_SPI00,
        .end	= SW_INT_IRQNO_SPI00,
        .flags  = IORESOURCE_IRQ,
    }
};

static struct platform_device sun7i_spi0_device = {
    .name           = "sun7i-spi",
    .id             = 0,
    .num_resources  = ARRAY_SIZE(sun7i_spi0_resources),
    .resource       = sun7i_spi0_resources,
    .dev            = {
        .platform_data = &sun7i_spi0_pdata,
    },
};

struct sun7i_spi_platform_data sun7i_spi1_pdata = {
    .cs_bitmap  = 0x3,
    .num_cs     = 2,
    .clk_name   = "ahb_spi1",
};

static struct resource sun7i_spi1_resources[] = {
    [0] = {
        .start  = SPI1_BASE_ADDR,
        .end    = SPI1_BASE_ADDR + 1024,
        .flags  = IORESOURCE_MEM,
    },
#ifdef CONFIG_SUN7I_SPI_NDMA
    [1] = {
        .start  = N_SRC_SPI1_RX,
        .end    = N_SRC_SPI1_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = N_DST_SPI1_TX,
        .end    = N_DST_SPI1_TX,
        .flags  = IORESOURCE_DMA,
    },
#else
    [1] = {
        .start  = D_SRC_SPI1_RX,
        .end    = D_SRC_SPI1_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = N_DST_SPI1_TX,
        .end    = N_DST_SPI1_TX,
        .flags  = IORESOURCE_DMA,
    },
#endif
    [3] = {
        .start	= SW_INT_IRQNO_SPI01,
        .end	= SW_INT_IRQNO_SPI01,
        .flags  = IORESOURCE_IRQ,
    }
};

static struct platform_device sun7i_spi1_device = {
    .name           = "sun7i-spi",
    .id             = 1,
    .num_resources  = ARRAY_SIZE(sun7i_spi1_resources),
    .resource       = sun7i_spi1_resources,
    .dev            = {
        .platform_data = &sun7i_spi1_pdata,
    },
};

static struct resource sun7i_spi2_resources[] = {
    [0] = {
        .start  = SPI2_BASE_ADDR,
        .end    = SPI2_BASE_ADDR + 1024,
        .flags  = IORESOURCE_MEM,
    },
#ifdef CONFIG_SUN7I_SPI_NDMA
    [1] = {
        .start  = N_SRC_SPI2_RX,
        .end    = N_SRC_SPI2_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = N_DST_SPI2_TX,
        .end    = N_DST_SPI2_TX,
        .flags  = IORESOURCE_DMA,
    },
#else
    [1] = {
        .start  = D_SRC_SPI2_RX,
        .end    = D_SRC_SPI2_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = N_DST_SPI2_TX,
        .end    = N_DST_SPI2_TX,
        .flags  = IORESOURCE_DMA,
    },
#endif
    [3] = {
        .start	= SW_INT_IRQNO_SPI02,
        .end	= SW_INT_IRQNO_SPI02,
        .flags  = IORESOURCE_IRQ,
    }
};

struct sun7i_spi_platform_data sun7i_spi2_pdata = {
    .cs_bitmap  = 0x3,
    .num_cs     = 2,
    .clk_name   = "ahb_spi2",
};

static struct platform_device sun7i_spi2_device = {
    .name           = "sun7i-spi",
    .id             = 2,
    .num_resources  = ARRAY_SIZE(sun7i_spi2_resources),
    .resource       = sun7i_spi2_resources,
    .dev            = {
        .platform_data = &sun7i_spi2_pdata,
    },
};

static struct resource sun7i_spi3_resources[] = {
    [0] = {
        .start  = SPI3_BASE_ADDR,
        .end    = SPI3_BASE_ADDR + 1024,
        .flags  = IORESOURCE_MEM,
    },
#ifdef CONFIG_SUN7I_SPI_NDMA
    [1] = {
        .start  = N_SRC_SPI3_RX,
        .end    = N_SRC_SPI3_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = N_DST_SPI3_TX,
        .end    = N_DST_SPI3_TX,
        .flags  = IORESOURCE_DMA,
    },
#else
    [1] = {
        .start  = D_SRC_SPI3_RX,
        .end    = D_SRC_SPI3_RX,
        .flags  = IORESOURCE_DMA,
    },
    [2] = {
        .start  = N_DST_SPI3_TX,
        .end    = N_DST_SPI3_TX,
        .flags  = IORESOURCE_DMA,
    },
#endif
    [3] = {
        .start	= SW_INT_IRQNO_SPI3,
        .end	= SW_INT_IRQNO_SPI3,
        .flags  = IORESOURCE_IRQ,
    }
};

struct sun7i_spi_platform_data sun7i_spi3_pdata = {
    .cs_bitmap  = 0x3,
    .num_cs     = 2,
    .clk_name = "ahb_spi3",
};

static struct platform_device sun7i_spi3_device = {
    .name           = "sun7i-spi",
    .id             = 3,
    .num_resources  = ARRAY_SIZE(sun7i_spi3_resources),
    .resource       = sun7i_spi3_resources,
    .dev            = {
        .platform_data = &sun7i_spi3_pdata,
    },
};

static struct spi_board_info *spi_boards = NULL;
static int sun7i_spi_register_spidev(void)
{
    int spi_dev_num = 0;
    int ret = 0;
    int i = 0;
    unsigned int irq_gpio = 0;
    char spi_board_name[32] = {0};
    struct spi_board_info* board;

    ret = script_parser_fetch("spi_devices", "spi_dev_num", &spi_dev_num, sizeof(int));
    if(ret != SCRIPT_PARSER_OK){
        spi_err("Get spi devices number failed\n");
        return -1;
    }
    spi_inf("Found %d spi devices in config files\n", spi_dev_num);

    /* alloc spidev board information structure */
    spi_boards = (struct spi_board_info*)kzalloc(sizeof(struct spi_board_info) * spi_dev_num, GFP_KERNEL);
    if (spi_boards == NULL)
    {
        spi_err("Alloc spi board information failed \n");
        return -1;
    }

    spi_inf("%-10s %-16s %-16s %-8s %-4s %-4s\n", "boards num", "modalias", "max_spd_hz", "bus_num", "cs", "mode");
    for (i=0; i<spi_dev_num; i++)
    {
        board = &spi_boards[i];
        sprintf(spi_board_name, "spi_board%d", i);
        ret = script_parser_fetch(spi_board_name, "modalias", (void*)board->modalias, sizeof(char*));
        if(ret != SCRIPT_PARSER_OK) {
            spi_err("Get spi devices modalias failed\n");
            goto fail;
        }
        ret = script_parser_fetch(spi_board_name, "max_speed_hz", (void*)&board->max_speed_hz, sizeof(int));
        if(ret != SCRIPT_PARSER_OK) {
            spi_err("Get spi devices max_speed_hz failed\n");
            goto fail;
        }
        ret = script_parser_fetch(spi_board_name, "bus_num", (void*)&board->bus_num, sizeof(u16));
        if(ret != SCRIPT_PARSER_OK) {
            spi_err("Get spi devices bus_num failed\n");
            goto fail;
        }
        ret = script_parser_fetch(spi_board_name, "chip_select", (void*)&board->chip_select, sizeof(u16));
        if(ret != SCRIPT_PARSER_OK) {
            spi_err("Get spi devices chip_select failed\n");
            goto fail;
        }
        ret = script_parser_fetch(spi_board_name, "mode", (void*)&board->mode, sizeof(u8));
        if(ret != SCRIPT_PARSER_OK) {
            spi_err("Get spi devices mode failed\n");
            goto fail;
        }
	ret = script_parser_fetch(spi_board_name, "irq_gpio", (void*)&irq_gpio, sizeof(unsigned int));
	if (ret != SCRIPT_PARSER_OK) {
		spi_inf("%s irq gpio not used\n", spi_board_name);
		board->irq = -1;
	} else if(gpio_request(irq_gpio, spi_board_name) == 0)
		board->irq = gpio_to_irq(irq_gpio);
        /*
        ret = script_parser_fetch(spi_board_name, "full_duplex", &board->full_duplex, sizeof(int));
        if(ret != SCRIPT_PARSER_OK) {
            spi_err("Get spi devices full_duplex failed\n");
            goto fail;
        }
        ret = script_parser_fetch(spi_board_name, "manual_cs", &board->manual_cs, sizeof(int));
        if(ret != SCRIPT_PARSER_OK) {
            spi_err("Get spi devices manual_cs failed\n");
            goto fail;
        }
        */
        spi_inf("%-10d %-16s %-16d %-8d %-4d 0x%-4d\n", i, board->modalias, board->max_speed_hz, board->bus_num, board->chip_select, board->mode);
    }

    /* register boards */
    ret = spi_register_board_info(spi_boards, spi_dev_num);
    if (ret)
    {
        spi_err("Register board information failed\n");
        goto fail;
    }
    return 0;
fail:
    if (spi_boards)
    {
        kfree(spi_boards);
        spi_boards = NULL;
    }
    return -1;

}

static int sun7i_spi_get_cfg_csbitmap(int bus_num)
{
    int value = 0;
    int ret   = 0;
    char *main_name[] = {"spi0_para", "spi1_para", "spi2_para", "spi3_para"};
    char *sub_name = "spi_cs_bitmap";
    ret = script_parser_fetch(main_name[bus_num], sub_name, &value, sizeof(int));
    if(ret != SCRIPT_PARSER_OK){
        spi_err("get spi %d para failed, err code = %d \n", bus_num, ret);
        return 0;
    }
    spi_inf("bus num = %d, spi used = %d \n", bus_num, value);
    return value;

}

#ifdef CONFIG_SUN7I_SPI_NORFLASH
#include <linux/spi/flash.h>
#include <linux/mtd/partitions.h>

/*struct mtd_partition part = {
	.name = "p1",
	.size = 8388608,
	.offset = 0,
};*/

static struct flash_platform_data at25df641_info = {
    .name = "m25p80",
    .parts = NULL,
    .nr_parts = 0,
    .type = "at25df641",
};

static struct spi_board_info norflash = {
    .modalias = "m25p80",
    .platform_data = &at25df641_info,
    .mode = SPI_MODE_0,
    .irq = 0,
    .max_speed_hz = 10 * 1000 * 1000,
    .bus_num = 0,
    .chip_select = 0,
};

static void __init sun7i_spi_norflash(void)
{
    if (spi_register_board_info(&norflash, 1)) {
        spi_err("%s: Register norflash:%s information failed\n",
                __func__, at25df641_info.type);
    } else {
        spi_inf("%s: Register norflash:%s information OK\n",
                __func__, at25df641_info.type);
    }
}
#else
static void __init sun7i_spi_norflash(void)
{}
#endif /* CONFIG_SUN7I_SPI_NORFLASH */

/* get configuration in the script */
#define SPI0_USED_MASK 0x1
#define SPI1_USED_MASK 0x2
#define SPI2_USED_MASK 0x4
#define SPI3_USED_MASK 0x8
static int spi_used = 0;

static int __init spi_sun7i_init(void)
{
    int used = 0;
    int i = 0;
    int ret = 0;
    char spi_para[16] = {0};

    spi_dbg("sw spi init !!\n");
    spi_used = 0;
    for (i=0; i<4; i++)
    {
        used = 0;
        sprintf(spi_para, "spi%d_para", i);
        ret = script_parser_fetch(spi_para, "spi_used", &used, sizeof(int));
        if (ret)
        {
            spi_err("sw spi init fetch spi%d uning configuration failed\n", i);
            continue;
        }
        if (used)
            spi_used |= 1 << i;
    }

    ret = sun7i_spi_register_spidev();
    if (ret)
    {
        spi_err("register spi devices board info failed \n");
    }

//    sun7i_spi_norflash();

    if (spi_used & SPI0_USED_MASK)
        platform_device_register(&sun7i_spi0_device);
    if (spi_used & SPI1_USED_MASK)
        platform_device_register(&sun7i_spi1_device);
    if (spi_used & SPI2_USED_MASK)
        platform_device_register(&sun7i_spi2_device);
    if (spi_used & SPI3_USED_MASK)
        platform_device_register(&sun7i_spi3_device);

    if (spi_used)
    {
        return platform_driver_register(&sun7i_spi_driver);
    }
    else
    {
        pr_warning("spi: cannot find any using configuration for \
                    all 4 spi controllers, return directly!\n");
        return 0;
    }
}

static void __exit spi_sun7i_exit(void)
{
    if (spi_used)
        platform_driver_unregister(&sun7i_spi_driver);
}

module_init(spi_sun7i_init);
module_exit(spi_sun7i_exit);

MODULE_AUTHOR("pannan");
MODULE_DESCRIPTION("SUN7I SPI BUS Driver");
MODULE_ALIAS("platform:sun7i-spi");
MODULE_LICENSE("GPL");
