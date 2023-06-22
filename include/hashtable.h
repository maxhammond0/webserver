#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

struct hashtable {
    int size; // Read-only
    int num_entries; // Read-only
    float load; // Read-only
    struct llist **bucket;
    int (*hashf)(void *data, int data_size, int bucket_count);
};

/**
 * Create a new hashtable
 */
extern struct hashtable *hashtable_create(int size, int (*hashf)(void *, int, int));

/**
 * Destroy a hashtable
 *
 * NOTE: does *not* free the data pointer
 */
extern void hashtable_destroy(struct hashtable *ht);

/**
 * Put to hash table with a string key
 */
extern void *hashtable_put(struct hashtable *ht, char *key, void *data);

/**
 * Put to hash table with a binary key
 */
extern void *hashtable_put_bin(struct hashtable *ht, void *key, int key_size, void *data);

/**
 * Get from the hash table with a string key
 */
extern void *hashtable_get(struct hashtable *ht, char *key);

/**
 * Get from the hash table with a binary data key
 */
extern void *hashtable_get_bin(struct hashtable *ht, void *key, int key_size);

/**
 * Delete from the hashtable by string key
 */
extern void *hashtable_delete(struct hashtable *ht, char *key);

/**
 * Delete from the hashtable by binary key
 *
 * NOTE: does *not* free the data--just free's the hash table entry
 */
extern void *hashtable_delete_bin(struct hashtable *ht, void *key, int key_size);

/**
 * For-each element in the hashtable
 *
 * Note: elements are returned in effectively random order.
 */
extern void hashtable_foreach(struct hashtable *ht, void (*f)(void *, void *), void *arg);

#endif
