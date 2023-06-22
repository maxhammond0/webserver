#ifndef _RESPONSE_H_
#define _RESPONSE_H_

/*
 * A collection of functions for building up a response to a html request
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define BUFLEN (8192)

/*
 * Takes in a HTTP request and fills in buf with the desired response
 * ------------------------------------------------------------------
 *
 * request: pointer to a string of the HTTP request
 *
 * returns: A pointer to the string of the desired response
 */
char *generate_response(char *request);

/*
 * Parses the http request
 */
char **parse_request(char *request);

#endif
