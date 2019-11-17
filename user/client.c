/*
              UNKNOWN PUBLIC LICENSE

 Copyright (C) 2019 Wira S.T. M.T.

 Currently no license applied because author liv in
 Indonesia, a country which doesn't really concern
 about digital content copyright.

 */

/**
 * @file    client.c
 * @brief   TCP Client code.
 *
 * @addtogroup Webclient
 * @{
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "ip_addr.h"
#include "mem.h"
#include "espconn.h"
#include "user_interface.h"

#include "client.h"

/**
 * @brief HTTP Client request arguments
 */
typedef struct {
    char * path;
    int port;
    char * post_data;
    char * headers;
    char * hostname;
    char * buffer;
    int buffer_size;
    bool secure;
} request_args;

/**
 * @brief String dumping
 * @param[in] String input
 * @return Processed string
 */
LOCAL char * ICACHE_FLASH_ATTR esp_strdup(const char * str){
    if(str == NULL){ return NULL;	}

    char * new_str = (char *)os_malloc(os_strlen(str) + 1);
    if(new_str == NULL){ os_printf("esp_strdup: malloc error\r\n"); return NULL; }
    os_strcpy(new_str, str);

    return new_str;
}

/**
 * @brief Basic/RAW client request
 * @param[in] Hostname URL
 * @param[in] Port number
 * @param[in] Secure SSL
 * @param[in] Path URL
 * @param[in] POST data
 * @param[in] Data headers
 */
LOCAL void ICACHE_FLASH_ATTR tcp_client_raw(const char * hostname, int port, bool secure, const char * path, const char * post_data, const char * headers){
    request_args * req = (request_args *)os_malloc(sizeof(request_args));

    req->hostname = esp_strdup(hostname);
    req->path = esp_strdup(path);
    req->port = port;
    req->secure = secure;
    req->headers = esp_strdup(headers);
    req->post_data = esp_strdup(post_data);

    req->buffer_size = 1;
    req->buffer = (char *)os_malloc(1);
    req->buffer[0] = '\0';

    ip_addr_t addr;
    err_t error = espconn_gethostbyname((struct espconn *)req,hostname,&addr,NULL);

    if(error==ESPCONN_OK){os_printf("DNS resolved\r\n");}
    else if(error==ESPCONN_INPROGRESS){os_printf("DNS pending\r\n");}
    else if(error==ESPCONN_ARG){os_printf("DNS argument error %s\n",hostname);}
    else{os_printf("DNS error code %d\n", error);}
}

/**
 * @brief HTTP client POST request
 * @param Request URL
 * @param POST data
 * @param Data headers
 */
LOCAL void ICACHE_FLASH_ATTR tcp_client_post(const char * url, const char * post_data, const char * headers){
    char hostname[128] = "";
    int port = 80;
    bool secure = false;

    bool is_http  = os_strncmp(url, "http://",  strlen("http://"))  == 0;
    bool is_https = os_strncmp(url, "https://", strlen("https://")) == 0;

    if(is_http){ url += strlen("http://"); }
    else if(is_https) {
        port = 443;
        secure = true;
        url += strlen("https://");
    }
    else{ os_printf("URL is not HTTP or HTTPS %s\n", url); return; }

    char * path = os_strchr(url, '/');
    if(path == NULL){ path = os_strchr(url, '\0'); }

    char * colon = os_strchr(url, ':');
    if(colon > path){ colon = NULL; }

    if (colon == NULL) {
        os_memcpy(hostname, url, path - url);
        hostname[path - url] = '\0';
    }
    else{
        port = atoi(colon + 1);
        if (port == 0) { os_printf("Port error %s\n", url); return; }

        os_memcpy(hostname, url, colon - url);
        hostname[colon - url] = '\0';
    }

    if(path[0] == '\0'){ path = "/"; }

    tcp_client_raw(hostname, port, secure, path, post_data, headers);
}

void ICACHE_FLASH_ATTR tcp_client_get(const char * url, const char * headers){
    tcp_client_post(url, NULL, headers);
}

/** @} */
