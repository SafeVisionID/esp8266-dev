#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "ip_addr.h"
#include "espconn.h"
#include "user_interface.h"

#include "httpd.h"
#include "blinky.h"
#include "wifi_sap.h"

LOCAL void ICACHE_FLASH_ATTR user_wifi_softap_set_ip(void){

    const char* start_ip = "192.168.5.100";
    const char* end_ip = "192.168.5.105";
    struct dhcps_lease dhcplease;

    struct ip_info ipinfo;

    wifi_softap_dhcps_stop();

    IP4_ADDR(&ipinfo.ip, 192, 168, 5, 1);
    IP4_ADDR(&ipinfo.gw, 192, 168, 1, 1);
    IP4_ADDR(&ipinfo.netmask, 255, 255, 255, 0);
    wifi_set_ip_info(SOFTAP_IF,&ipinfo);

    IP4_ADDR(&dhcplease.start_ip, 192, 168, 5, 100);
    IP4_ADDR(&dhcplease.end_ip, 192, 168, 5, 105);
    wifi_softap_set_dhcps_lease(&dhcplease);

    wifi_softap_dhcps_start();
}

LOCAL void ICACHE_FLASH_ATTR user_wifi_softap_conf(void){
    struct softap_config softapConf;

    os_memset(softapConf.ssid,0,32);
    os_memset(softapConf.password,0,32);

    wifi_softap_get_config(&softapConf);

    softapConf.ssid_len = 0;
    softapConf.authmode = AUTH_WPA2_PSK;

    os_memcpy(softapConf.ssid,"SafeVisionID",12);
    os_memcpy(softapConf.password,"safevision",10);

    user_wifi_softap_set_ip();

    wifi_softap_set_config(&softapConf);

    led_wifi_softap();
}

void ICACHE_FLASH_ATTR user_wifi_softap_init(void){
    wifi_set_opmode(SOFTAP_MODE);
    user_tcpserver_init(SERVER_LOCAL_PORT);
    user_wifi_softap_conf();
}


