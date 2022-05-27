#include "server.h"

#include <stdbool.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static int sd;
static bool running = false;
static size_t max_msg_size;

static int max_client_sd = 0;
static fd_set client_sds;

static void send_all(const char *msg)
{
        for (int i = 0; i <= max_client_sd; ++i) {
                if (!FD_ISSET(i, &client_sds))
                        continue;

                size_t msg_len = strlen(msg) < max_msg_size ? strlen(msg) : max_msg_size;
                if (send(i, msg, msg_len, 0) == -1) {
                        printf("server failed to send message to client\n");
                        exit(-1);
                }
        }

        printf("server sent '%s' to all clients\n", msg);
}

static int accepted_cons;

/* new thread created for every client */
static void *serve_client(void *_client_sd)
{
        int client_sd = *(const int *)_client_sd;

        char recv_buf[max_msg_size];

        while (running) {
                int rc = recv(client_sd, recv_buf, sizeof(recv_buf), 0);
                if (rc <= 0) {
                        printf("server failed to recieve client message, disconnecting\n");
                        
                        --accepted_cons;
                        FD_CLR(client_sd, &client_sds);

                        break;
                }
                else
                        recv_buf[rc] = '\0';

                printf("server recieved '%s' from %d\n", recv_buf, client_sd);
                send_all(recv_buf);
        }

        return NULL;
}

static void accept_clients(int max_cons, pthread_t client_threads[])
{
        if (listen(sd, max_cons) == -1) {
                printf("failed to listen on server socket\n");
                exit(-1);
        }

        accepted_cons = 0;

        while (running && accepted_cons <= max_cons) {
                struct sockaddr_in inc_addr = { 0 };
                socklen_t addr_size = sizeof(inc_addr);

                int inc_sd = accept(sd, (struct sockaddr *)&inc_addr, &addr_size);
                if (inc_sd == -1) {
                        printf("failed to accept server connection\n");
                        exit(-1);
                }

                printf("new connection on %d from %s\n", inc_sd, inet_ntoa(inc_addr.sin_addr));

                FD_SET(inc_sd, &client_sds);
                if (inc_sd > max_client_sd)
                        max_client_sd = inc_sd;

                pthread_create(&client_threads[accepted_cons], NULL, serve_client, &inc_sd);

                ++accepted_cons;
        }
}

void server_run(int max_cons, uint16_t port, size_t _max_msg_size)
{
        max_msg_size = _max_msg_size;

        sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sd == -1) {
                printf("failed to create server socket\n");
                exit(-1);
        }

        struct sockaddr_in addr = {
                .sin_addr = INADDR_ANY,
                .sin_family = AF_INET,
                .sin_port = htons(port),
        };
        if (bind(sd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
                printf("failed to bind server socket\n");
                exit(-1);
        }

        running = true;

        pthread_t client_threads[max_cons];
        accept_clients(max_cons, client_threads);
        
        while (running)
                continue;

        for (int i = 0; i < accepted_cons; ++i)
                pthread_join(client_threads[i], NULL);

        close(sd);
}
