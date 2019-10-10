#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "ip_addr.h"
#include "mem.h"
#include "espconn.h"
#include "user_interface.h"

#include "httpd.h"
#include "wifi_sta.h"

LOCAL os_timer_t ip_test_timer;

LOCAL void ICACHE_FLASH_ATTR user_wifi_station_check_ip(void){
    struct ip_info ipconfig;

    os_timer_disarm(&ip_test_timer);

    wifi_get_ip_info(STATION_IF,&ipconfig);

    if(wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr !=0 ){
        os_printf("got ip !!! \r\n");
        user_tcpserver_init(SERVER_LOCAL_PORT);
    }else{
        if (wifi_station_get_connect_status() == STATION_WRONG_PASSWORD ||
            wifi_station_get_connect_status() == STATION_NO_AP_FOUND ||
            wifi_station_get_connect_status() == STATION_CONNECT_FAIL){

            os_printf("connect fail !!! \r\n");
        }
        else{
            os_timer_setfn(&ip_test_timer, (os_timer_func_t *)user_wifi_station_check_ip, NULL);
            os_timer_arm(&ip_test_timer, 100, 0);
        }
    }
}

LOCAL void ICACHE_FLASH_ATTR user_wifi_station_conf(void){
    struct station_config stationConf;
    char ssid[32];
    char password[64];

    os_memset(stationConf.ssid, 0, 32);
    os_memset(stationConf.password, 0, 64);
    stationConf.bssid_set = 0;

    wifi_station_get_config(&stationConf);

    if(os_strlen(stationConf.ssid)==0){
        os_printf("Using default ssid\r\n");
        os_strcpy(ssid,"AchmadiGamePhone");
        os_memcpy(&stationConf.ssid, ssid, 32);
    }else{
        os_printf("Using previous ssid: %s\r\n",stationConf.ssid);
    }

    if(os_strlen(stationConf.password)==0){
        os_printf("Using default password\r\n");
        os_strcpy(password,"abu_musa2015");
        os_memcpy(&stationConf.password, password, 64);
    }else{
        os_printf("Using previous password: %s\r\n",stationConf.password);
    }

    wifi_station_set_config(&stationConf);

    os_timer_disarm(&ip_test_timer);
    os_timer_setfn(&ip_test_timer, (os_timer_func_t *)user_wifi_station_check_ip, NULL);
    os_timer_arm(&ip_test_timer, 100, 0);
}

void ICACHE_FLASH_ATTR user_wifi_station_init(void){
    wifi_set_opmode(STATION_MODE);
#if USE_MAX_POWER
    wifi_set_phy_mode(PHY_MODE_11N);
#endif
    user_wifi_station_conf();
}
