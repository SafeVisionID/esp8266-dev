#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "driver/gpio16.h"
#include "os_type.h"
#include "user_interface.h"

#include "blinky.h"

os_timer_t blinky_timer;
os_timer_t wifiled_timer;
static uint8_t led_on = 0,wifiled_on = 0;

LOCAL void ICACHE_FLASH_ATTR blinky_timer_handler(void *prv){
  	if (led_on==1) {
    		gpio16_output_set(1);
    		led_on=0;
  	}
  	else {
    		gpio16_output_set(0);
    		led_on=1;
  	}
}

LOCAL void ICACHE_FLASH_ATTR wifiled_timer_handler(void *prv){
    if (wifiled_on==1) {
            gpio_output_set(0, BIT2, BIT2, 0);
            wifiled_on=0;
    }
    else {
            gpio_output_set(BIT2, 0, BIT2, 0);
            wifiled_on=1;
    }
}

void ICACHE_FLASH_ATTR led_wifi_softap(void){
    gpio_output_set(0, BIT2, BIT2, 0);
}

void ICACHE_FLASH_ATTR blinky_wifi_station(void){
    os_timer_setfn(&wifiled_timer, (os_timer_func_t *)wifiled_timer_handler, NULL);
    os_timer_arm(&wifiled_timer, 100, 1);
}

void ICACHE_FLASH_ATTR blinky_init(void){
    gpio_init();
    gpio16_output_conf();

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO2_U);

    os_timer_setfn(&blinky_timer, (os_timer_func_t *)blinky_timer_handler, NULL);
    os_timer_arm(&blinky_timer, 500, 1);
}

