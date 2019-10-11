#ifndef _BLINKY_
#define _BLINKY_

#define USE_GPIO2_BLINK 0
#define LED_WIFI_ON     1
#define LED_WIFI_OFF    0

void blinky_init(void);
void blinky_wifi(uint8 wifiled);

#endif
