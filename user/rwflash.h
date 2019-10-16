#ifndef RWFLASH_H
#define RWFLASH_H

#define TEST_FLASH_ADDR     0x80
#define TEST_FLASH_STRLEN   8

void rwflash_int_write(uint16 sec,uint16 *src);
void rwflash_int_read(uint16 sec,uint16 *des);
void rwflash_str_write(uint16 sec,char *src);
void rwflash_str_read(uint16 sec,char *des);

#endif // RWFLASH_H
