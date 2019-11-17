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

/**
 * @brief TCP client GET request
 * @param URL request
 * @param Data headers
 */
void tcp_client_get(const char * url, const char * headers);

#endif // CLIENT_H
/** @} */
