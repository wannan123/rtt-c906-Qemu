/*
 * Copyright (C) 2019-2020 Alibaba Group Holding Limited
 */

#include <stdlib.h>
#include <stdio.h>
#include <board.h>
#include "board_uart.h"
#include <drv/uart.h>



// Console config, Almost all solutions and demos use these.
#define BOARD_UART0_IDX (0)
#define BOARD_UART0_BAUD (115200)

#ifndef CONSOLE_UART_IDX
#define CONSOLE_UART_IDX (BOARD_UART0_IDX)
#endif

#ifndef CONFIG_CLI_USART_BAUD
#define CONFIG_CLI_USART_BAUD (BOARD_UART0_BAUD)
#endif

#ifndef CONFIG_CONSOLE_UART_BUFSIZE
#define CONFIG_CONSOLE_UART_BUFSIZE (128)
#endif

__attribute__((weak)) csi_uart_t g_console_handle;



#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <drv/uart.h>


/* UART发送字符函数 */
static int uart_putc(struct rt_serial_device *serial, char c)
{
    csi_uart_putc(&g_console_handle, c);
    return RT_EOK;
}

/* UART接收字符函数 */
static int uart_getc(struct rt_serial_device *serial)
{
    int ch = csi_uart_getc(&g_console_handle);
    
    return ch;
}
/* UART操作函数集 */
static const struct rt_uart_ops drv_uart_ops =
{
    .configure = RT_NULL,
    .control = RT_NULL,
    .putc = uart_putc,
    .getc = uart_getc,
    .dma_transmit = RT_NULL,
};

/* UART设备句柄 */
static struct rt_serial_device serial;
static struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

void uart_isr(int vector, void *param)
{
    rt_kprintf("UART interrupt triggered\n");
    dw_uart_irq_handler(param);
}

#include "soc.h" 
/* UART初始化函数 */
int rt_hw_uart_init(void)
{
    /* 初始化UART硬件 */
    csi_uart_init(&g_console_handle, CONSOLE_UART_IDX);
    csi_uart_baud(&g_console_handle, CONFIG_CLI_USART_BAUD);
    csi_uart_format(&g_console_handle, UART_DATA_BITS_8, UART_PARITY_NONE, UART_STOP_BITS_1);

    /* 注册UART设备 */
    serial.ops = &drv_uart_ops;
    serial.config = config;
    rt_hw_serial_register(&serial, "uart0", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, RT_NULL);
    
    /* 注册中断处理函数 */
    // rt_hw_interrupt_install(UART_IRQ, uart_isr, RT_NULL, "uart0");
    // rt_hw_interrupt_umask(UART_IRQ);
    rt_hw_interrupt_install(DW_UART0_IRQn, uart_isr, RT_NULL, "uart0");
    rt_hw_interrupt_umask(DW_UART0_IRQn);

    return RT_EOK;
}
