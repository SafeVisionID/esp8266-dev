#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "spi_flash.h"
#include "user_interface.h"

#include "rwflash.h"

LOCAL void ICACHE_FLASH_ATTR rwflash_write(uint16 sec,uint32 *src){
    spi_flash_erase_sector(sec);
    spi_flash_write(sec * SPI_FLASH_SEC_SIZE, src, 4*sizeof(src));
}

LOCAL void ICACHE_FLASH_ATTR rwflash_read(uint16 sec,uint32 *des){
    spi_flash_read(sec * SPI_FLASH_SEC_SIZE, des, 4*sizeof(des));
}

void ICACHE_FLASH_ATTR rwflash_int_write(uint16 sec, uint16 *src){
    rwflash_write(sec, (uint32 *)src);
}

void ICACHE_FLASH_ATTR rwflash_int_read(uint16 sec, uint16 *des){
    rwflash_read(sec, (uint32 *)des);
}

void ICACHE_FLASH_ATTR rwflash_str_write(uint16 sec, char *src){
    rwflash_write(sec, (uint32 *)src);
}

void ICACHE_FLASH_ATTR rwflash_str_read(uint16 sec, char *des){
    rwflash_read(sec, (uint32 *)des);
}
