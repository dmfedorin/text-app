#ifndef LOGIN_H__
#define LOGIN_H__

#include <stdbool.h>
#include <stdint.h>

#define LOGIN_USERNAME_LENGTH 32

void login_init(const char *login_file_name);
void login_stop(void);

bool login_is_valid(const char name[LOGIN_USERNAME_LENGTH], uint64_t passw_hash);
void login_add(const char name[LOGIN_USERNAME_LENGTH], uint64_t passw_hash);

#endif
