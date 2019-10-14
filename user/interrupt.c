#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"

LOCAL void ICACHE_FLASH_ATTR gpio_intr_handler(){
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    os_printf_plus("GPIO Interrupt Called\r\n");

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
}

void ICACHE_FLASH_ATTR user_intrr_gpio_init(void){
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO0_U);
    GPIO_DIS_OUTPUT(GPIO_ID_PIN(0));

    ETS_GPIO_INTR_DISABLE();
    ETS_GPIO_INTR_ATTACH(gpio_intr_handler, NULL);
    gpio_pin_intr_state_set(GPIO_ID_PIN(0),GPIO_PIN_INTR_ANYEDGE);
    ETS_GPIO_INTR_ENABLE();
}
