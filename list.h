#ifndef LIST_H
#define LIST_H

struct node {
    int prev;
    int key;
    int next;
};

struct list {
    struct node *data;
    int capacity;
    int free;
    int head;
    int tail;
};

int list_ctor(struct list *list, int capacity);
void list_dtor(struct list *list);
int list_search(struct list *list, const int key);
void list_traverse(struct list *list);
void list_prepend(struct list *list, const int x);
void list_append(struct list *list, const int x);
void list_insert(struct list *list, const int x, const int y);
void list_delete(struct list *list, const int x);

#endif
