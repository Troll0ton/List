#include "../include/list.h"

//-----------------------------------------------------------------------------

// capacity
// log_file

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
                // ??
                //
                DATA(curr_pos).value = P(FREE_CELL);
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
        for(int i = 0; i < N(MISTAKES); i++)
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
        if((int) DATA(curr_pos).value == P(FREE_CELL))
        {
            DATA(curr_pos).next = 0;
            DATA(curr_pos).prev = DELETED_PAR;
            prev_free = DATA(prev_free).next = curr_pos;
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
        Lst->Info.error_codes |= E(RESIZE);
    }

    Lst->Data = (Node*) recalloc (Lst->Data, Lst->capacity, Lst->size, sizeof (Node));

    free_space_ctor (Lst);

    verificate_list (Lst);
}

//----------------------------------------------------------------------------

int init_node (List *Lst)
{
    if(DATA(0).next == 0 || ++(Lst->size) > Lst->capacity)
    {
        list_resize (Lst, LST_INCREASE);
    }

    int curr_pos = DATA(0).next;

    DATA(0).next = DATA(DATA(0).next).next;

    DATA(curr_pos).value = 0;
    DATA(curr_pos).next  = 0;
    DATA(curr_pos).prev  = 0;

    return curr_pos;

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------
// push_front

void list_push_head (List *Lst, double value)
{
    int curr_pos = init_node (Lst);

    DATA(curr_pos).value = value;

    if(Lst->tail == 0)
    {
        Lst->tail = curr_pos;
    }

    else
    {
        DATA(Lst->head).prev = curr_pos;
        DATA(curr_pos).next  = Lst->head;
    }

    Lst->head = curr_pos;

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------
// push_back

void list_push_tail (List *Lst, double value)
{
    int curr_pos = init_node (Lst);

    DATA(curr_pos).value = value;

    if(Lst->head == 0)
    {
        Lst->head = curr_pos;
    }

    else
    {
        DATA(Lst->tail).next = curr_pos;
        DATA(curr_pos).prev  = Lst->tail;
    }

    Lst->tail = curr_pos;

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

// push_after

void list_push_right (List *Lst, double value, int insert_pos)
{
    if(insert_pos < 1                              ||
       insert_pos > Lst->tail                      ||
       (int) DATA(insert_pos).value == P(FREE_CELL)  )
    {
        Lst->Info.error_codes |= E(PUSH);
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

            DATA(curr_pos).value             = value;
            DATA(curr_pos).prev              = insert_pos;
            DATA(curr_pos).next              = DATA(insert_pos).next;
            DATA(DATA(insert_pos).next).prev = curr_pos;
            DATA(insert_pos).next            = curr_pos;
        }
    }

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

int get_logic_pos (List* Lst, int logic_pos)
{
    if(logic_pos < 1)
    {
        Lst->Info.error_codes |= E(GET_POS);
    }

// phys_pos
    int physc_pos = Lst->head;

    for(int i = 0; i < logic_pos - 1; i++)
    {
        physc_pos = DATA(physc_pos).next;

        if(physc_pos == 0)
        {
            Lst->Info.error_codes |= E(GET_POS);

            break;
        }
    }

    verificate_list (Lst);

    return physc_pos;
}

//-----------------------------------------------------------------------------

void list_linearize (List* Lst)
{
    List Lst_auxl = { 0 };

    list_ctor (&Lst_auxl, Lst->capacity);

    int curr_pos = Lst->head;

    for(int i = 1; i < Lst->size - 1; i++)
    {
        // NEW_DATA
        DATA_AUX(i)      = DATA(curr_pos);
        DATA_AUX(i).prev = i - 1;
        DATA_AUX(i).next = i + 1;

        curr_pos = DATA(curr_pos).next;
    }

    // Last element

    DATA_AUX(Lst->size - 1).value = DATA(Lst->tail).value;
    DATA_AUX(Lst->size - 1).next  = 0;
    DATA_AUX(Lst->size - 1).prev  = Lst->size - 2;

    for(int i = Lst->size; i < Lst->capacity - 1; i++)
    {
        DATA_AUX(i).value = P(FREE_CELL);
        DATA_AUX(i).next  = i + 1;
        DATA_AUX(i).prev  = DELETED_PAR;
    }

    DATA_AUX(Lst->capacity - 1).next = 0;
    DATA_AUX(Lst->capacity - 1).prev = DELETED_PAR;

    for(int i = 1; i < Lst->capacity; i++)
    {
        DATA(i) = DATA_AUX(i);
    }

    list_dtor (&Lst_auxl);

    DATA(0).next = Lst->size;
    Lst->head    = 1;
    Lst->tail    = Lst->size - 1;

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------

void list_pop (List *Lst, int del_pos)
{
    if(del_pos > Lst->size ||
       del_pos < 1         ||
       (int) DATA(del_pos).value == P(FREE_CELL))
    {
        Lst->Info.error_codes |= E(POP);
    }

    else if(Lst->head == Lst->tail && Lst->head == del_pos)
    {
        Lst->head = 0;
        Lst->tail = 0;
    }

    else if(del_pos == Lst->head)
    {
        DATA(DATA(Lst->head).next).prev = 0;
        Lst->head = DATA(Lst->head).next;
    }

    else if(del_pos == Lst->tail)
    {
        DATA(DATA(Lst->tail).prev).next = 0;
        Lst->tail = DATA(Lst->tail).prev;
    }

    else
    {
        DATA(DATA(del_pos).prev).next = DATA(del_pos).next;
        DATA(DATA(del_pos).next).prev = DATA(del_pos).prev;
    }

    DATA(del_pos).value = P(FREE_CELL);
    DATA(del_pos).next  = DATA(0).next;
    DATA(del_pos).prev  = DELETED_PAR;
    DATA(0).next        = del_pos;
    Lst->size--;

    verificate_list (Lst);

    if(Lst->size < Lst->capacity / 4)
    {
        list_resize (Lst, LST_DECREASE);
    }

    verificate_list (Lst);
}

//-----------------------------------------------------------------------------
// list_dump

void debug_list (List *Lst)
{
    list_linearize (Lst);

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
               Lst->head, Lst->tail, DATA(0).next, Lst->size, Lst->capacity);

    make_list_graph (Lst);

    dbg_print ("_______________________________________________________________ \n</pre>\n\n\n\n\n");
}

//-----------------------------------------------------------------------------

void make_list_graph (List *Lst)
{
    int curr_pos = Lst->head;
    int id       = 1;
    // curr_elem
    // free_elem_start

    Lst->Info.dot_file = fopen ("../dump/list_graph.dot", "w+");

    if(Lst->Info.dot_file == NULL) Lst->Info.error_codes |= E(DOT_FILE);

    verificate_list (Lst);

    dot_print ("digraph structs {                                                                  \n"
               "rankdir = TB;                                                                      \n"
               "graph [dpi = 100];                                                                 \n"
               "ranksep = 1.5;                                                                     \n"
               "edge[penwidth = 10];                                                               \n"
               "cell0 [style = filled, color = black, fillcolor = paleturquoise1,                  \n"
               "shape=record,label = \" { <nul> id: 0 | NULL | <frn> FREE: %d | <prv> NULL } \" ]; \n",
               DATA(0).next);

    while(1)
    {
        dot_print ("cell%d [style = filled, color = black, fillcolor = lightsalmon, "
                   "shape=record,label = \" { <idk> id: %d | value:                 ",
                   id, id);

        if((int) DATA(curr_pos).value == P(FREE_CELL)) dot_print ("_NAN_ ");
        else                                           dot_print ("%5lg ", DATA(curr_pos).value);

        dot_print ("|<nxt> next: %5d | <prv> prev: ", DATA(curr_pos).next);

        if((int) DATA(curr_pos).prev == DELETED_PAR) dot_print ("_____");
        else                                         dot_print ("%5d", DATA(curr_pos).prev);

        dot_print ("} \" ];\n");

        if((int) DATA(curr_pos).next == 0) break;

        curr_pos = DATA(curr_pos).next;

        id++;
    }

    int id_fill = id;
    id++;

    curr_pos = DATA(0).next;

    while(1)
    {
        dot_print ("  cell%d [style = filled , color = black, fillcolor = palegreen1,"
                   "  shape=record,label = \" { <idk> id: %d | value: FREE           ",
                   id, id);

        dot_print ("| <frn> next: %5d | <prv> prev: FREE } \" ];\n", DATA(curr_pos).next);

        if((int) DATA(curr_pos).next == 0) break;

        curr_pos = DATA(curr_pos).next;

        id++;
    }

    for(int i = 0; i < id; i++)
    {
        dot_print ("cell%d -> cell%d [weight = 5, style = invis];\n", i, i + 1);
    }

    dot_print ("\nsplines = ortho\n"
               "{rank = same;      ");

    for(int i = 0; i <= id; i++)
    {
         dot_print ("cell%d; ", i);
    }

    dot_print ("}\n \n");

    for(int i = 1; i < id_fill; i++)
    {
         dot_print ("cell%d: <nxt> -> cell%d: <nxt> [weight = 5];\n", i,     i + 1);
         dot_print ("cell%d: <prv> -> cell%d: <prv> [weight = 5];\n", i + 1, i    );
    }

    dot_print ("cell0:  <nul> -> cell1: <idk> [weight = 5];  \n"
               "cell1:  <prv> -> cell0: <prv> [weight = 5];  \n");


    if(id - id_fill - 1 > 0) dot_print ("cell0: <nul> -> cell%d: <idk> [weight = 5];\n", id_fill + 1);

    for(int i = id_fill + 1; i < id; i++)
    {
         dot_print ("cell%d: <frn> -> cell%d: <frn> [weight = 5];\n", i, i + 1);
    }

    dot_print ("}\n");

    fclose (Lst->Info.dot_file);

    char img_name[L(FILE_NAME_LEN)] = "";
    char dot_name[L(FILE_NAME_LEN)] = "";

    sprintf (img_name, "<img width=\"1400px\" src=\"../dump/graph%d.png\">   \n", Lst->Info.graph_num);
    sprintf (dot_name, "dot -Tpng ../dump/list_graph.dot -o ../dump/graph%d.png", Lst->Info.graph_num);
    Lst->Info.graph_num++;

    system    (dot_name);
    dbg_print (img_name);
}

//-----------------------------------------------------------------------------

int verificate_list (List *Lst)
{
    handle_errors (Lst);

    if(Lst->Data == NULL)
    {
        Lst->Info.error_codes |= E(MEM_DATA);
    }

    if(Lst->size > Lst->capacity)
    {
        Lst->Info.error_codes |= E(OVERFLOW);
    }

    if(Lst->capacity < 0)
    {
        Lst->Info.error_codes |= E(CAPACITY);
    }

    if(Lst->size < 0)
    {
        Lst->Info.error_codes |= E(SIZE);
    }

    if(Lst->Info.dbg_file == NULL)
    {
        Lst->Info.error_codes |= E(DBG_FILE);
    }

    if((Lst->head == 0 && Lst->tail != 0) ||
       (Lst->head != 0 && Lst->tail == 0) ||
        Lst->head < 0  || Lst->tail < 0     )
    {
        Lst->Info.error_codes |= E(DATA_VALUE);
    }

    if((int) DATA(0).value != 0 ||
       (int) DATA(0).prev  != 0   )
    {
        Lst->Info.error_codes |= E(NULL_ELEM);
    }

    if (Lst->Info.error_codes != 0)
    {
        Lst->Info.cur_status = "ERROR";

        return E(FATAL);
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


