#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "driver/gpio16.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"

#include "blinky.h"

os_timer_t blinky_timer;
static uint8_t led_on = 0;

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

void blinky_wifi(uint8 wifiled){
    switch(wifiled){
        case LED_WIFI_OFF: wifi_status_led_uninstall();break;
        case LED_WIFI_ON: wifi_status_led_install(2,PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);break;
        default: wifi_status_led_uninstall();break;
    }
}

void ICACHE_FLASH_ATTR blinky_init(void){
    gpio_init();
    gpio16_output_conf();

    os_timer_setfn(&blinky_timer, (os_timer_func_t *)blinky_timer_handler, NULL);
    os_timer_arm(&blinky_timer, 500, 1);

    blinky_wifi(LED_WIFI_OFF);
}

