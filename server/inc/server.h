#ifndef SERVER_H__
#define SERVER_H__

#include <stdint.h>
#include <stddef.h>

void server_run(int max_cons, uint16_t port, size_t _max_msg_size);

#endif
