#ifndef PIN_INTERRUPT_H
#define PIN_INTERRUPT_H

#include "ets_sys.h"

#define GPIO_INTRR_USE_POLL 1

#define MAGNET_INTRR_PINNUM   12
#define MAGNET_INTRR_PINNAME  PERIPHS_IO_MUX_MTDI_U
#define MAGNET_INTRR_PINFUNC  FUNC_GPIO12

#define PIR_INTRR_PINNUM   13
#define PIR_INTRR_PINNAME  PERIPHS_IO_MUX_MTCK_U
#define PIR_INTRR_PINFUNC  FUNC_GPIO13

#define SW_INTRR_PINNUM   14
#define SW_INTRR_PINNAME  PERIPHS_IO_MUX_MTMS_U
#define SW_INTRR_PINFUNC  FUNC_GPIO14

#if GPIO_INTRR_USE_POLL
void user_poll_gpio_init(void);
#else
void user_intrr_gpio_init(void);
#endif

#endif // PIN_INTERRUPT_H
