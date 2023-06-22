#ifndef _LLIST_H_
#define _LLIST_H_

#include <stdlib.h>

struct llist {
    struct node *head;
    int count;
};

/*
 * Creates a linked list, returns a pointer to the llist struct
 */
struct llist *llist_create(void);

/*
 * Frees all memory related to llist, including all nodes
 */
void llist_destroy(struct llist *llist);

/*
 * Inserts a node with data being the parameter data at the head of the llist
 * Returns data
 */
void *llist_insert(struct llist *llist, void *data);

/*
 * Appends to the end of the llist a node with the data, data
 * Returns data
 */
void *llist_append(struct llist *list, void *data);

/*
 * Returns the data at the head of the llist
 */
void *llist_head(struct llist *llist);

/*
 * Returns the data at the tail of the llist
 */
void *llist_tail(struct llist *llist);

/*
 * Finds an element in the list, and returns data
 *
 * cmpfn returns 0 if both values are equal
 */
void *llist_find(struct llist *llist, void *data,
        int (*cmpfn) (void *, void *));

/*
 * Removes the node with data, *data from the list and returns data
 */
void *llist_delete(struct llist *llist, void *data,
        int (*cmpfn) (void *, void *));

/*
 * Returns the count of nodes in the linked list
 */
int llist_count(struct llist *llist);

/*
 * Iterates over each element in the linked list and applies fn() to the
 * data
 */
void llist_foreach(struct llist *llist, void (*fn) (void *, void *),
        void *arg);

/*
 * Returns the linked list as an array
 */
void **llist_array_get(struct llist *llist);

/*
 * Frees array created previously
 */
void llist_array_free(void **array);

#endif
