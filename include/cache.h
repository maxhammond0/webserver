#ifndef _CACHE_H_
#define _CACHE_H_

/*
 * Implementation of a LRU Cache
 */


struct cache_entry {
    char *path;
    char *content_type;
    int content_length;
    void *content;
};

struct cache {
    struct hashtable *index;
    struct cache_entry *head, *tail;
    int max_size;
    int cur_size;
};

#endif
