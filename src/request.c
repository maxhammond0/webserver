#include "request.h"

void parse_request(char *response) {
    char *eol = strchr(response, '\n');

    if (eol != NULL) {
        *eol = '\0';
    }

    printf("%s\n", response);
}
