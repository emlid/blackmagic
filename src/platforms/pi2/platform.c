/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2011  Black Sphere Technologies Ltd.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "platform.h"
#include "gdb_if.h"
#include "jtag_scan.h"
#include "gpio.h"

int platform_init(int argc, char **argv)
{
	assert(gpio_enable(GPIO_SWDIO) == 0);
	assert(gpio_enable(GPIO_SWDCLK) == 0);
	assert(gpio_direction(GPIO_SWDIO, true) == 0);
	assert(gpio_direction(GPIO_SWDCLK, true) == 0);

	assert(gdb_if_init() == 0);

	printf("platform_init executuon. jtag_scan returns %d", jtag_scan(NULL));

	//swdptap_init();

	return 0;
}

void platform_buffer_flush(void)
{
}

int platform_buffer_write(const uint8_t *data, int size)
{
	return size;
}

int platform_buffer_read(uint8_t *data, int size)
{
	int index = 0;
	platform_buffer_flush();
	return size;
}

#ifdef WIN32
#warning "This vasprintf() is dubious!"
int vasprintf(char **strp, const char *fmt, va_list ap)
{
	int size = 128, ret = 0;

	*strp = malloc(size);
	while(*strp && ((ret = vsnprintf(*strp, size, fmt, ap)) == size))
		*strp = realloc(*strp, size <<= 1);

	return ret;
}
#endif

const char *platform_target_voltage(void)
{
	return "not supported";
}

void platform_delay(uint32_t delay)
{
	usleep(delay * 100000);
}

void morse(const char *msg, char repeat)
{
	(void)repeat;

	if (msg != NULL)
		fprintf(stderr,"%s\n", msg);
}

