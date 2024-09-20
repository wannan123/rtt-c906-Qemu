/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-19     JasonHu      first version
 */

#include <rthw.h>
#include <rtthread.h>

#include "rt_interrupt.h"

extern rt_uint32_t rt_interrupt_nest;
extern rt_uint32_t rt_interrupt_from_thread, rt_interrupt_to_thread;
extern rt_uint32_t rt_thread_switch_interrupt_flag;

struct rt_irq_desc isr_table[INTERRUPTS_MAX];

static void rt_hw_interrupt_handler(int vector, void *param)
{
    rt_kprintf("Unhandled interrupt %d occured!!!\n", vector);
}

/**
 * This function will initialize hardware interrupt
 */
void rt_hw_interrupt_init(void)
{
    rt_int32_t idx;

    rt_memset(isr_table, 0x00, sizeof(isr_table));
    for (idx = 0; idx < INTERRUPTS_MAX; idx++)
    {
        isr_table[idx].handler = rt_hw_interrupt_handler;
    }

    /* init interrupt nest, and context in thread sp */
    rt_interrupt_nest               = 0;
    rt_interrupt_from_thread        = 0;
    rt_interrupt_to_thread          = 0;
    rt_thread_switch_interrupt_flag = 0;
}

/**
 * This function will mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_mask(int vector)
{

}

/**

 * This function will un-mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_umask(int vector)
{

}

/**
 * This function will install a interrupt service routine to a interrupt.
 * @param vector the interrupt number
 * @param handler the interrupt service routine to be installed
 * @param param the interrupt service function parameter
 * @param name the interrupt name
 * @return old handler
 */
rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name)
{
    rt_isr_handler_t old_handler = RT_NULL;
    if ((vector < 0) || (vector > IRQ_MAX_NR))
    {
        return old_handler;
    }

    old_handler = isr_table[IRQ_OFFSET + vector].handler;

#ifdef RT_USING_INTERRUPT_INFO
    rt_strncpy(isr_table[IRQ_OFFSET + vector].name, name, RT_NAME_MAX);
#endif /* RT_USING_INTERRUPT_INFO */
    isr_table[IRQ_OFFSET + vector].handler = handler;
    isr_table[IRQ_OFFSET + vector].param = param;

    return old_handler;
}
#include <stdint.h>
#include <soc.h>
#include <csi_core.h>
#include <csi_config.h>
#include <drv/common.h>
#include "irq_port.h"

void rt_hw_irq_isr(void)
{
    uint32_t irqn;

    /* 获取当前中断号 */
    irqn = soc_irq_get_irq_num();

    /* 进入中断 */
    rt_interrupt_enter();

    /* 检查中断号是否有效并调用中断服务例程 */
    if (irqn < IRQ_MAX_NR && isr_table[IRQ_OFFSET + irqn].handler)
    {
        isr_table[IRQ_OFFSET + irqn].handler(irqn, isr_table[IRQ_OFFSET + irqn].param);
    }
    else
    {
        rt_kprintf("Unhandled interrupt %d occurred!!!\n", irqn);
    }

    /* 离开中断 */
    rt_interrupt_leave();

    /* 清除中断 */
    soc_irq_end(irqn);
}