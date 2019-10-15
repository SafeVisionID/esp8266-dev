#ifndef HTTPD_H
#define HTTPD_H

#define SERVER_LOCAL_PORT   3000

#define STR_REQ     1
#define STR_DATA    2

#define SWITCH_REBOOT       1

void user_tcpserver_init(uint32 port);
void user_wifi_switch(void);

#endif // HTTPD_H
