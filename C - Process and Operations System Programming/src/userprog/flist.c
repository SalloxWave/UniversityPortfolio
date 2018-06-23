#include <stddef.h>

#include "flist.h"
  
void map_init(struct map* m)
{
    list_init(&m->content);
    m->next_key = 2;
}

key_t map_insert(struct map* m, value_t v)
{
    struct association* my_association_elem = 
        (struct association*)malloc(sizeof(struct association));

    my_association_elem->key = m->next_key;
    my_association_elem->value = v;

    list_push_front(&m->content, &my_association_elem->elem);
    return m->next_key++;
}

value_t map_find(struct map* m, key_t k)
{
    for (struct list_elem* e = list_begin(&m->content); e != list_end(&m->content); e = list_next(e))
    {
        struct association *a;
        a = list_entry(e, struct association, elem);
        if (a->key == k) return a->value;
    }
    return NULL;
}

value_t map_remove(struct map* m, key_t k)
{
    for (struct list_elem* e = list_begin(&m->content); e != list_end(&m->content); e = list_next(e))
    {
        struct association *a;
        a = list_entry(e, struct association, elem);
        if (a->key == k) 
        {
            value_t v = a->value;
            e = list_remove(e);
            free(a);
            return v;
        }        
    }
    return NULL;    
}

void map_free(struct map* m)
{
    for (struct list_elem* e = list_begin(&m->content); e != list_end(&m->content);)
    {        
        struct association *a = list_entry(e, struct association, elem);
        e = list_remove(e);
        free(a);
    }    
}

void map_for_each(struct map* m, void (*exec) (key_t k, value_t v, int aux), int aux)
{
    for (struct list_elem* e = list_begin(&m->content); e != list_end(&m->content); e = list_next(e))
    {
        struct association *a;
        a = list_entry(e, struct association, elem);
        exec(a->key, a->value, aux);
    }
}

void map_remove_if(struct map* m, bool (*cond) (key_t k, value_t v, int aux), int aux)
{
    for (struct list_elem* e = list_begin(&m->content); e != list_end(&m->content);)
    {
        struct association *a;
        a = list_entry(e, struct association, elem);
        if (cond(a->key, a->value, aux)) {            
            e = list_remove(e);
            free(a);            
        }
        else {
            e = list_next(e);
        }
    }
}