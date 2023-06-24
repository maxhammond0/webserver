#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "response.h"
#include "request.h"
#include "cache.h"

#define BUFLEN (8192)

char default_root[] = "public_html";

int open_listen_fd(int port, int buffers)
{
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char port_str[10];
    sprintf(port_str, "%d", port);
    if (getaddrinfo(NULL, port_str, &hints, &res) != 0)

    {
        fprintf(stderr, "Error getting address info\n");
        return -1;
    }

    int listenfd;
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Error creating socket\n");
        return -1;
    }

    int yes=1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("setsockopt");
        return -1;
    }

    if (bind(listenfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        fprintf(stderr, "Error binding to socket\n");
        return -1;
    }

    if (listen(listenfd, buffers) == -1)
    {
        fprintf(stderr, "Error listen to socket\n");
        return -1;
    }

    return listenfd;
}

int main(int argc, char **argv)
{
    int c;
    char *root_dir = default_root;
    int port = 3000;
    int threads = 1;
    int buffers = 1;
    char *schedalg = "FIFO";


    while ((c = getopt(argc, argv, "d:p:t:b:s:")) != -1)
    {
        switch (c)
        {
            case 'd':
                root_dir = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                threads = atoi(optarg);
                break;
            case 'b':
                buffers = atoi(optarg);
                break;
            case 's':
                if (!strcmp(optarg, "FIFO") && !strcmp(optarg, "SFF"))
                {
                    fprintf(stderr, "Unrecognized scheduling algorithm");
                    exit(1);
                }
                if (strcmp(optarg, "SFF") == 0)
                {
                    schedalg = "SFF";
                }
                break;
            default:
                fprintf(stderr, "./wserver [-d basedir] [-p port] [-t threads] [-b buffers] [-s schedalg]\n");
                exit(1);
        }
    }
    printf("root dir: %s\n", root_dir);
    printf("port: %d\n", port);
    printf("threads: %d\n", threads);
    printf("buffers: %d\n", buffers);
    printf("scheduling algorithm: %s\n", schedalg);

    if (chdir(root_dir) == -1)
    {
        fprintf(stderr, "Couldn't find root directory");
        exit(1);
    }

    struct cache *cache = cache_create(10, 0);

    int listenfd = open_listen_fd(port, buffers);
    if (listenfd == -1) {
        fprintf(stderr, "Couldn't open file descriptor");
        exit(1);
    }

    struct sockaddr_storage client_addr;
    socklen_t sin_size;

    while (1)
    {
        int readfd = accept(listenfd, (struct sockaddr*)&client_addr,
                &sin_size);

        handle_request(readfd, cache); // handle rquest and send response

        close(readfd);
    }

    printf("Shutting down server\n");

    close(listenfd);

    return 0;
}
