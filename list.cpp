#include <stdlib.h>
#include <stdio.h>
#include "list.h"

static int list_head(const struct list * const list);
static int list_tail(const struct list * const list);

static void generate_filename(char *filename, int namelen);
static void compile_dot(char *filename);
static void write_all(FILE *file, struct list *list);
static void write_params(FILE *file);
static void write_data(FILE *file, struct list *list);
static void write_data_nodes(FILE *file, struct list *list);
static void write_data_edges(FILE *file, struct list *list);
static void write_free(FILE *file, struct list *list);
static void write_free_nodes(FILE *file, struct list *list);
static void write_free_edges(FILE *file, struct list *list);

/* circular, doubly linked list with a sentinel */
/* free elements form a singly linked list */
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

        for (int i = 1; i < capacity; ++i)
                list->data[i].next = i+1;
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
        list->free = list->data[list->free].next;
        list->data[j].key = x;
        list->data[j].next = list->data[i].next;
        list->data[j].prev = i;
        list->data[list->data[i].next].prev = j;
        list->data[i].next = j;
        return j;
}

/* delete i-th element in an array */
void list_delete(struct list *list, const int i)
{
        list->data[list->data[i].prev].next = list->data[i].next;
        list->data[list->data[i].next].prev = list->data[i].prev;

        list->data[i].next = list->data[list->free].next;
        list->data[list->free].next = i;
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

/* remove static */
static int list_tail(const struct list * const list)
{
        return list->data[0].prev;
}

/* dump list using GraphViz */
void list_dump(struct list *list)
{
        const int namelen = 20;
        char filename[namelen] = "";
        generate_filename(filename, namelen);

        FILE *file = fopen(filename, "w");
        write_all(file, list);
        fclose(file);

        compile_dot(filename);
}

static void generate_filename(char *filename, int namelen)
{
        static int filenum = 0;
        snprintf(filename, namelen, "list%03d.dot", filenum++);
}

static void compile_dot(char *filename)
{
        const int cmdlen = 100;
        char cmd[cmdlen] = "";
        snprintf(cmd, cmdlen, "dot -T png %s -o %.7s.png", filename, filename);
        system(cmd);
}

static void write_all(FILE *file, struct list *list)
{
        fprintf(file, "digraph dump {\n");
        write_params(file);
        write_data(file, list);
        write_free(file, list);
        fprintf(file, "}\n");
}

static void write_params(FILE *file)
{
        fprintf(file, "  rankdir = LR\n");
        fprintf(file, "  node [\n");
        fprintf(file, "  shape = record\n");
        fprintf(file, "  style = filled\n");
        fprintf(file, "  fillcolor = \"#FFB26F\"\n");
        fprintf(file, "  ]\n");
}

static void write_data(FILE *file, struct list *list)
{
        fprintf(file, "  subgraph cluster_data {\n");
        write_data_nodes(file, list);
        write_data_edges(file, list);
        fprintf(file, "  }\n");
}

static void write_data_nodes(FILE *file, struct list *list)
{
        int i = 0;
        do {
                fprintf(file, "    n%p [ label = \"i: %d|p: %d|k: %d|n: %d\" ]\n",
                        &list->data[i], i, list->data[i].prev,
                        list->data[i].key, list->data[i].next);
                i = list->data[i].next;
        } while (i != 0);
}
static void write_data_edges(FILE *file, struct list *list)
{
        int i = 0;
        do {
                fprintf(file, "  n%p -> n%p\n",
                        &list->data[i], &list->data[list->data[i].next]);
                i = list->data[i].next;
        } while (i != 0);

        i = 0;
        do {
                fprintf(file, "  n%p -> n%p\n",
                        &list->data[i], &list->data[list->data[i].prev]);
                i = list->data[i].prev;
        } while (i != 0);
}

static void write_free_nodes(FILE *file, struct list *list)
{
        for (int i = list->free; i != list->capacity; i = list->data[i].next) {
                fprintf(file, "    n%p [ label = \"i: %d|p: %d|k: %d|n: %d\" ]\n",
                        &list->data[i], i, list->data[i].prev,
                        list->data[i].key, list->data[i].next);
        }
}
static void write_free_edges(FILE *file, struct list *list)
{
        for (int i = list->free; i != list->capacity-1; i = list->data[i].next) {
                fprintf(file, "    n%p -> n%p\n",
                        &list->data[i], &list->data[list->data[i].next]);
        }
}

static void write_free(FILE *file, struct list *list)
{
        fprintf(file, "  subgraph cluster_free {\n");
        fprintf(file, "  label = \"free\";\n");
        write_free_nodes(file, list);
        write_free_edges(file, list);
        fprintf(file, "  }\n");
}
