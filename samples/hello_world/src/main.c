/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

int main(void)
{
	printf("\nHello World!\nRunning Zephyr S-mode on top of the %s\n", CONFIG_BOARD_TARGET);

	return 0;
}
