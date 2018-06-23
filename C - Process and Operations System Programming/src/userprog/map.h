#ifndef _INCL_GUARD
#define _INCL_GUARD

#include <stdbool.h>
#include <stdlib.h>
#include "list.h"

##define PANIC() exit(1)

typedef char* value_t;
typedef int key_t;

//typedef enum {false, true} bool;

struct association
{
    key_t key;
    value_t value;
    struct list_elem elem;
};

struct map
{
    struct list content;
    int next_key;
};

void smth();

void map_init(struct map* m);

key_t map_insert(struct map* m, value_t v);

value_t map_find(struct map* m, key_t k);

value_t map_remove(struct map* m, key_t k);

void map_for_each(struct map* m, void (*exec) (key_t k, value_t v, int aux), int aux);

void map_remove_if(struct map* m, bool (*cond) (key_t k, value_t v, int aux), int aux);

#endif