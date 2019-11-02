/*
              UNKNOWN PUBLIC LICENSE

 Copyright (C) 2019 Wira S.T. M.T.

 Currently no license applied because author liv in
 Indonesia, a country which doesn't really concern
 about digital content copyright.

 */

/**
 * @file    rwflash.h
 * @brief   Flash Memory header.
 *
 * @addtogroup Storage
 * @{
 */

#ifndef RWFLASH_H
#define RWFLASH_H

/**
 * @brief Flash memory storage start address
 */
#define FLASH_START_ADDR    0x80

#define FLASH_USERID_ADDR 0x81
#define FLASH_DEVSID_ADDR 0x82

/**
 * @brief Test Flash Length
 */
#define TEST_FLASH_STRLEN   16

/**
 * @brief Flash write integer
 * @param[in] Write Address
 * @param[in] Integer input array
 */
void rwflash_int_write(uint16 sec,uint16 *src);

/**
 * @brief Flash read integer
 * @param[in] read Address
 * @param[out] Integer output array
 */
void rwflash_int_read(uint16 sec,uint16 *des);

/**
 * @brief Flash write string
 * @param[in] Write Address
 * @param[in] String input
 */
void rwflash_str_write(uint16 sec,char *src);

/**
 * @brief Flash read string
 * @param[in] Read Address
 * @param[out] String output
 */
void rwflash_str_read(uint16 sec,char *des);

#endif // RWFLASH_H
/** @} */
