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
#include "gpio.h"

#define GPIO_EXPORTS "/sys/class/gpio/export"
#define GPIO_DIRECTION "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE "/sys/class/gpio/gpio%d/value"

int gpio_enable(uint8_t pin) {
	static const int bufsize = 48;
	char buf[bufsize];

	/* Enable the specified GPIO. */
	int exports = open(GPIO_EXPORTS, O_WRONLY | O_APPEND);
	if(exports < 0) {
		fprintf(stderr, "Error opening %s\n", GPIO_EXPORTS);
		return -1;
	}
	snprintf(buf, bufsize, "%d", pin);
	write(exports, buf, strlen(buf));
	close(exports);

	return 0;
}

int gpio_direction(uint8_t pin, bool output) {
	static const int bufsize = 48;
	char buf[bufsize];

	/* Set the direction. */
	snprintf(buf, bufsize, GPIO_DIRECTION, pin);
	int dir = open(buf, O_WRONLY | O_APPEND);
	if(dir < 0) {
		fprintf(stderr, "Error opening %s\n", buf);
		return -1;
	}
	write(dir, output ? "out" : "in", output ? 3 : 2);
	close(dir);

	return 0;
}

int gpio_set_value(uint8_t pin, bool value) {
	static const int bufsize = 48;
	char buf[bufsize];
	char val = value ? '1' : '0';

	/* Set the direction. */
	snprintf(buf, bufsize, GPIO_VALUE, pin);
	int dir = open(buf, O_WRONLY | O_APPEND);
	if(dir < 0) {
		fprintf(stderr, "Error opening %s\n", buf);
		return -1;
	}
	write(dir, &val, 1);
	close(dir);

	return 0;
}

int gpio_set(uint8_t pin) {
	return gpio_set_value(pin, true);
}

int gpio_clear(uint8_t pin) {
	return gpio_set_value(pin, false);
}

bool gpio_get(uint8_t pin) {
	static const int bufsize = 48;
	char buf[bufsize];

	/* Set the direction. */
	snprintf(buf, bufsize, GPIO_VALUE, pin);
	int dir = open(buf, O_RDONLY);
	if(dir < 0) {
		fprintf(stderr, "Error opening %s\n", buf);
		return false;
	}
	char value;
	read(dir, &value, 1);
	close(dir);

	return (value == '1');
}
