#ifndef JSON_H
#define JSON_H

void json_open(char* strJSON);
void json_close(char* strJSON);
void json_string(char* strJSON, char* field, char* value);

void json_boolean(char* strJSON, char* field, uint8 status);

#endif // JSON_H
