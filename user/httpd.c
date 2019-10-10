#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "ip_addr.h"
#include "mem.h"
#include "espconn.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "driver/uart_register.h"

#include "httpd.h"

LOCAL struct espconn esp_conn;
LOCAL esp_tcp esptcp;

LOCAL const char index_html[] = \
        "<!DOCTYPE html>"\
        "<html>"\
        "<body>"\
            "<h1>ESP8266 HTTP server demo </h1>"\
        "</body>"\
        "</html>";

LOCAL void ICACHE_FLASH_ATTR http_resp(struct espconn *pespconn, int error, char *html_txt){
    char *buffer = NULL;
    int html_len = 0;

    buffer = (char*) os_malloc(256*sizeof(char));
    if(buffer != NULL){
        if(html_txt != NULL){
            html_len = os_strlen(html_txt);
        }
        else{
            html_len = 0;
        }

        os_sprintf(buffer, "HTTP/1.1 %d OK\r\n"
                                    "Content-Length: %d\r\n"
                                    "Content-Type: text/html\r\n"
                                    "Connection: Closed\r\n"
                                    "\r\n"
        ,error, html_len);

        if(html_len>0){
            buffer = (char*) os_realloc(buffer,(256+html_len)*sizeof(char));
            os_strcat(buffer,html_txt);
        }

        espconn_sent(pespconn,buffer,os_strlen(buffer));
        os_free(buffer);
    }
}

LOCAL void ICACHE_FLASH_ATTR tcp_server_sent_cb(void *arg){
    os_printf("tcp data sent\r\n");
}

LOCAL void ICACHE_FLASH_ATTR tcp_server_parse(char *strIN, char *strOUT){
    char strInput[90];
    char strSplit[3][30];
    uint8 i,j,cnt;

    os_strcpy(strInput,strIN);
    j=0; cnt=0;
    for(i=0;i<=os_strlen(strInput);i++){
        if(strInput[i]==' ' || strInput[i]=='\0'){
            strSplit[cnt][j]='\0';
            cnt++;
            j=0;
        }
        else {
            if(strInput[i] != '/'){
                strSplit[cnt][j]=strInput[i];
                j++;
            }
        }
    }

    os_strcpy(strOUT,strSplit[1]);
}

LOCAL void ICACHE_FLASH_ATTR tcp_conf_parse(char *strIN, char *strOUT,uint8 num){
    char strInput[90];
    char strSplit[3][30];
    uint8 i,j,cnt;

    os_strcpy(strInput,strIN);
    j=0; cnt=0;
    for(i=0;i<=os_strlen(strInput);i++){
        if(strInput[i]==' ' || strInput[i]=='\0' || strInput[i]=='='){
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

LOCAL void ICACHE_FLASH_ATTR tcp_server_recv_cb(void *arg,char *pusrdata, unsigned short len){
    char *ptr = 0;
    char txthtml[200];
    char strRecv[64];
    char strReq[32];
    char password[64];
    struct station_config stationConf;

    struct espconn *pespconn = arg;
    ptr = (char*) os_strstr(pusrdata,"\r\n");
    ptr[0] = '\0';

// Define all HTTP request here
    os_printf("tcp recv : %s\r\n", pusrdata);
    if (os_strcmp(pusrdata, "GET / HTTP/1.1") == 0){
        http_resp(pespconn,200,(char*)index_html);
    }
    else{
        tcp_server_parse(pusrdata,strRecv);
        tcp_conf_parse(strRecv,strReq,STR_REQ);

        if(os_strcmp("pass",strReq)==0){
            tcp_conf_parse(strRecv,password,STR_DATA);
            os_printf("new password: %s\r\n",password);

            os_sprintf(txthtml,"new password: %s\r\n",password);
            http_resp(pespconn,200,(char*)txthtml);

            os_memset(stationConf.password, 0, 64);
            stationConf.bssid_set = 0;

            wifi_station_get_config(&stationConf);
            os_memcpy(&stationConf.password, password, 64);
            wifi_station_set_config(&stationConf);
        }
        if(os_strcmp("serial",strReq)==0){
            uart0_sendStr("Serial Response as HTTP request\r\n");
            os_sprintf(txthtml,"Serial Response Requested\r\n",password);
            http_resp(pespconn,200,(char*)txthtml);
        }
        else{
            http_resp(pespconn,200,NULL);
        }
    }
}

LOCAL void ICACHE_FLASH_ATTR tcp_server_disconn_cb(void *arg){
    os_printf("tcp disconnected\r\n");
}

LOCAL void ICACHE_FLASH_ATTR tcp_server_reconn_cb(void *arg,int8 err){
    os_printf("reconnecting, error code %d !!! \r\n",err);

}

LOCAL void ICACHE_FLASH_ATTR tcp_server_listen(void *arg){
    struct espconn *pesp_conn = arg;

    os_printf("tcp_server_listen !!! \r\n");

    espconn_regist_recvcb(pesp_conn, tcp_server_recv_cb);
    espconn_regist_sentcb(pesp_conn, tcp_server_sent_cb);

    espconn_regist_reconcb(pesp_conn, tcp_server_reconn_cb);
    espconn_regist_disconcb(pesp_conn, tcp_server_disconn_cb);
}

void ICACHE_FLASH_ATTR user_tcpserver_init(uint32 port){
    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;
    esp_conn.proto.tcp = &esptcp;
    esp_conn.proto.tcp->local_port = port;

    espconn_regist_connectcb(&esp_conn,tcp_server_listen);

    int8 ret = espconn_accept(&esp_conn);
    os_printf("espconn_accepted at [%d]\r\n", ret);
}

