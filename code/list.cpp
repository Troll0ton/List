#include "../include/list.h"

//-----------------------------------------------------------------------------

int list_ctor_ (List       *Lst,       int capacity_ctor, const char* lst_name,
                const char* file_name, int lst_line                            )
{
    Lst->capacity = capacity_ctor + 1;
    Lst->size = 1;
    Lst->head = 0;
    Lst->tail = 0;

    if (capacity_ctor >= 0)
    {
        Lst->buffer = (double*) calloc (1, (capacity_ctor + 1) * sizeof (double));
        Lst->next   = (int*)    calloc (1, (capacity_ctor + 1) * sizeof (int));
        Lst->prev   = (int*)    calloc (1, (capacity_ctor + 1) * sizeof (int));

        if(Lst->buffer == NULL || Lst->next == NULL || Lst->prev == NULL)
        {
            Lst->Info.error_codes |= ERR_DATA;

            return ERR_DATA;
        }
    }

    else
    {
        Lst->Info.error_codes |= ERR_CAP;

        return ERR_CAP;
    }


    for(int i = 1; i < Lst->capacity; i++)
    {
        if(i != 1) Lst->prev[i - 1];

        if(i != Lst->capacity - 1) Lst->next[i + 1];
    }

    Lst->free = 1;

    return list_info_ctor (Lst, capacity_ctor, lst_name, file_name, lst_line);
}

//-----------------------------------------------------------------------------

int list_info_ctor (List       *Lst        int capacity_ctor, const char* lst_name,
                    const char* file_name, int lst_line                            )
{
    Lst->Info.name        = lst_name;
    Lst->Info.file        = file_name;
    Lst->Info.dbg_file    = fopen ("../dump/list_dump.txt", "w+");
    Lst->Info.line        = lst_line;
    Lst->Info.error_codes = 0;
    Lst->Info.cur_status  = "OK";

    if(Lst->Info.dbg_file == NULL)
    {
        return ERROR_LINFO_CTOR;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void list_dtor (List *Lst)
{
    free (Lst->buffer);
    free (Lst->next);
    free (Lst->prev);

    fclose (Lst->Info.dbg_file);
}

//-----------------------------------------------------------------------------

void list_resize (List *Lst, int opt_resize)
{
    if(opt_resize == LST_INCREASE)
    {
        if(Lst->capacity == 0)
        {
            Lst->capacity = 2;
        }

        Lst->capacity *= 2;
    }

    else if(opt_resize == LST_DECREASE)
    {
        Lst->capacity /= 2;
    }

    else
    {
        Lst->Info.error_codes |= ERR_RESIZE;
    }

    Lst->buffer = (double*) recalloc (Lst->buffer, Lst->capacity, Lst->size, sizeof (double));
    Lst->next   = (int*)    recalloc (Lst->next,   Lst->capacity, Lst->size, sizeof (int));
    Lst->prev   = (int*)    recalloc (Lst->prev,   Lst->capacity, Lst->size, sizeof (int));
}

//-----------------------------------------------------------------------------

void list_push_head (List *Lst, double elem)
{
    if(next[free] == 0)
    {
        list_resize (Lst, LST_INCREASE);
    }

    Lst->size++;

    if(Lst->size > Lst->capacity)
    {
        list_resize (Lst, LST_INCREASE);
    }

    Lst->prev[free]   = 0;
    Lst->next[free]   = 0;
    Lst->buffer[free] = elem;

    if(Lst->tail == 0)
    {
        Lst->tail = free;
    }

    else
    {
        int secnd_hd = Lst->head;
        Lst->prev[secnd_hd] = free;
        Lst->next[free] = secnd_hd;
    }

    Lst->head = free;

    debug_list (Lst);
}

//-----------------------------------------------------------------------------

void list_push_in (List *Lst, double elem, int pos)
{
    Lst->size++;

    if(Lst->size + 1 > Lst->capacity)
    {
        list_resize (Lst, LST_INCREASE);
    }

    Lst->buffer[Lst->size - 1] = elem;

    Lst->prev[Lst->size - 1] = pos;
    int next_elem = Lst->next[pos];
    Lst->next[pos] = Lst->size - 1;
    Lst->next[Lst->size - 1] = next_elem;

    if(pos == Lst->tail)
    {
        Lst->tail = Lst->size - 1;
    }

    else
    {
        Lst->prev[next_elem] = Lst->size - 1;
    }

    debug_list (Lst);
}

//-----------------------------------------------------------------------------

void list_pop (List *Lst, int pos)
{
    if(Lst->buffer[pos] == -1)
    {
        return ERROR_POP;
    }

    else if(Lst->head == Lst->tail)
    {
        Lst->head = 0;
        Lst->tail = 0;
    }

    if(pos == Lst->head)
    {
        Lst->prev[Lst->next[Lst->head]] = 0;
        int new_head = Lst->next[Lst->head];
        Lst->next[Lst->head] = 0;
        Lst->head = new_head;
    }

    else if(pos == Lst->tail)
    {
        Lst->next[Lst->prev[Lst->tail]] = 0;
        int new_tail = Lst->prev[Lst->tail];
        Lst->prev[Lst->tail] = 0;
        Lst-> tail = new_tail;
    }

    else
    {
        Lst->next[Lst->prev[pos]] = Lst->next[pos];
        Lst->prev[Lst->next[pos]] = Lst->prev[pos];
    }

    Lst->next[pos] = 0;
    Lst->prev[pos] = 0;
    Lst->buffer[pos] = -1;

    debug_list (Lst);

    Lst->size--;
}

//-----------------------------------------------------------------------------

void debug_list (List *Lst)
{
    printf ("_________________________LIST__________________________________\n\n");

    printf ("%d - HEAD\n", Lst->head);
    printf ("%d - TAIL\n", Lst->tail);

    printf ("_______________________________________________________________\n\n");

    for (int i = 0; i < Lst->size; i++)
    {
        printf ("%lg ", Lst->buffer[i]);
    }

    printf ("\n");

    for (int i = 0; i < Lst->size; i++)
    {
        printf ("%d ", Lst->next[i]);
    }

    printf ("\n");

    for (int i = 0; i < Lst->size; i++)
    {
        printf ("%d ", Lst->prev[i]);
    }

    printf ("\n_______________________________________________________________\n\n");
}

//-----------------------------------------------------------------------------

void *recalloc (void *buffer, int capacity, int size, int size_of_type)
{
    char *pointer = (char*) realloc ((char*) buffer, capacity * size_of_type);

    if(capacity > size)
    {
        memset (pointer + size * size_of_type, '\0', (capacity - size) * size_of_type);
    }

    return (void*) pointer;
}

//-----------------------------------------------------------------------------


