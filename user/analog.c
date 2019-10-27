#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"

#include "analog.h"

uint16 ICACHE_RODATA_ATTR user_get_adc(void){
    uint16 vres;

    uint8 i;
    for(i=0;i<ADC_MEASURES_NUM;i++){
        vres += system_adc_read();
    }

    return vres / ADC_MEASURES_NUM;
}
