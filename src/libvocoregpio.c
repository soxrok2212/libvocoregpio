/*
 * libvocoregpio: A GPIO driver for VoCore v2 https://vocore.io/v2.html
 *
 * Copyright (c) 2019-2020, soxrok2212 <soxrok2212@gmail.com>
 * SPDX-License-Identifier: GPL-3.0+
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* 
 * This is a driver to use GPIO efficenitly on Vonger's VoCore v2.
 * It has been built using the MT7628 datasheet as reference http://vonger.cn/upload/MT7628_Full.pdf
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

/*
 * Register definitions
 * 
 * GPIO_*_0 controls GPIO0...31
 * GPIO_*_1 controls GPIO32...64
 * GPIO_*_2 controls GPIO65...95
 */

/* 
 * GPIO_CTRL
 * 
 * Controls the direction of data per register (input/output)
 */

#define GPIO_CTRL	0x600

/*
 * GPIO_POL
 *
 * Controls the polarity of the data (high+read/low+write, high+write/low+read)
 */

#define GPIO_POL	0x610

/*
 * GPIO_DATA
 * 
 * Stores the current GPIO data
 */

#define GPIO_DATA	0x620

/*
 * GPIO_DSET
 * 
 * Sets bits in GPIO_DATA registers
 */

#define GPIO_DSET	0x630

/*
 * GPIO_DCLR
 *
 * Clears bits in GPIO_DATA registers
 */

#define GPIO_DCLR	0x640

/*
 * GINT_REDGE
 *
 * Enables the condition of rising edge triggered interrupt
 */

#define GINT_REDGE	0x650

/*
 * GINT_FEDGE
 *
 * Enables the condition of falling edge triggered interrupt
 */

#define GINT_FEDGE	0x660

/*
 * GINT_HLVL
 *
 * Enables the condition of high level triggered interrupt
 */

#define GINT_HLVL	0x670

/*
 * GINT_LLVL
 *
 * Enables the condition of low level triggered interrupt
 */

#define GINT_LLVL	0x680

/*
 * GINT_STAT
 *
 * Records the GPIO current interrupt status
 */

#define GINT_STAT	0x690

/*
 * GINT_EDGE
 *
 * Records the GPIO current interrupt's edge status
 */

#define GINT_EDGE	0x6A0

/*
 * MMAP_PATH
 *
 * Set /dev/mem as path for reading GPIO pins
 */

#define MMAP_PATH 		"/dev/mem"

static uint8_t *gpio_mmap_reg = NULL; // create pointer to register map
static int gpio_mmap_fd = 0; // create file descriptor for /dev/mem gpio map

/*
 * gpio_mmap
 *
 * Attempt to open /dev/mem and store it in a pointer
 */

static int gpio_mmap(void) {
	// open will return <0 if there was a problem opening /dev/mem
	if ((gpio_mmap_fd = open(MMAP_PATH, O_RDWR)) < 0) {
		fprintf(stderr, "[!] Unable to open /dev/mem as read write\n");
		return -1;
	}
	// create new mapping starting at address 0x10000000
	gpio_mmap_reg = (uint8_t*)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, gpio_mmap_fd, 0x10000000);
	if (gpio_mmap_reg == MAP_FAILED) {
		fprintf(stderr, "[!] Failed to create a mapping\n");
		gpio_mmap_reg = NULL;
		return -1;
	}
	// close the fd after fail/successful mapping
	close(gpio_mmap_fd);
	return 0;
}

void invalidPin(void) {
	fprintf(stderr, "[!] pin > 95 specified\n");
	exit(1);
}

void setDirection(uint8_t pin, int direction) {
	// create a 32-bit unsigned int
	uint32_t val = 0;
	
	if (pin < 32) {
		val = *(volatile uint32_t *)(gpio_mmap_reg + GPIO_CTRL);
		// use bitwise logic to only affect one bit
		if (direction) {
			val |= (1u << pin);
		}
		else {
			val &= ~(1u << pin);
		}
	}
	else if (pin < 64) {
		val = *(volatile uint32_t *)(gpio_mmap_reg + GPIO_CTRL + 0x04);
		if (direction) {
			val |= (1u << (pin-32));
		}
		else {
			val &= (1u << (pin-32));
		}
	}
	else if (pin < 96) {
		val = *(volatile uint32_t *)(gpio_mmap_reg + GPIO_CTRL + 0x08);
		if (direction) {
			val |= (1u << (pin-64));
		}
		else {
			val &= (1u << (pin-64));
		}
	}
	else {
		invalidPin();
	}
}

void setPolarity(uint8_t pin, int polarity) {
	uint32_t val = 0;

}

void setData(uint8_t pin, int data) {
	uint32_t val = 0;
	
	if (pin < 32) {
		val = (1u << pin);
		if (data) {
			*(volatile uint32_t *)(gpio_mmap_reg + GPIO_DSET) = val;
		}
	}
	else if (pin < 64) {
		val = (1u << (pin-32));
		if (data) {
			*(volatile uint32_t *)(gpio_mmap_reg + GPIO_DSET + 0x04) = val;
		}
	}
	else if (pin < 96) {
		val = (1u << (pin-64));
		if (data) {
			*(volatile uint32_t *)(gpio_mmap_reg + GPIO_DSET + 0x08) = val;
		}
	}
	else {
		invalidPin();
	}
}

int getData(uint8_t pin) {
	uint32_t val = 0;
	if (pin < 32) {
		val = *(volatile uint32_t *)(gpio_mmap_reg + GPIO_DATA);
		val = (val >> pin) & 1u;
	}
	else if (pin < 64) {
		val = *(volatile uint32_t *)(gpio_mmap_reg + GPIO_DATA + 0x04);
		val = (val >> (pin-32)) & 1u;
	}
	else if (pin < 96) {
		val = *(volatile uint32_t *)(gpio_mmap_reg + GPIO_DATA + 0x08);
		val = (val >> (pin-64)) & 1u;
		val = (val >> (pin-24)) & 1u;
	}
	else {
		invalidPin();
	}
	return val;
}

void clearData(uint8_t pin) {
	uint32_t val = 0;

}

int main(int argc, char *argv[]){
// run the mapping function
if (gpio_mmap()) {
	return -1;

}
return 0;
}
