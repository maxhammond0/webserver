#include <sys/socket.h>

#include "response.h"

#define BUFLEN (8192)

void handle_request(int fd) {
    char buf[BUFLEN];

    int read = recv(fd, buf, BUFLEN, 0);
    if (read == -1)
    {
        fprintf(stderr, "Error receiving data\n");
        exit(1);
    }

    printf("%s", buf);

    char *response = generate_response(buf);
    if (response == NULL)
    {
        fprintf(stderr, "Invalid response\n");
    }

    int bytes_sent = send(fd, response, strlen(response), 0);
    if (bytes_sent < 0)
    {
        fprintf(stderr, "Error sending data\n");
    }

    free(response);
}
