#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "driver/gpio16.h"
#include "os_type.h"
#include "user_interface.h"

#include "blinky.h"

LOCAL os_timer_t blinky_timer;
LOCAL os_timer_t wifista_timer;
LOCAL os_timer_t wifisap_timer;
LOCAL os_timer_t wifinon_timer;

LOCAL uint8_t wifi_led = 0;

#if LED16_BLINKY
LOCAL uint8 blink_led = 0;
#endif

LOCAL void ICACHE_RODATA_ATTR user_wifi_blink_off(void){
    os_timer_disarm(&wifisap_timer);
    os_timer_disarm(&wifista_timer);
    os_timer_disarm(&wifinon_timer);
    wifi_led = 0;
}

LOCAL void ICACHE_FLASH_ATTR wifi_on_timer_handler(void *prv){
    if (wifi_led==1) { gpio_output_set(0, BIT2, BIT2, 0); wifi_led=0; }
    else { gpio_output_set(BIT2, 0, BIT2, 0); wifi_led=1; }
}

LOCAL void ICACHE_FLASH_ATTR wifi_off_timer_handler(void *prv){
    if (wifi_led==11 || wifi_led==13) {
        gpio_output_set(BIT2, 0, BIT2, 0);

    }
    else if(wifi_led==10 || wifi_led==12) {
        gpio_output_set(0, BIT2, BIT2, 0);
    }
    else if(wifi_led==14){
        wifi_led = 0;
    }else{
        gpio_output_set(BIT2, 0, BIT2, 0);
    }

    wifi_led++;
}

#if LED16_BLINKY
LOCAL void ICACHE_FLASH_ATTR blinky_timer_handler(void *prv){
    if (blink_led==1) { gpio16_output_set(1); blink_led=0; }
    else { gpio16_output_set(0); blink_led=1; }
}
#endif

void ICACHE_FLASH_ATTR blinky_wifi_softap(void){
    user_wifi_blink_off();

    os_timer_setfn(&wifisap_timer, (os_timer_func_t *)wifi_on_timer_handler, NULL);
    os_timer_arm(&wifisap_timer, 500, 1);
}

void ICACHE_FLASH_ATTR blinky_wifi_station(void){
    user_wifi_blink_off();

    os_timer_setfn(&wifista_timer, (os_timer_func_t *)wifi_on_timer_handler, NULL);
    os_timer_arm(&wifista_timer, 100, 1);
}

void ICACHE_FLASH_ATTR blinky_wifi_none(void){
    user_wifi_blink_off();

    os_timer_setfn(&wifinon_timer, (os_timer_func_t *)wifi_off_timer_handler, NULL);
    os_timer_arm(&wifinon_timer, 100, 1);
}

void ICACHE_FLASH_ATTR blinky_init(void){
    gpio_init();

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO2_U);

#if LED16_BLINKY
    gpio16_output_conf();
    os_timer_setfn(&blinky_timer, (os_timer_func_t *)blinky_timer_handler, NULL);
    os_timer_arm(&blinky_timer, 500, 1);
#endif
}
