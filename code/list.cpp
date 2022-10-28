#include "../include/list.h"

//-----------------------------------------------------------------------------

int list_ctor_ (List *lst,             int capacity_ctor, const char* lst_name,
                const char* file_name, int lst_line                            )
{
    lst->capacity = capacity_ctor + 1;
    lst->size = 1;

    lst->List_info.name = lst_name;
    lst->List_info.file = file_name;
    lst->List_info.dbg_file = fopen ("../dump/list_dump.txt", "w+");
    lst->List_info.line = lst_line;
    lst->List_info.error_codes = 0;
    lst->List_info.cur_status = "OK";

    lst->head = 0;
    lst->tail = 0;

    if (capacity_ctor >= 0)
    {
        lst->buffer = (double*) calloc (1, (capacity_ctor + 1) * sizeof (double));
        lst->next   = (int*)    calloc (1, (capacity_ctor + 1) * sizeof (int));
        lst->prev   = (int*)    calloc (1, (capacity_ctor + 1) * sizeof (int));
    }

    else
    {
        lst->List_info.error_codes |= ERR_CAP;

        return ERR_CAP;
    }


    for(int i = 1; i < lst->capacity; i++)
    {
        if(i != 1)
        {
            lst->prev[i - 1];
        }

        if(i != lst->capacity - 1)
        {
            lst->next[i + 1];
        }
    }

    lst->free = 1;

    return 0;
}

//-----------------------------------------------------------------------------

void list_dtor (List *lst)
{
    free (lst->buffer);
    free (lst->next);
    free (lst->prev);

    fclose (lst->List_info.dbg_file);
}

//-----------------------------------------------------------------------------

void list_resize (List *lst, int opt_resize)
{
    if(opt_resize == lst_increase)
    {
        if(lst->capacity == 0)
        {
            lst->capacity = 2;
        }

        lst->capacity *= 2;
    }

    else if(opt_resize == lst_decrease)
    {
        lst->capacity /= 2;
    }

    else
    {
        lst->List_info.error_codes |= ERR_RESIZE;
    }

    lst->buffer = (double*) recalloc (lst->buffer, lst->capacity, lst->size);
    lst->next   = (int*)    recalloc (lst->buffer, lst->capacity, lst->size);
    lst->prev   = (int*)    recalloc (lst->buffer, lst->capacity, lst->size);

    for(int i = lst->size; i < lst->size; i++)
    {
        if(i != 1)
        {
            lst->prev[i - 1];
        }

        if(i != lst->size - 1)
        {
            lst->next[i + 1];
        }
    }
}

//-----------------------------------------------------------------------------

void list_push_head (List *lst, double elem)
{
    if(next[free] == 0)
    {
        list_resize (lst, lst_increase);
    }

    lst->size++;

    if(lst->size > lst->capacity)
    {
        list_resize (lst, lst_increase);
    }

    lst->prev[lst->size - 1]   = 0;
    lst->next[lst->size - 1]   = 0;
    lst->buffer[lst->size - 1] = elem;

    if(lst->tail == 0)
    {
        lst->tail = lst->size - 1;
    }

    else
    {
        int secnd_hd = lst->head;
        lst->prev[secnd_hd] = lst->size - 1;
        lst->next[lst->size - 1] = secnd_hd;
    }

    lst->head = lst->size - 1;

    debug_list (lst);
}

//-----------------------------------------------------------------------------

void list_push_in (List *lst, double elem, int pos)
{
    lst->size++;

    if(lst->size + 1 > lst->capacity)
    {
        list_resize (lst, lst_increase);
    }

    lst->buffer[lst->size - 1] = elem;

    lst->prev[lst->size - 1] = pos;
    int next_elem = lst->next[pos];
    lst->next[pos] = lst->size - 1;
    lst->next[lst->size - 1] = next_elem;

    if(pos == lst->tail)
    {
        lst->tail = lst->size - 1;
    }

    else
    {
        lst->prev[next_elem] = lst->size - 1;
    }

    debug_list (lst);
}

//-----------------------------------------------------------------------------

void list_pop (List *lst, int pos)
{
    if(pos == lst->head)
    {
        lst->prev[lst->next[lst->head]] = 0;
        int new_head = lst->next[lst->head];
        lst->next[lst->head] = 0;
        lst->head = new_head;
    }

    else if(pos == lst->tail)
    {
        lst->next[lst->prev[lst->tail]] = 0;
        int new_tail = lst->prev[lst->tail];
        lst->prev[lst->tail] = 0;
        lst-> tail = new_tail;
    }

    else
    {
        lst->next[lst->prev[pos]] = lst->next[pos];
        lst->prev[lst->next[pos]] = lst->prev[pos];
    }

    lst->next[pos] = 0;
    lst->prev[pos] = 0;
    lst->buffer[pos] = -1;

    debug_list (lst);

    lst->size--;
}

//-----------------------------------------------------------------------------

void debug_list (List *lst)
{
    printf ("_________________________LIST__________________________________\n\n");

    printf ("%d - HEAD\n", lst->head);
    printf ("%d - TAIL\n", lst->tail);

    printf ("_______________________________________________________________\n\n");

    for (int i = 0; i < lst->size; i++)
    {
        printf ("%lg ", lst->buffer[i]);
    }

    printf ("\n");

    for (int i = 0; i < lst->size; i++)
    {
        printf ("%d ", lst->next[i]);
    }

    printf ("\n");

    for (int i = 0; i < lst->size; i++)
    {
        printf ("%d ", lst->prev[i]);
    }

    printf ("\n_______________________________________________________________\n\n");
}

//-----------------------------------------------------------------------------

void *recalloc (void *buffer, int capacity, int size)
{
    char *pointer = (char*) realloc ((char*) buffer, capacity * sizeof (double));

    if(capacity > size)
    {
        memset (pointer + size * sizeof (double), '\0', (capacity - size) * sizeof (double));
    }

    return (void*) pointer;
}

//-----------------------------------------------------------------------------


