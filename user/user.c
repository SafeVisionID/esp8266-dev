#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"

#include "driver/uart.h"
#include "driver/gpio16.h"

#include "blinky.h"
#include "httpd.h"
#include "wifi_sap.h"
#include "wifi_sta.h"

#define USE_SOFTAP 1

#define SERIALBAUD 9600

extern os_timer_t blinky_timer;

void ICACHE_FLASH_ATTR print_os_printf(){
  uint8 bootmode;

  os_printf("\r\n\r\n");
  os_printf("[INFO] -------------------------------------------\r\n");

  os_printf("[INFO] Compiled at %s %s\r\n", __DATE__,__TIME__);
  os_printf("[INFO] SDK: %s\r\n", system_get_sdk_version());
  os_printf("[INFO] Chip ID: %08X\r\n", system_get_chip_id());
  os_printf("[INFO] BOOT Version: %d\r\n",system_get_boot_version());
  os_printf("[INFO] BIN User addr: 0x%08X\r\n",system_get_userbin_addr());

  bootmode = system_get_boot_mode();
  if(bootmode == 0){ os_printf("[INFO] Boot using Enhance Mode\r\n"); }
  else{os_printf("[INFO] Boot using Normal Mode\r\n");}

  os_printf("[INFO] CPU Freq: %d MHz\r\n", system_get_cpu_freq());
  os_printf("[INFO] Memory Info:\r\n"); system_print_meminfo();

  os_printf("[INFO] -------------------------------------------\r\n");
  os_printf("\r\n\r\n");
}

void ICACHE_FLASH_ATTR user_init(){
    uart_init(SERIALBAUD,SERIALBAUD);
    uart_rx_intr_enable(UART0);
    print_os_printf();

	gpio_init();
	gpio_output_set(0, 0, (1 << 2), 0);
	gpio16_output_conf();

	os_timer_setfn(&blinky_timer, (os_timer_func_t *)blinky_timer_handler, NULL);
    os_timer_arm(&blinky_timer, 500, 1);

#if USE_SOFTAP
    user_wifi_softap_init();
#else
    user_wifi_station_init();
#endif

}

