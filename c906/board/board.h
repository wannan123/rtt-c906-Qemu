/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/01/11     flyingcys    The first version
 */
#ifndef __BOARD_H__
#define __BOARD_H__

#include "tick.h"


#include <rtconfig.h>

extern unsigned long __heap_start;
extern unsigned long __heap_end;

void rt_hw_board_init(void);

#endif
