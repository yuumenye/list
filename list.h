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
};

int list_ctor(struct list *list, int capacity);
void list_dtor(struct list *list);
int list_search(struct list *list, const int key);
void list_traverse(const struct list * const list);
int list_insert(struct list *list, const int x, const int i);
void list_delete(struct list *list, const int x);
void list_delete_all(struct list *list);
int list_index(struct list *list, const int i);

#endif
