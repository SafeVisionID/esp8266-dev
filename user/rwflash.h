#ifndef RWFLASH_H
#define RWFLASH_H

#define TEST_FLASH_ADDR 0x8c

void rwflash_one_write(uint16 sec,uint32 *src);
void rwflash_one_read(uint16 sec,uint32 *des);

#endif // RWFLASH_H
