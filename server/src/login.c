#include "login.h"

#include <stdio.h>
#include <string.h>

static FILE *login_file;

void login_init(const char *login_file_name)
{
        login_file = fopen(login_file_name, "a+");
}

void login_stop(void)
{
        fclose(login_file);
}

struct login_info {
        char name[LOGIN_USERNAME_LENGTH];
        uint64_t passw_hash;
};

bool login_is_valid(const char name[LOGIN_USERNAME_LENGTH], uint64_t passw_hash)
{
        return false;
}

void login_add(const char name[LOGIN_USERNAME_LENGTH], uint64_t passw_hash)
{
        struct login_info new_login = {
                .passw_hash = passw_hash,
        };
        memcpy(new_login.name, name, LOGIN_USERNAME_LENGTH);
}
