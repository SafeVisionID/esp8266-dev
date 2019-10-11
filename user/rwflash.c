#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "spi_flash.h"
#include "user_interface.h"

#include "rwflash.h"

void ICACHE_FLASH_ATTR rwflash_one_write(uint16 sec,uint32 *src){
    spi_flash_erase_sector(sec);
    spi_flash_write(sec * SPI_FLASH_SEC_SIZE, src, 1);
}

void ICACHE_FLASH_ATTR rwflash_one_read(uint16 sec,uint32 *des){
    spi_flash_read(sec * SPI_FLASH_SEC_SIZE, des, 1);
}
