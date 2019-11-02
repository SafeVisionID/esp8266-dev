/*
              UNKNOWN PUBLIC LICENSE

 Copyright (C) 2019 Wira S.T. M.T.

 Currently no license applied because author liv in
 Indonesia, a country which doesn't really concern
 about digital content copyright.

 */

/**
 * @file    rwflash.c
 * @brief   Flash Memory code.
 *
 * @addtogroup Storage
 * @{
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "spi_flash.h"
#include "user_interface.h"

#include "rwflash.h"

char strConfigs[FLASH_CONFIGS_LEN];

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

void ICACHE_FLASH_ATTR rwflash_conf_parse(char *strIN, char *strOUT, uint8 num){
    char strInput[FLASH_CONFIGS_LEN];
    char strSplit[8][FLASH_CONFIGS_LEN/8];
    uint8 i,j,cnt;

    os_strcpy(strInput,strIN);
    j=0; cnt=0;
    for(i=0;i<=os_strlen(strInput);i++){
        if(strInput[i]==' ' || strInput[i]=='\0' || strInput[i]==';'){
            strSplit[cnt][j]='\0';
            cnt++;
            j=0;
        }
        else {
            strSplit[cnt][j]=strInput[i];
            j++;
        }
    }

    os_strcpy(strOUT,strSplit[num]);
}
/** @} */
