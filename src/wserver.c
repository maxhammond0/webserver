#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

    // 0 = FIFO, 1 = SFF
    int schedalg = 0;

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
                    schedalg = 1;
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
    printf("scheduling algorithm: %d\n", schedalg);

    if (chdir(root_dir) == -1) {
        fprintf(stderr, "Couldn't find root directory");
        exit(1);
    }

    return 0;
}
