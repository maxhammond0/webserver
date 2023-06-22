#include "llist.h"

struct node
{
    void *data;
    struct node *next;
};

struct llist *llist_create(void)
{
    struct llist *llist = malloc(sizeof(struct llist));
    if (llist == NULL)
    {
        return NULL;
    }

    llist->head = NULL;

    return llist;
}

void llist_destroy(struct llist *llist)
{
    struct node *head = llist->head;
    struct node *next = NULL;

    while (head != NULL)
    {
        next = head->next;
        free(head);
        head = next;
    }

    free(llist);
}

void *llist_insert(struct llist *llist, void *data)
{
    struct node *head = malloc(sizeof(struct node));
    if (head == NULL)
    {
        return NULL;
    }

    head->data = data;
    head->next = llist->head;

    llist->head = head;
    llist->count++;

    return data;
}

void *llist_append(struct llist *llist, void *data)
{
    struct node *node = malloc(sizeof(struct node));
    if (node == NULL)
    {
        return NULL;
    }

    node->data = data;
    node->next = NULL;

    struct node *head = llist->head;

    while (head->next != NULL)
    {
        head = head->next;
    }

    head->next = node;

    return data;
}

void *llist_head(struct llist *llist)
{
    return llist->head == NULL ? NULL : llist->head->data;
}

void *llist_tail(struct llist *llist)
{
    if (llist->head == NULL)
    {
        return NULL;
    }

    struct node *cursor = llist->head;

    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    return cursor->data;
}

void *llist_find(struct llist *llist, void *data,
        int (*cmpfn) (void *, void *))
{

    struct node *cursor = llist->head;

    while (cursor->next != NULL)
    {
        if (cmpfn(cursor->data, data) == 0)
        {
            return data;
        }

        cursor = cursor->next;
    }

    return NULL;
}

void *llist_delete(struct llist *llist, void *data,
        int (*cmpfn) (void *, void *))
{

    struct node *cursor = llist->head;
    struct node *prev = NULL;

    while (cursor != NULL)
    {
        if (cmpfn(cursor->data, data) == 0)
        {
            void *data = data;

            if (prev == NULL)
            {
                llist->head = cursor->next;
                free(cursor);
            }
            else
            {
                prev->next = cursor->next;
                free(cursor);
            }

            llist->count--;
            return data;
        }

        prev = cursor;
        cursor = cursor->next;
    }

    return NULL;
}

int llist_count(struct llist *llist)
{
    return llist->count;
}

void llist_foreach(struct llist *llist, void (*fn) (void *, void *),
        void *arg)
{

    struct node *cursor = llist->head;

    while (cursor != NULL)
    {
        fn(cursor->data, arg);
        cursor = cursor->next;
    }
}

void **llist_array_get(struct llist *llist)
{
    if (llist->head == NULL)
    {
        return NULL;
    }

    void **array = malloc(sizeof *array * llist->count+1);

    struct node *cursor = llist->head;

    int i = 0;
    while (cursor != NULL)
    {
        array[i] = cursor->data;
        cursor = cursor->next;
        i++;
    }

    array[llist->count] = NULL;

    return array;
}

void llist_array_free(void **array)
{
    free(array);
}
