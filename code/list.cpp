#include "../include/list.h"

//-----------------------------------------------------------------------------

int list_ctor_ (List       *Lst,       int capacity_ctor, const char* lst_name,
                const char* file_name, int lst_line                            )
{
    Lst->capacity = capacity_ctor + 1;
    Lst->size = 1;
    Lst->head = 0;
    Lst->tail = 0;

    list_info_ctor (Lst, lst_name, file_name, lst_line);

    if(capacity_ctor > 0)
    {
        Lst->Data = (Node*) calloc (capacity_ctor + 1, sizeof (Node));

        if(Lst->Data != NULL)
        {
            for(int curr_pos = 1; curr_pos < Lst->capacity; curr_pos++)
            {
                Lst->Data[curr_pos].value = POISON;
            }
        }
    }

    free_space_ctor (Lst);

    return verificate_list (Lst);
}

//-----------------------------------------------------------------------------

void list_info_ctor (List       *Lst,       const char* lst_name,
                     const char* file_name, int         lst_line )
{
    Lst->Info.name = lst_name;
    Lst->Info.file = file_name;
    Lst->Info.line = lst_line;

    Lst->Info.dbg_file = fopen ("../dump/list_dump.txt",  "w+");
    Lst->Info.dot_file = fopen ("../dump/list_graph.dot", "w+");

    Lst->Info.error_codes = 0;
    Lst->Info.cur_status  = "OK";
}

//-----------------------------------------------------------------------------

