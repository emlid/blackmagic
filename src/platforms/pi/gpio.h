#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include <stdbool.h>

extern int gpio_enable(uint8_t pin);
extern int gpio_direction(uint8_t pin, bool output);
extern int gpio_set_value(uint8_t pin, bool value);
extern int gpio_set(uint8_t pin);
extern int gpio_clear(uint8_t pin);
extern bool gpio_get(uint8_t pin);

#endif /* __GPIO_H */
