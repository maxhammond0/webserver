#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "response.h"

#define BUFLEN (8192)

char default_root[] = "public_html";

int main(int argc, char **argv) {
    // if (argc != 4) {
    //     printf("Incorrect usage\n");
    //     printf("Should be ./wserver <ipv4> <port> <root dir>\n");
    //     return 1;
    // }
    int c;
    char *root_dir = default_root;
    int port = 10000;
    int threads = 1;
    int buffers = 1;

    char *schedalg = "FIFO";

    while ((c = getopt(argc, argv, "d:p:t:b:s:")) != -1) {
        switch (c) {
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
                if (!strcmp(optarg, "FIFO") && !strcmp(optarg, "SFF")) {
                    fprintf(stderr, "Unrecognized scheduling algorithm");
                    exit(1);
                }
                if (strcmp(optarg, "SFF") == 0) {
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

    if (chdir(root_dir) == -1) {
        fprintf(stderr, "Couldn't find root directory");
        exit(1);
    }

    // struct sockaddr_in sa;
    struct sockaddr_storage their_addr;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char port_str[10];
    sprintf(port_str, "%d", port);
    if (getaddrinfo(NULL, port_str, &hints, &res) != 0) {
        fprintf(stderr, "Error getting address info\n");
        exit(1);
    }

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Error creating socket\n");
        exit(1);
    }

    int yes=1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "Error binding to socket\n");
        exit(1);
    }

    if (listen(sockfd, buffers) == -1) { 
        fprintf(stderr, "Error listen to socket\n");
        exit(1);
    }

    char buf[BUFLEN];


    while (1) {

        socklen_t addr_size = sizeof(their_addr);
        int readfd;
        if ((readfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size)) == -1) {
            fprintf(stderr, "Error accepting socket connection\n");
            exit(1);
        }

        int read = recv(readfd, buf, BUFLEN, 0);
        if (read == -1) {
            fprintf(stderr, "Error receiving data\n");
            exit(1);
        }

        printf("%s", buf);

        char *response = generate_response(buf);
        if (response == NULL) {
            fprintf(stderr, "Invalid response");
        }
        
        int bytes_sent = send(readfd, response, strlen(response), 0);
        if (bytes_sent < 0) {
            fprintf(stderr, "Error sending data\n");
        }

        free(response);

        close(readfd);
    }

    // Unreachable code
    close(sockfd);

    return 0;
}
