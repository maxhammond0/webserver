#ifndef _RESPONSE_H_
#define _RESPONSE_H_

/*
 * A collection of functions for building up a response to a html request
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "cache.h"

#define BUFLEN (8192)

/*
 * Parses the http request
 */
char **parse_request(char *request);

/*
 * Handles a get request and returns the correct HTML response as a char *
 */
char *handle_get_request(char *filename, struct cache *cache);

#endif
