#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#include "cache.h"

struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    struct cache_entry *entry = malloc(sizeof(struct cache_entry));

    entry->path = path;
    entry->content_type = content_type;
    entry->content = content;
    entry->content_length = content_length;

    return entry;
}

void free_entry(struct cache_entry *entry)
{
    free(entry);
}

/**
 * Insert a cache entry at the head of the linked list
 */
void dllist_insert_head(struct cache *cache, struct cache_entry *ce)
{
    // Insert at the head of the list
    if (cache->head == NULL)
    {
        cache->head = cache->tail = ce;
        ce->prev = ce->next = NULL;
    }
    else
    {
        cache->head->prev = ce;
        ce->next = cache->head;
        ce->prev = NULL;
        cache->head = ce;
    }
}

/**
 * Move a cache entry to the head of the list
 */
void dllist_move_to_head(struct cache *cache, struct cache_entry *ce)
{
    if (ce != cache->head) {
        if (ce == cache->tail) {
            // We're the tail
            cache->tail = ce->prev;
            cache->tail->next = NULL;

        } else {
            // We're neither the head nor the tail
            ce->prev->next = ce->next;
            ce->next->prev = ce->prev;
        }

        ce->next = cache->head;
        cache->head->prev = ce;
        ce->prev = NULL;
        cache->head = ce;
    }
}


/**
 * Removes the tail from the list and returns it
 *
 * NOTE: does not deallocate the tail
 */
struct cache_entry *dllist_remove_tail(struct cache *cache)
{
    struct cache_entry *oldtail = cache->tail;

    cache->tail = oldtail->prev;
    cache->tail->next = NULL;

    cache->cur_size--;

    return oldtail;
}

struct cache *cache_create(int max_size, int hashsize)
{
    struct cache *cache = malloc(sizeof(struct cache));
    struct hashtable *table = hashtable_create(hashsize, NULL);

    cache->head = NULL;
    cache->tail = NULL;
    cache->index = table;
    cache->max_size = max_size;
    cache->cur_size = hashsize;

    return cache;
}

void cache_free(struct cache *cache)
{
    struct cache_entry *cur_entry = cache->head;

    hashtable_destroy(cache->index);

    while (cur_entry != NULL) {
        struct cache_entry *next_entry = cur_entry->next;

        free_entry(cur_entry);

        cur_entry = next_entry;
    }

    free(cache);
}

void cache_put(struct cache *cache, char *path, char *content_type,
        void *content, int content_length)
{
    struct cache_entry *entry = alloc_entry(path, content_type, content,
            content_length);

    // Insert entry into hashtable and head of dll
    hashtable_put(cache->index, path, entry);
    dllist_insert_head(cache, entry);
    cache->cur_size++;

    if (cache->cur_size > cache->max_size)
    {
        struct cache_entry *tail = dllist_remove_tail(cache);
        hashtable_delete(cache->index, tail->path);
        free(tail);
        cache->cur_size--;
    }
}

struct cache_entry *cache_get(struct cache *cache, char *path)
{
    struct cache_entry *entry = hashtable_get(cache->index, path);
    if (entry == NULL) {
        return NULL;
    }

    cache_put(cache, entry->path, entry->content_type, entry->content,
            entry->content_length);

    return entry;
}
