#include <sys/socket.h>

#include "response.h"
#include "cache.h"

#define MAX_RESPONSE_SIZE (65536) // 65536K

void handle_request(int fd, struct cache *cache)
{
    char buf[MAX_RESPONSE_SIZE];

    int read = recv(fd, buf, MAX_RESPONSE_SIZE, 0);
    if (read == -1)
    {
        fprintf(stderr, "Error receiving data\n");
        return;
    }

    printf("%s", buf);

    char **request = parse_request(buf);
    if (request == NULL)
    {
        return;
    }

    if (strcmp(request[0], "GET") == 0) // parse a get response
    {
        char *response = handle_get_request(request[1], cache);
        if (send(fd, response, strlen(response), 0) == -1)
        {
            fprintf(stderr, "Error sending message\n");
        }
        free(response);
    }
    else if (strcmp(request[0], "POST") == 0) // parse a post response
    {
        // TODO
    }

    free(request[0]);
    free(request[1]);
    free(request);
}
