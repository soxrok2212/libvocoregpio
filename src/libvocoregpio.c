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
#include <sys/mman.h>
#include <fcntl.h>

/*
 * GPIO base address
 * 
 * Each register is 32 bits, thus our offsets must be 32 bits as well
 */

#define GPIO_BASE_ADDRESS	0x10000600

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

#define GPIO_CTRL_0		GPIO_BASE_ADDRESS
#define GPIO_CTRL_1		GPIO_BASE_ADDRESS + 0x4
#define GPIO_CTRL_2		GPIO_BASE_ADDRESS + 0x8

/*
 * GPIO_POL
 *
 * Controls the polarity of the data (high+read/low+write, high+write/low+read)
 */

#define GPIO_POL_0		GPIO_BASE_ADDRESS + 0x10
#define GPIO_POL_1		GPIO_BASE_ADDRESS + 0x14
#define GPIO_POL_2		GPIO_BASE_ADDRESS + 0x18

/*
 * GPIO_DATA
 * 
 * Stores the current GPIO data
 */

#define GPIO_DATA_0		GPIO_BASE_ADDRESS + 0x20
#define GPIO_DATA_1		GPIO_BASE_ADDRESS + 0x24
#define GPIO_DATA_2		GPIO_BASE_ADDRESS + 0x28

/*
 * GPIO_DSET
 * 
 * Sets bits in GPIO_DATA registers
 */

#define GPIO_DSET_0		GPIO_BASE_ADDRESS + 0x30
#define GPIO_DSET_1		GPIO_BASE_ADDRESS + 0x34
#define GPIO_DSET_2		GPIO_BASE_ADDRESS + 0x38

/*
 * GPIO_DCLR
 *
 * Clears bits in GPIO_DATA registers
 */

#define GPIO_DCLR_0		GPIO_BASE_ADDRESS + 0x40
#define GPIO_DCLR_1		GPIO_BASE_ADDRESS + 0x44
#define GPIO_DCLR_2		GPIO_BASE_ADDRESS + 0x48

/*
 * GINT_REDGE
 *
 * Enables the condition of rising edge triggered interrupt
 */

#define GINT_REDGE_0		GPIO_BASE_ADDRESS + 0x50
#define GINT_REDGE_1		GPIO_BASE_ADDRESS + 0x54
#define GINT_REDGE_2		GPIO_BASE_ADDRESS + 0x58

/*
 * GINT_FEDGE
 *
 * Enables the condition of falling edge triggered interrupt
 */

#define GINT_FEDGE_0		GPIO_BASE_ADDRESS + 0x60
#define GINT_FEDGE_1		GPIO_BASE_ADDRESS + 0x64
#define GINT_FEDGE_2		GPIO_BASE_ADDRESS + 0x68

/*
 * GINT_HLVL
 *
 * Enables the condition of high level triggered interrupt
 */

#define GINT_HLVL_0		GPIO_BASE_ADDRESS + 0x70
#define GINT_HLVL_1		GPIO_BASE_ADDRESS + 0x74
#define GINT_HLVL_2		GPIO_BASE_ADDRESS + 0x78

/*
 * GINT_LLVL
 *
 * Enables the condition of low level triggered interrupt
 */

#define GINT_LLVL_0		GPIO_BASE_ADDRESS + 0x80
#define GINT_LLVL_1		GPIO_BASE_ADDRESS + 0x84
#define GINT_LLVL_2		GPIO_BASE_ADDRESS + 0x88

/*
 * GINT_STAT
 *
 * Records the GPIO current interrupt status
 */

#define GINT_STAT_0		GPIO_BASE_ADDRESS + 0x90
#define GINT_STAT_1		GPIO_BASE_ADDRESS + 0x94
#define GINT_STAT_2		GPIO_BASE_ADDRESS + 0x98

/*
 * GINT_EDGE
 *
 * Records the GPIO current interrupt's edge status
 */

#define GINT_EDGE_0		GPIO_BASE_ADDRESS + 0xA0
#define GINT_EDGE_1		GPIO_BASE_ADDRESS + 0xA4
#define GINT_EDGE_2		GPIO_BASE_ADDRESS + 0xA8

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

void setDirection() {

}

void setPolarity() {

}

void setData() {

}
void clearData() {

}

int main(int argc, char *argv[]){

}
