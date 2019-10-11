#ifndef HTTPD_H
#define HTTPD_H

#define SERVER_LOCAL_PORT         8080

#define STR_REQ     0
#define STR_DATA    1

#define TEST_FLASH_ADDR 0x8c

void user_tcpserver_init(uint32 port);

#endif // HTTPD_H
