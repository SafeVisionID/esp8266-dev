#ifndef _BLINKY_
#define _BLINKY_

#define LED_WIFI_ON     1
#define LED_WIFI_OFF    0

#define LED16_BLINKY    0

void blinky_init(void);
void blinky_wifi_none(void);
void blinky_wifi_softap(void);
void blinky_wifi_station(void);

#endif
