#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <err.h>

#include "platform.h"
#include "gpio.h"

#define LOW                 0
#define HIGH                1

#define BCM2708_PERI_BASE   0x20000000
#define GPIO_BASE           (BCM2708_PERI_BASE + 0x200000)
#define PAGE_SIZE           (4*1024)
#define BLOCK_SIZE          (4*1024)

// GPIO setup. Always use INP_GPIO(x) before OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define GPIO_MODE_IN(g)     *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define GPIO_MODE_OUT(g)    *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_MODE_ALT(g,a)  *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET_HIGH       *(gpio+7)  // sets   bits which are 1
#define GPIO_SET_LOW        *(gpio+10) // clears bits which are 1
#define GPIO_GET(g)         (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define RPI_GPIO_2    2    // Pin 3
#define RPI_GPIO_3    3    // Pin 5
#define RPI_GPIO_4    4    // Pin 7
#define RPI_GPIO_7    7    // Pin 26
#define RPI_GPIO_8    8    // Pin 24
#define RPI_GPIO_9    9    // Pin 21
#define RPI_GPIO_10   10   // Pin 19
#define RPI_GPIO_11   11   // Pin 23
#define RPI_GPIO_14   14   // Pin 8
#define RPI_GPIO_15   15   // Pin 10
#define RPI_GPIO_17   17   // Pin 11
#define RPI_GPIO_18   18   // Pin 12
#define RPI_GPIO_22   22   // Pin 15
#define RPI_GPIO_23   23   // Pin 16
#define RPI_GPIO_24   24   // Pin 18
#define RPI_GPIO_25   25   // Pin 22
#define RPI_GPIO_27   27   // Pin 13
#define RPI_GPIO_28   28   // Pin 3
#define RPI_GPIO_29   29   // Pin 4
#define RPI_GPIO_30   30   // Pin 5
#define RPI_GPIO_31   31   // Pin 6

volatile uint32_t *gpio = NULL;

int gpio_enable(uint8_t pin) {
    int mem_fd;
    void* gpio_map;

    if (gpio != NULL) {
        return 0;
    }

    mem_fd = open("/dev/mem", O_RDWR | O_SYNC);

    if (mem_fd < 0) {
        err(EXIT_FAILURE, "/dev/mem");
    }

    gpio_map = mmap(NULL, BLOCK_SIZE,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED,
                          mem_fd,
                          GPIO_BASE
                   );

    if (gpio_map == MAP_FAILED) {
        err(1, "mmap");
    }

    gpio = (volatile uint32_t *)gpio_map; // Always use volatile pointer!

    if (close(mem_fd) < 0) {
        err(1, "/dev/mem");
    }

    return 0;
}

int gpio_direction(uint8_t pin, bool output) 
{
    if (output == 0) {
        GPIO_MODE_IN(pin);
    } else {
        GPIO_MODE_IN(pin);
        GPIO_MODE_OUT(pin);
    }

    return 0;
}

int gpio_set_value(uint8_t pin, bool value) 
{
    if (value == LOW) {
        GPIO_SET_LOW = 1 << pin;
    } else {
        GPIO_SET_HIGH = 1 << pin;
    }

    return 0;
}

int gpio_set(uint8_t pin) 
{
    return gpio_set_value(pin, true);
}

int gpio_clear(uint8_t pin) 
{
    return gpio_set_value(pin, false);
}

bool gpio_get(uint8_t pin) 
{
    /* SWD controllers might be too slow for RPi2.
     * That's why we need to stretch a little. */

    usleep(1);

    uint32_t value = GPIO_GET(pin);

    return value? true: false;
}
