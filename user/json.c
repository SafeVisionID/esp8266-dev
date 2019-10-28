/*
              UNKNOWN PUBLIC LICENSE

 Copyright (C) 2019 Wira S.T. M.T.

 Currently no license applied because author liv in
 Indonesia, a country which doesn't really concern
 about digital content copyright.

 */

/**
 * @file    json.c
 * @brief   JSON String Builder code.
 *
 * @addtogroup Webserver
 * @{
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "spi_flash.h"
#include "user_interface.h"

#include "json.h"

void json_open(char* strJSON){
    os_strcpy(strJSON,"{");
}

void json_close(char* strJSON){
    strJSON[os_strlen(strJSON)-1] = '}';
}

void json_string(char* strJSON, char* field, char* value){
    os_sprintf(strJSON,"%s\"%s\": \"%s\",",strJSON,field,value);
}

void json_boolean(char* strJSON, char* field, uint8 value){
    if(value==0){
        os_sprintf(strJSON,"%s\"%s\": false,",strJSON,field);
    }
    else if (value==1) {
        os_sprintf(strJSON,"%s\"%s\": true,",strJSON,field);
    }
}
/** @} */
