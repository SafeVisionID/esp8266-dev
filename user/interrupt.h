#ifndef PIN_INTERRUPT_H
#define PIN_INTERRUPT_H

#include "ets_sys.h"

#define GPIO_INTRR_USE_POLL 0

#define GPIO_INTRR_PINNUM   12
#define GPIO_INTRR_PINNAME  PERIPHS_IO_MUX_MTDI_U
#define GPIO_INTRR_PINFUNC  FUNC_GPIO12

#if GPIO_INTRR_USE_POLL
void user_poll_gpio_init(void);
#else
void user_intrr_gpio_init(void);
#endif

#endif // PIN_INTERRUPT_H
