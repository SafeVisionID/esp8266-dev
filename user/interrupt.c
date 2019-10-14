#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"

LOCAL os_timer_t gpio_poll_timer;
LOCAL uint8 gpio12_stt = 0;

LOCAL void ICACHE_RODATA_ATTR gpio_poll_handler(){
    if( !(GPIO_INPUT_GET(GPIO_ID_PIN(12))) ){
        if(gpio12_stt==0){
            os_printf_plus("GPIO Interrupt Triggered\r\n");
            gpio12_stt = 1;
        }
    }
    else{
        gpio12_stt = 0;
    }
}

void ICACHE_FLASH_ATTR user_poll_gpio_init(void){
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);
    GPIO_DIS_OUTPUT(GPIO_ID_PIN(12));

    os_timer_setfn(&gpio_poll_timer, (os_timer_func_t *)gpio_poll_handler, NULL);
    os_timer_arm(&gpio_poll_timer, 1, 1);
}
