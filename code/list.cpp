#include "../include/list.h"

//-----------------------------------------------------------------------------

void list_ctor_ (List       *Lst,       int capacity_ctor, const char* lst_name,
                 const char* file_name, int lst_line                            )
{
    Lst->capacity = capacity_ctor + 1;
    Lst->size = 1;
    Lst->head = 0;
    Lst->tail = 0;

    if (capacity_ctor >= 0)
    {
        Lst->Data = (Node*) calloc (capacity_ctor + 1, sizeof (Node));

        if(Lst->Data == NULL)
        {
            printf ("ERROR - Data\n");
        }

        for(int curr_pos = 1; curr_pos < Lst->capacity; curr_pos++)
        {
            Lst->Data[curr_pos].value = NANV;
        }
    }

    else
    {
        printf ("ERROR - CAP\n");
    }

    free_space_ctor (Lst);

    list_info_ctor (Lst, lst_name, file_name, lst_line);
}

//-----------------------------------------------------------------------------

void list_info_ctor (List       *Lst,       const char* lst_name,
                     const char* file_name, int         lst_line )
{
    Lst->Info.name = lst_name;
    Lst->Info.file = file_name;
    Lst->Info.line = lst_line;

    Lst->Info.dbg_file    = fopen ("../dump/list_dump.txt", "w+");
    Lst->Info.error_codes = 0;
    Lst->Info.cur_status  = "OK";

    if(Lst->Info.dbg_file == NULL)
    {
        printf ("ERROR - FILE\n");
    }
}

//-----------------------------------------------------------------------------

void free_space_ctor (List *Lst)
{
    int prev_free = 0;

    for(int curr_pos = 1; curr_pos < Lst->capacity; curr_pos++)
    {
        if((int) Lst->Data[curr_pos].value == NANV)
        {
            Lst->Data[curr_pos].next  = 0;
            Lst->Data[curr_pos].prev  = DELETED_PAR;
            Lst->Data[prev_free].next = curr_pos;
            prev_free                 = curr_pos;
        }
    }

    Lst->free = Lst->Data[0].next;
}

//-----------------------------------------------------------------------------

void list_dtor (List *Lst)
{
    Lst->size = DELETED_PAR;
    Lst->head = DELETED_PAR;
    Lst->tail = DELETED_PAR;

    free (Lst->Data);

    list_info_dtor (Lst);
}

//-----------------------------------------------------------------------------

void list_info_dtor (List *Lst)
{
    fclose (Lst->Info.dbg_file);

    Lst->Info.error_codes = DELETED_PAR;
    Lst->Info.cur_status  = "DELETED";
}

//-----------------------------------------------------------------------------

void list_resize (List *Lst, int opt_resize)
{
    int prev_capacity = Lst->capacity;

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
        printf ("ERROR - RESIZE\n");
    }

    Lst->Data = (Node*) recalloc (Lst->Data, Lst->capacity, Lst->size, sizeof (Node));

    for(int curr_pos = prev_capacity; curr_pos < Lst->capacity; curr_pos++)
    {
        Lst->Data[curr_pos].value = NANV;
    }

    free_space_ctor (Lst);
}

//-----------------------------------------------------------------------------

int init_node (List *Lst)
{
    if(Lst->free == 0 || ++(Lst->size) > Lst->capacity)
    {
        list_resize (Lst, LST_INCREASE);
    }

    int curr_pos = Lst->free;

    Lst->Data[0].next = Lst->free = Lst->Data[Lst->free].next;

    Lst->Data[curr_pos].value = 0;
    Lst->Data[curr_pos].next  = 0;
    Lst->Data[curr_pos].prev  = 0;

    return curr_pos;
}

//-----------------------------------------------------------------------------

void list_push_head (List *Lst, double value)
{
    int curr_pos = init_node (Lst);

    Lst->Data[curr_pos].value = value;

    if(Lst->tail == 0)
    {
        Lst->tail = curr_pos;
    }

    else
    {
        Lst->Data[Lst->head].prev = curr_pos;
        Lst->Data[curr_pos].next  = Lst->head;
    }

    Lst->head = curr_pos;
}

//-----------------------------------------------------------------------------

void list_push_tail (List *Lst, double value)
{
    int curr_pos = init_node (Lst);

    Lst->Data[curr_pos].value = value;

    if(Lst->head == 0)
    {
        Lst->head = curr_pos;
    }

    else
    {
        Lst->Data[Lst->tail].next = curr_pos;
        Lst->Data[curr_pos].prev  = Lst->tail;
    }

    Lst->tail = curr_pos;
}

//-----------------------------------------------------------------------------

void list_push_right (List *Lst, double value, int insert_pos)
{
    if(insert_pos == Lst->tail)
    {
        list_push_tail (Lst, value);
    }

    else
    {
        int curr_pos = init_node (Lst);

        Lst->Data[curr_pos].value = value;

        Lst->Data[curr_pos].prev                   = insert_pos;
        Lst->Data[curr_pos].next                   = Lst->Data[insert_pos].next;
        Lst->Data[Lst->Data[insert_pos].next].prev = curr_pos;
        Lst->Data[insert_pos].next                 = curr_pos;
    }
}

//-----------------------------------------------------------------------------

void list_pop (List *Lst, int del_pos)
{
    if(del_pos > Lst->size || (int) Lst->Data[del_pos].value == NANV)
    {
        printf ("ERROR - POP\n");
    }

    else if(Lst->head == Lst->tail && Lst->head == del_pos)
    {
        Lst->head = 0;
        Lst->tail = 0;
    }

    else if(del_pos == Lst->head)
    {
        Lst->Data[Lst->Data[Lst->head].next].prev = 0;
        Lst->head = Lst->Data[Lst->head].next;
    }

    else if(del_pos == Lst->tail)
    {
        Lst->Data[Lst->Data[Lst->tail].prev].next = 0;
        Lst->tail = Lst->Data[Lst->tail].prev;
    }

    else
    {
        Lst->Data[Lst->Data[del_pos].prev].next = Lst->Data[del_pos].next;
        Lst->Data[Lst->Data[del_pos].next].prev = Lst->Data[del_pos].prev;
    }

    Lst->Data[del_pos].value = NANV;
    Lst->Data[del_pos].next = Lst->Data[0].next;
    Lst->Data[del_pos].prev = DELETED_PAR;
    Lst->Data[0].next = del_pos;
    Lst->free = del_pos;
    Lst->size--;

    if(Lst->size < Lst->capacity / 4)
    {
        list_resize (Lst, LST_DECREASE);
    }
}

//-----------------------------------------------------------------------------

void debug_list (List *Lst)
{
    printf ("_________________________LIST__________________________________\n\n");

    printf ("%d - HEAD\n", Lst->head);
    printf ("%d - TAIL\n", Lst->tail);
    printf ("%d - free\n", Lst->free);

    printf ("_______________________________________________________________\n\n");

    for (int i = 0; i < Lst->capacity; i++)
    {
        printf (" %3d  ", i);
    }
    printf ("\n");

    for (int i = 0; i < Lst->capacity; i++)
    {
        if(Lst->Data[i].value != 111) printf ("|%3lg| ", Lst->Data[i].value);
        else printf ("|___| ");
    }

    printf ("\n");

    for (int i = 0; i < Lst->capacity; i++)
    {
        printf ("|%3d| ", Lst->Data[i].next);
    }

    printf ("\n");

    for (int i = 0; i < Lst->capacity; i++)
    {
        printf ("|%3d| ", Lst->Data[i].prev);
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


