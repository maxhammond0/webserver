#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "cache.h"

/*
 * Handles the HTTP request and parses it to then let the request.h file handle
 * the request
 */

void handle_request(int fd, struct cache *cache);

#endif
