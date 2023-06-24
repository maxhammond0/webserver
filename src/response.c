#include "cache.h"
#include <string.h>
#include <ctype.h>

#include "response.h"

#define DEFAULT_MIME_TYPE "text/html"

char **parse_request(char *request)
{
    char **result = malloc(sizeof(char *) * 2);
    if (result == NULL)
    {
        return NULL; // Not enough memory
    }

    char *method = strtok(request, " ");
    char *filepath = strtok(NULL, " ");

    if (method == NULL || filepath == NULL)
    {
        free(result);
        return NULL; // Invalid request
    }

    // If the requested filepath is "/", change it to "/index.html"
    if (strcmp(filepath, "/") == 0)
    {
        filepath = "/index.html";
    }

    // Allocate memory and copy the method and filepath into the result array
    result[0] = strdup(method);

    if (result[0] == NULL)
    {
        free(result);
        return NULL; // Not enough memory
    }

    // Ignore the leading slash in the filepath before duplicating
    result[1] = strdup(filepath + 1);
    if (result[1] == NULL)
    {
        free(result[0]);
        free(result);
        return NULL; // Not enough memory
    }

    return result;
}

char *get_html_content(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        file = fopen("../404.html", "r");
        if (!file)
        {
            fprintf(stderr, "File not found");
            return NULL;
        }
    }

    // Seek to the end of the file to determine its size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);  // Return to the start of the file

    char *content = malloc(length + 1);
    if (content == NULL)
    {
        fprintf(stderr, "Could not allocate %ld bytes for file %s\n", length, filename);
        fclose(file);
        return NULL;
    }

    fread(content, 1, length, file);
    fclose(file);

    content[length] = '\0';

    return content;
}

char *strlower(char *s)
{
    for (char *p = s; *p != '\0'; p++) {
        *p = tolower(*p);
    }

    return s;
}

char *mime_type_get(char *filename)
{
    char *ext = strrchr(filename, '.');

    if (ext == NULL) {
        return DEFAULT_MIME_TYPE;
    }

    ext++;

    strlower(ext);

    if (strcmp(ext, "html") == 0 || strcmp(ext, "htm") == 0) { return "text/html"; }
    if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0) { return "image/jpg"; }
    if (strcmp(ext, "css") == 0) { return "text/css"; }
    if (strcmp(ext, "js") == 0) { return "application/javascript"; }
    if (strcmp(ext, "json") == 0) { return "application/json"; }
    if (strcmp(ext, "txt") == 0) { return "text/plain"; }
    if (strcmp(ext, "gif") == 0) { return "image/gif"; }
    if (strcmp(ext, "png") == 0) { return "image/png"; }

    return DEFAULT_MIME_TYPE;
}

char *handle_get_request(char *path, struct cache *cache)
{

    // struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
    // path, content_type, content, content_length
    struct cache_entry *entry;
    char *content;
    char *content_type;
    long content_length;
    if ((entry = cache_get(cache, path)) == NULL) {
        content = get_html_content(path);
        content_type = mime_type_get(path);
        content_length = strlen(content);

        cache_put(cache, path, content, content_type, content_length);
    } else {
        content = entry->content;
        content_type = entry->content_type;
        content_length = entry->content_length;
    }

    char *response = malloc(BUFLEN*sizeof(char));

    char date[30];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(date, sizeof(date), "%a %b %d %T %Z %Y", timeinfo);

    snprintf(response, BUFLEN,
            "HTTP/1.1 200 OK\r\n"
            "Date: %s\r\n"
            "Connection: close\r\n"
            "Content-Length: %ld\r\n"
            "Content-Type: %s/html\r\n"
            "\r\n"
            "%s\r\n",
            date, content_length, content_type, content);

    free(content);
    free(content_type);
    return response;
}
