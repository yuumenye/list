#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/* For simplicity's sake 0th element count as a real one
 * so list capacity is requested capacity + 1 */
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

void list_traverse(struct list *list)
{
    int i = list->head;
    for (int i = list->head; i != 0; i = list->data[i].next)
        printf("%d ", list->data[i].key);
    putchar('\n');
}

/* return key index in an array */
int list_search(struct list *list, const int x)
{
    int i = list->head;
    while (i != 0 && list->data[i].key != x)
        i = list->data[i].next;
    return i;
}

/* insert before head */
void list_prepend(struct list *list, const int x)
{
    int i = list->free;
    list->data[list->free++].key = x;
    list->data[i].next = list->head;
    list->data[i].prev = 0;
    if (list->head != 0)
        list->data[list->head].prev = i;
    list->head = i;
}

/* insert x after y */
void list_insert(struct list *list, const int x, const int y)
{
    int j = list_search(list, y);
    int i = list->free;
    list->data[list->free++].key = x;

    list->data[i].next = list->data[j].next;
    list->data[i].prev = j;
    if (list->data[j].next != 0)
        list->data[list->data[j].next].prev = i;
    list->data[j].next = i;
}

void list_delete(struct list *list, const int x)
{
    int i = list_search(list, x);
    if (list->data[i].prev != 0)
        list->data[list->data[i].prev].next = list->data[i].next;
    else 
        list->head = list->data[i].next;
    if (list->data[i].next != 0)
        list->data[list->data[i].next].prev = list->data[i].prev;
}
