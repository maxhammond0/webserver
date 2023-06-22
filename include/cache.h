#ifndef _WEBCACHE_H_
#define _WEBCACHE_H_

// Individual hash table entry
struct cache_entry
{
    char *path;   // Endpoint path--key to the cache
    char *content_type;
    int content_length;
    void *content;

    struct cache_entry *prev, *next; // Doubly-linked list
};

// A cache
struct cache
{
    struct hashtable *index;
    struct cache_entry *head, *tail; // Doubly-linked list
    int max_size; // Maxiumum number of entries
    int cur_size; // Current number of entries
};

/**
 * Allocate a cache entry
 */
extern struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length);

/**
 * Deallocate a cache entry
 */
extern void free_entry(struct cache_entry *entry);

/**
 * Create a new cache
 *
 * max_size: maximum number of entries in the cache
 * hashsize: hashtable size (0 for default)
 */
extern struct cache *cache_create(int max_size, int hashsize);

/*
 * Frees all memory of a cache
 */
extern void cache_free(struct cache *cache);

/**
 * Store an entry in the cache
 *
 * This will also remove the least-recently-used items as necessary.
 *
 * NOTE: doesn't check for duplicate cache entries
 */
extern void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length);

/**
 * Retrieve an entry from the cache
 */
extern struct cache_entry *cache_get(struct cache *cache, char *path);

#endif
