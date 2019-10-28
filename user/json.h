/*
              UNKNOWN PUBLIC LICENSE

 Copyright (C) 2019 Wira S.T. M.T.

 Currently no license applied because author liv in
 Indonesia, a country which doesn't really concern
 about digital content copyright.

 */

/**
 * @file    json.h
 * @brief   JSON String Builder header.
 *
 * @addtogroup Webserver
 * @{
 */

#ifndef JSON_H
#define JSON_H

/**
 * @brief JSON start string "{"
 * @param[in] String buffer
 */
void json_open(char* strJSON);

/**
 * @brief JSON close string "}"
 * @param[in] String buffer
 */
void json_close(char* strJSON);

/**
 * @brief JSON body string
 * @param[in] String buffer
 * @param[in] String field
 * @param[in] String value
 */
void json_string(char* strJSON, char* field, char* value);

/**
 * @brief JSON body string
 * @param[in] String buffer
 * @param[in] String field
 * @param[in] Boolean value
 */
void json_boolean(char* strJSON, char* field, uint8 value);

#endif // JSON_H
/** @} */