int verificate_list (List *Lst)
{
    handle_errors (Lst);

    if(Lst->Data == NULL)          Lst->Info.error_codes |= ERR_MEMDATA;
    if(Lst->size > Lst->capacity)  Lst->Info.error_codes |= ERR_OVERF;
    if(Lst->capacity < 0)          Lst->Info.error_codes |= ERR_CAP;
    if(Lst->size < 0)              Lst->Info.error_codes |= ERR_SIZE;
    if(Lst->Info.dbg_file == NULL) Lst->Info.error_codes |= ERR_DBGFILE;
    if(Lst->Info.dot_file == NULL) Lst->Info.error_codes |= ERR_DOTFILE;
    if((int) Lst->Data[0].value != 0 ||
       (int) Lst->Data[0].prev  != 0   ) Lst->Info.error_codes |= ERR_NUL_ELEM;

    if (Lst->Info.error_codes != 0)
    {
        Lst->Info.cur_status = "ERROR";

        return ERROR_LST;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void handle_errors (List *Lst)
{
    if(Lst->Info.error_codes != 0)
    {
        for(int i = 0; i < NUM_OF_MIS; i++)
        {
            if(Lst->Info.error_codes & error_arr[i].error_code)
            {
                printf ("%d %s\n", error_arr[i].error_code, error_arr[i].error_output);
            }
        }
    }
}

//-----------------------------------------------------------------------------

void free_space_ctor (List *Lst)
{
    int prev_free = 0;

    for(int curr_pos = 1; curr_pos < Lst->capacity; curr_pos++)
    {
        if((int) Lst->Data[curr_pos].value == POISON)
        {
            Lst->Data[curr_pos].next = 0;
            Lst->Data[curr_pos].prev = DELETED_PAR;
            prev_free = Lst->Data[prev_free].next = curr_pos;
        }
    }

    verificate_list (Lst);
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
    fclose (Lst->Info.dot_file);

    Lst->Info.error_codes = DELETED_PAR;
    Lst->Info.cur_status  = "DELETED";
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
        printf ("ERROR - RESIZE\n");
    }

    Lst->Data = (Node*) recalloc (Lst->Data, Lst->capacity, Lst->size, sizeof (Node));

    free_space_ctor (Lst);

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

int init_node (List *Lst)
{
    if(Lst->Data[0].next == 0 || ++(Lst->size) > Lst->capacity)
    {
        list_resize (Lst, LST_INCREASE);
    }

    int curr_pos = Lst->Data[0].next;

    Lst->Data[0].next = Lst->Data[Lst->Data[0].next].next;

    Lst->Data[curr_pos].value = 0;
    Lst->Data[curr_pos].next  = 0;
    Lst->Data[curr_pos].prev  = 0;

    return curr_pos;

    verificate_list (Lst);
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

    verificate_list (Lst);
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

    verificate_list (Lst);
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

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

int get_logic_pos (List* Lst, int logic_pos)
{
    int physc_pos = Lst->head;

    for(int i = 0; i < logic_pos - 1; i++)
    {
        physc_pos = Lst->Data[physc_pos].next;
    }

    verificate_list (Lst);

    return physc_pos;
}

//-----------------------------------------------------------------------------

void rewrite_lst_lgc (List* Lst)
{
    List Lst_auxl = { 0 };

    list_ctor (&Lst_auxl, Lst->capacity);

    int curr_pos = Lst->head;

    for(int i = 1; i < Lst->size; i++)
    {
        Lst_auxl.Data[i] = Lst->Data[curr_pos];
        Lst_auxl.Data[i].prev = i - 1;
        Lst_auxl.Data[i].next = i + 1;

        curr_pos = Lst->Data[curr_pos].next;
    }

    for(int i = Lst->size; i < Lst->capacity - 1; i++)
    {
        Lst_auxl.Data[i].value = POISON;
        Lst_auxl.Data[i].next = i + 1;
        Lst_auxl.Data[i].prev = DELETED_PAR;
    }

    Lst_auxl.Data[Lst->capacity - 1].next = 0;
    Lst_auxl.Data[Lst->capacity - 1].prev = DELETED_PAR;

    for(int i = 1; i < Lst->capacity; i++)
    {
        Lst->Data[i] = Lst_auxl.Data[i];
    }

    list_dtor (&Lst_auxl);

    Lst->Data[0].next = Lst->size;
    Lst->head = 1;
    Lst->tail = Lst->size - 1;

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

void list_pop (List *Lst, int del_pos)
{
    if(del_pos > Lst->size || (int) Lst->Data[del_pos].value == POISON)
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

    Lst->Data[del_pos].value = POISON;
    Lst->Data[del_pos].next  = Lst->Data[0].next;
    Lst->Data[del_pos].prev  = DELETED_PAR;
    Lst->Data[0].next = del_pos;
    Lst->size--;

    if(Lst->size < Lst->capacity / 4)
    {
        list_resize (Lst, LST_DECREASE);
    }

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

void debug_list (List *Lst)
{
    fprintf (Lst->Info.dbg_file, "_________________________LIST__________________________________\n\n"
                                 "%d - HEAD\n"
                                 "%d - TAIL\n"
                                 "%d - free\n"
                                 "_______________________________________________________________\n\n",
                                 Lst->head, Lst->tail, Lst->Data[0].next);

    for(int i = 0; i < Lst->capacity; i++)
    {
        fprintf (Lst->Info.dbg_file, " %3d  ", i);
    }

    fprintf (Lst->Info.dbg_file, "\n");

    for (int i = 0; i < Lst->capacity; i++)
    {
        if((int) Lst->Data[i].value == POISON)
        {
            fprintf (Lst->Info.dbg_file, "|___| ");
        }

        else
        {
            fprintf (Lst->Info.dbg_file, "|%3lg| ", Lst->Data[i].value);
        }
    }

    fprintf (Lst->Info.dbg_file, "\n");

    for (int i = 0; i < Lst->capacity; i++)
    {
        fprintf (Lst->Info.dbg_file, "|%3d| ", Lst->Data[i].next);
    }

    fprintf (Lst->Info.dbg_file, "\n");

    for (int i = 0; i < Lst->capacity; i++)
    {
        if(Lst->Data[i].prev == POISON || Lst->Data[i].prev == DELETED_PAR)
        {
            fprintf (Lst->Info.dbg_file, "|___| ");
        }
        else
        {
            fprintf (Lst->Info.dbg_file, "|%3d| ", Lst->Data[i].prev);
        }
    }

    fprintf (Lst->Info.dbg_file, "_______________________________________________________________\n\n\n\n\n");
}

//-----------------------------------------------------------------------------

void make_list_graph (List *Lst)
{
    int curr_pos = 0;
    int id       = 0;

    fprintf (Lst->Info.dot_file, "digraph structs {\n"
                                 "  rankdir=HR;    \n");

    while(1)
    {
        fprintf (Lst->Info.dot_file, "  cell%d [shape=record,label=%c { value: ",
                                     id, QUOTES);

        if((int) Lst->Data[curr_pos].value == POISON) fprintf (Lst->Info.dot_file, "_NAN_ ");
        else                                          fprintf (Lst->Info.dot_file, "%lg ", Lst->Data[curr_pos].value);

        fprintf (Lst->Info.dot_file, "| next: %d | prev: ", Lst->Data[curr_pos].next);

        if((int) Lst->Data[curr_pos].value == DELETED_PAR) fprintf (Lst->Info.dot_file, "_____");
        else                                               fprintf (Lst->Info.dot_file, "%d", Lst->Data[curr_pos].prev);

        fprintf (Lst->Info.dot_file, "}%c ];\n", QUOTES);

        if((int) Lst->Data[curr_pos].next == 0) break;

        curr_pos = Lst->Data[curr_pos].next;

        id++;
    }

    fprintf (Lst->Info.dot_file, "  ");

    for(int i = 0; i < id; i++)
    {
         fprintf (Lst->Info.dot_file, "cell%d -> ", i);
    }

    fprintf (Lst->Info.dot_file, "cell%d;\n", id);
    fprintf (Lst->Info.dot_file, "}\n");
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


