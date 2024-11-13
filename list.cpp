#include <stdlib.h>
#include <stdio.h>
#include "list.h"

static int list_head(const struct list * const list);
static int list_tail(const struct list * const list);

/* circular, doubly linked list with a sentinel */
int list_ctor(struct list *list, int capacity)
{
        if (!list)
                return 1;
        capacity += 1;
        list->data = (struct node *) calloc(capacity, sizeof(struct node));
        if (!list->data)
                return 1;
        list->capacity = capacity;
        ++list->free;

        /*
        for (int i = list->free; i < list->capacity; ++i)
                list->data[i].key = -1;
        */
        return 0;
}

void list_dtor(struct list *list)
{
        free(list->data);
        list->data = NULL;
}

void list_traverse(struct list const * const list)
{
        for (int i = list_head(list); i != 0; i = list->data[i].next)
                printf("%d ", list->data[i].key);
        if (list_head(list) != 0)
                putchar('\n');
}

/* return x index in an array */
int list_search(struct list *list, const int x)
{
        int i = list_head(list);
        list->data[0].key = x;
        while (list->data[i].key != x)
                i = list->data[i].next;
        return i;
}

/* insert x after i-th element in an array */
int list_insert(struct list *list, const int x, const int i)
{
        int j = list->free;
        list->data[j].key = x;
        list->data[j].next = list->data[i].next;
        list->data[j].prev = i;
        list->data[list->data[i].next].prev = j;
        list->data[i].next = j;
        ++list->free;
        return j;
}

/* delete i-th element in an array */
void list_delete(struct list *list, const int i)
{
        list->data[list->data[i].prev].next = list->data[i].next;
        list->data[list->data[i].next].prev = list->data[i].prev;
}

void list_delete_all(struct list *list)
{
        while (list_head(list) != 0)
                list_delete(list, list->data[list_head(list)].key);
}

/* return key of i-th element in a list */
int list_index(struct list *list, const int i)
{
        int j = 0;
        do {
                j = list->data[j].next;
        } while (j != 0 && j != i);
        return list->data[j].key;
}

static int list_head(const struct list * const list)
{
        return list->data[0].next;
}

static int list_tail(const struct list * const list)
{
        return list->data[0].prev;
}
