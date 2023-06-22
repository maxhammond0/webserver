#include "response.h"

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

char *get(char *filename)
{

    char *html_string = get_html_content(filename);
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
            "Content-Type: text/html\r\n"
            "\r\n"
            "%s\r\n",
            date, strlen(html_string), html_string);

    return response;
}

char *generate_response(char *request)
{

    char **args = parse_request(request);
    char *response;

    if (!strcmp(args[0], "GET"))
    {
        response = get(args[1]);
    }
    else
    {
        response = NULL;
    }

    free(args[0]);
    free(args[1]);
    free(args);

    return response;
}
