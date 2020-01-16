/*
              UNKNOWN PUBLIC LICENSE

 Copyright (C) 2019 Wira S.T. M.T.

 Currently no license applied because author liv in
 Indonesia, a country which doesn't really concern
 about digital content copyright.

 */

/**
 * @file    client.h
 * @brief   TCP Client header.
 *
 * @addtogroup Webclient
 * @{
 */

#ifndef CLIENT_H
#define CLIENT_H

#define BUFFER_SIZE_MAX 5000

/**
 * @brief TCP client GET request
 * @param[in] URL request
 */
void tcp_client_get(const char * url);

/**
 * @brief TCP client POST request
 * @param[in] URL request
 * @param[in] POST data to send
 */
void tcp_client_post(const char * url, const char *post_data);

#endif // CLIENT_H
/** @} */