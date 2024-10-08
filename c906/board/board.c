/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/01/11     flyingcys    The first version
 */
#include <rthw.h>
#include <rtthread.h>
#include <string.h>
#include "board.h"
#include "board_uart.h"
#include "tick.h"
void section_bss_clear(void)
{
    extern unsigned long __bss_start__;
    extern unsigned long __bss_end__;

    memset((void *)(&__bss_start__), \
           0, \
           (unsigned long)(&__bss_end__) - (unsigned long)(&__bss_start__));

}

void rt_hw_board_init(void)
{
#ifdef RT_USING_HEAP
    /* initialize memory system */
    rt_system_heap_init((void *)&__heap_start, (void *)&__heap_end);
#endif

    /* initalize interrupt */
    rt_hw_interrupt_init();

    /* init rtthread hardware */
    rt_hw_tick_init();

    rt_hw_uart_init();

    /* Set the shell console output device */
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#ifdef RT_USING_HEAP
    /* initialize memory system */
    rt_kprintf("RT_HW_HEAP_BEGIN:%x RT_HW_HEAP_END:%x size: %d\r\n", (void *)&__heap_start, (void *)&__heap_end, (void *)&__heap_end - (void *)&__heap_start);
#endif

}
