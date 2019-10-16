#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"

#include "interrupt.h"
#include "httpd.h"

#if GPIO_INTRR_USE_POLL

LOCAL os_timer_t gpio_poll_timer;
LOCAL uint8 gpio12_stt = 0;

LOCAL void ICACHE_RODATA_ATTR gpio_poll_handler(){
    if( !(GPIO_INPUT_GET(GPIO_ID_PIN(GPIO_INTRR_PINNUM))) ){
        if(gpio12_stt==0){
            os_printf_plus("WiFi Mode Switched !!!\r\n");
            user_wifi_switch();
            gpio12_stt = 1;
        }
    }
    else{
        gpio12_stt = 0;
    }
}

void ICACHE_FLASH_ATTR user_poll_gpio_init(void){
    PIN_FUNC_SELECT(GPIO_INTRR_PINNAME, GPIO_INTRR_PINFUNC);
    PIN_PULLUP_EN(GPIO_INTRR_PINNAME);
    GPIO_DIS_OUTPUT(GPIO_ID_PIN(GPIO_INTRR_PINNUM));

    os_timer_setfn(&gpio_poll_timer, (os_timer_func_t *)gpio_poll_handler, NULL);
    os_timer_arm(&gpio_poll_timer, 1, 1);
}
#else

LOCAL void gpio_intrr_handler(void *arg){
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    if (gpio_status & BIT(GPIO_INTRR_PINNUM)){
        gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_INTRR_PINNUM), GPIO_PIN_INTR_DISABLE);
        GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(GPIO_INTRR_PINNUM));

        os_printf_plus("GPIO Interrupt Triggered\r\n");

        gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_INTRR_PINNUM), GPIO_PIN_INTR_NEGEDGE);
    }
}

void ICACHE_FLASH_ATTR user_intrr_gpio_init(void){
    ETS_GPIO_INTR_ATTACH(gpio_intrr_handler,NULL);
    ETS_GPIO_INTR_DISABLE();

    PIN_FUNC_SELECT(GPIO_INTRR_PINNAME, GPIO_INTRR_PINFUNC);
    PIN_PULLUP_EN(GPIO_INTRR_PINNAME);
    GPIO_DIS_OUTPUT(GPIO_ID_PIN(GPIO_INTRR_PINNUM));

    gpio_register_set(GPIO_PIN_ADDR(GPIO_ID_PIN(GPIO_INTRR_PINNUM)),
                            GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE) |
                            GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) |
                            GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(GPIO_INTRR_PINNUM));

    gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_INTRR_PINNUM), GPIO_PIN_INTR_NEGEDGE);

    ETS_GPIO_INTR_ENABLE();
}
#endif
