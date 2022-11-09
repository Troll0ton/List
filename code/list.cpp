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
    Lst->Info.name      = lst_name;
    Lst->Info.line      = lst_line;
    Lst->Info.file      = file_name;
    Lst->Info.graph_num = 1;

    Lst->Info.dbg_file  = fopen ("../dump/list_dump.html",  "w+");

    Lst->Info.cur_status  = "OK";
    Lst->Info.error_codes = 0;
}

//-----------------------------------------------------------------------------

void handle_errors (List *Lst)
{
    if(Lst->Info.error_codes != 0)
    {
        // N(ERROR)
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
    Lst->size     = DELETED_PAR;
    Lst->capacity = DELETED_PAR;
    Lst->head     = DELETED_PAR;
    Lst->tail     = DELETED_PAR;

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

    Lst->Data = (Node*) recalloc (Lst->Data, Lst->capacity, Lst->size, sizeof (Node));

    free_space_ctor (Lst);

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

#define DATA(i) Lst->Data[i]
// ...

int init_node (List *Lst)
{
    if(Lst->Data[0].next == 0 || ++(Lst->size) > Lst->capacity)
    {
        list_resize (Lst, LST_INCREASE);
    }

    int curr_pos = Lst->Data[0].next;

    // free <=> Data[0].next

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
    if(insert_pos < 1                        ||
       insert_pos > Lst->tail                ||
       (int) Lst->Data[insert_pos].value == POISON  )
    {
        Lst->Info.error_codes |= ERR_PUSH;
    }

    else
    {
        if(insert_pos == Lst->tail)
        {
            list_push_tail (Lst, value);
        }

        else
        {
            int curr_pos = init_node (Lst);

            Lst->Data[curr_pos].value                  = value;
            Lst->Data[curr_pos].prev                   = insert_pos;
            Lst->Data[curr_pos].next                   = Lst->Data[insert_pos].next;
            Lst->Data[Lst->Data[insert_pos].next].prev = curr_pos;
            Lst->Data[insert_pos].next                 = curr_pos;
        }
    }

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

int get_logic_pos (List* Lst, int logic_pos)
{
    if(logic_pos < 1)
    {
        Lst->Info.error_codes |= ERR_GET_POS;
    }

    int physc_pos = Lst->head;

    for(int i = 0; i < logic_pos - 1; i++)
    {
        physc_pos = Lst->Data[physc_pos].next;

        if(physc_pos == 0)
        {
            Lst->Info.error_codes |= ERR_GET_POS;

            break;
        }
    }

    verificate_list (Lst);

    return physc_pos;
}

//-----------------------------------------------------------------------------
// list_linerize

void rewrite_lst_lgc (List* Lst)
{
    List Lst_auxl = { 0 };

    list_ctor (&Lst_auxl, Lst->capacity);

    int curr_pos = Lst->head;

    for(int i = 1; i < Lst->size - 1; i++)
    {
        Lst_auxl.Data[i]      = Lst->Data[curr_pos];
        Lst_auxl.Data[i].prev = i - 1;
        Lst_auxl.Data[i].next = i + 1;

        curr_pos = Lst->Data[curr_pos].next;
    }

    Lst_auxl.Data[Lst->size - 1].value = Lst->Data[Lst->tail].value;
    Lst_auxl.Data[Lst->size - 1].next  = 0;
    Lst_auxl.Data[Lst->size - 1].prev  = Lst->size - 2;

    for(int i = Lst->size; i < Lst->capacity - 1; i++)
    {
        Lst_auxl.Data[i].value = POISON;
        Lst_auxl.Data[i].next  = i + 1;
        Lst_auxl.Data[i].prev  = DELETED_PAR;
    }

    Lst_auxl.Data[Lst->capacity - 1].next = 0;
    Lst_auxl.Data[Lst->capacity - 1].prev = DELETED_PAR;

    for(int i = 1; i < Lst->capacity; i++)
    {
        Lst->Data[i] = Lst_auxl.Data[i];
    }

    list_dtor (&Lst_auxl);

    Lst->Data[0].next = Lst->size;
    Lst->head         = 1;
    Lst->tail         = Lst->size - 1;

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

void list_pop (List *Lst, int del_pos)
{
    if(del_pos > Lst->size ||
       del_pos < 1         ||
       (int) Lst->Data[del_pos].value == POISON)
    {
        Lst->Info.error_codes |= ERR_POP;
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
    Lst->Data[0].next        = del_pos;
    Lst->size--;

    verificate_list (Lst);

    if(Lst->size < Lst->capacity / 4)
    {
        list_resize (Lst, LST_DECREASE);
    }

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

void debug_list (List *Lst)
{
    rewrite_lst_lgc (Lst);

    dbg_print ("<pre>\n"
               "_________________________LIST__________________________________\n\n"
               "%s[%p](%s) at %s, LINE - %d \n"
               "%d - head\n"
               "%d - tail\n"
               "%d - free\n"
               "%d - size\n"
               "%d - capacity\n"
               "_______________________________________________________________\n\n",
               Lst->Info.name + 1,    Lst,
               Lst->Info.cur_status, (Lst->Info).file,
               Lst->Info.line,
               Lst->head, Lst->tail, Lst->Data[0].next, Lst->size, Lst->capacity);

    make_list_graph (Lst);

    dbg_print ("_______________________________________________________________ \n</pre>\n\n\n\n\n");
}

//-----------------------------------------------------------------------------

void make_list_graph (List *Lst)
{
    int curr_pos = Lst->head;
    int id       = 1;

    Lst->Info.dot_file = fopen ("../dump/list_graph.dot", "w+");

    if(Lst->Info.dot_file == NULL) Lst->Info.error_codes |= ERR_DOTFILE;

    verificate_list (Lst);

    dot_print ("digraph structs {          \n"
               "rankdir   =  TB;           \n"
               "cell0 [style = filled , color = black, fillcolor = paleturquoise1, "
               "shape=record,label=%c { <nul> id: 0 | NULL | <frn0> FREE: %d | NULL } %c ];\n",
               QUOTES, Lst->Data[0].next, QUOTES);

    while(1)
    {
        dot_print ("cell%d [style = filled, color = black, fillcolor = lightsalmon, "
                   "shape=record,label=%c { id: %d | value: ",
                   id, QUOTES, id);

        if((int) Lst->Data[curr_pos].value == POISON) dot_print ("_NAN_ ");
        else                                          dot_print ("%5lg ", Lst->Data[curr_pos].value);

        dot_print ("|<nxt%d> next: %5d | <prv%d> prev: ", id, Lst->Data[curr_pos].next, id);

        if((int) Lst->Data[curr_pos].prev == DELETED_PAR) dot_print ("_____");
        else                                              dot_print ("%5d", Lst->Data[curr_pos].prev);

        dot_print ("} %c ];\n", QUOTES);

        if((int) Lst->Data[curr_pos].next == 0) break;

        curr_pos = Lst->Data[curr_pos].next;

        id++;
    }

    int id_fill = id;
    id++;

    curr_pos = Lst->Data[0].next;

    while(1)
    {
        dot_print ("  cell%d [style = filled , color = black, fillcolor = palegreen1, "
                   "shape=record,label=%c { id: %d | value: FREE ",
                   id, QUOTES, id);

        dot_print ("| <frn%d> next: %5d | prev: FREE } %c ];\n", id, Lst->Data[curr_pos].next, QUOTES);

        if((int) Lst->Data[curr_pos].next == 0) break;

        curr_pos = Lst->Data[curr_pos].next;

        id++;
    }

    dot_print ("{ rank = same; ");

    for(int i = 0; i <= id; i++)
    {
         dot_print ("cell%d; ", i);
    }

    dot_print ("}\n edge[constraint = false]\n");

    for(int i = 1; i < id_fill; i++)
    {
         dot_print ("cell%d: <nxt%d> -> cell%d: <nxt%d>;\n", i, i, i + 1, i + 1);
         dot_print ("cell%d: <prv%d> -> cell%d: <prv%d>;\n", i + 1, i + 1, i, i);
    }

    dot_print ("cell0: <nul> -> cell1: <nxt1>;  \n");

    if(id - id_fill - 1 > 0) dot_print ("cell0: <frn0> -> cell%d: <frn%d>;\n", id_fill + 1, id_fill + 1);

    for(int i = id_fill + 1; i < id; i++)
    {
         dot_print ("cell%d: <frn%d> -> cell%d: <frn%d>;\n", i, i, i + 1, i + 1);
    }

    dot_print ("}\n");

    fclose (Lst->Info.dot_file);

    char img_name[MAX_FL_NAME_LEN] = "";
    char dot_name[MAX_FL_NAME_LEN] = "";

    sprintf (img_name, "<img width=\"1400px\" src=\"../dump/graph%d.png\"> \n",   Lst->Info.graph_num);
    sprintf (dot_name, "dot -Tpng ../dump/list_graph.dot -o ../dump/graph%d.png", Lst->Info.graph_num);
    Lst->Info.graph_num++;

    system    (dot_name);
    dbg_print (img_name);
}

//-----------------------------------------------------------------------------

int verificate_list (List *Lst)
{
    handle_errors (Lst);

    if(Lst->Data == NULL)                     Lst->Info.error_codes |= ERR_MEMDATA;
    if(Lst->size > Lst->capacity)             Lst->Info.error_codes |= ERR_OVERF;
    if(Lst->capacity < 0)                     Lst->Info.error_codes |= ERR_CAP;
    if(Lst->size < 0)                         Lst->Info.error_codes |= ERR_SIZE;
    if(Lst->Info.dbg_file == NULL)            Lst->Info.error_codes |= ERR_DBGFILE;
    if((Lst->head == 0 && Lst->tail != 0) ||
       (Lst->head != 0 && Lst->tail == 0)   ) Lst->Info.error_codes |= ERR_HT_VALUE;
    if(Lst->head < 0  || Lst->tail < 0)       Lst->Info.error_codes |= ERR_BZ_VALUE;
    if((int) Lst->Data[0].value != 0 ||
       (int) Lst->Data[0].prev  != 0   )      Lst->Info.error_codes |= ERR_NUL_ELEM;

    if (Lst->Info.error_codes != 0)
    {
        Lst->Info.cur_status = "ERROR";

        return ERROR_LST;
    }

    return 0;
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


