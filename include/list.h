//! @file list.h

#ifndef   LIST_H
#define   LIST_H

//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <limits.h>
#include <stdint.h>

//-----------------------------------------------------------------------------

#define DELETED_PAR -1
#define LST_INCREASE 1
#define LST_DECREASE 0

#define QUOTES 34

//-----------------------------------------------------------------------------

#define E(suffix) ERROR_##suffix

#define N(suffix) NUM_OF_##suffix

#define L(suffix) LIMIT_##suffix

#define P(suffix) POISON_##suffix

//-----------------------------------------------------------------------------

#define DATA(i) Lst->Data[i]

//-----------------------------------------------------------------------------

#define list_ctor(Lst, capacity) list_ctor_ (Lst, capacity, #Lst, __FILE__,  __LINE__)

#define dot_print(...) fprintf (Lst->Info.dot_file, __VA_ARGS__)

#define dbg_print(...) fprintf (Lst->Info.dbg_file, __VA_ARGS__)

//-----------------------------------------------------------------------------

enum LIST_INFO
{
    L(FILE_NAME_LEN) = 150,
};

//-----------------------------------------------------------------------------

enum POISONS
{
    P(FREE_CELL) = 0xDEBEBA,
};

//-----------------------------------------------------------------------------

typedef struct Node
{
    double value;
    int    next;
    int    prev;
} Node;

//-----------------------------------------------------------------------------

typedef struct List_info
{
    const char *name;
    const char *file;
    int         line;
    FILE       *dbg_file;
    FILE       *dot_file;
    int         error_codes;
    char       *cur_status;
    int         graph_num;
} List_info;

//-----------------------------------------------------------------------------

typedef struct List
{
    List_info Info;
    Node     *Data;
    int       head;
    int       tail;
    int       capacity;
    int       size;
} List;

//-----------------------------------------------------------------------------

typedef struct Error_info
{
    int     error_code;
    char   *error_output;
} Error_info;

//-----------------------------------------------------------------------------

enum LIST_ERROR_CODES
{
    E(MEM_DATA)   = (1 << 1),
    E(OVERFLOW)   = (1 << 2),
    E(CAPACITY)   = (1 << 3),
    E(SIZE)       = (1 << 4),
    E(DBG_FILE)   = (1 << 5),
    E(DOT_FILE)   = (1 << 6),
    E(NULL_ELEM)  = (1 << 7),
    E(RESIZE)     = (1 << 8),
    E(DATA_VALUE) = (1 << 9),
    E(PUSH)       = (1 << 11),
    E(POP)        = (1 << 12),
    E(GET_POS)    = (1 << 12),
    E(FATAL)      = 0xBADBED,
};

//-----------------------------------------------------------------------------

enum LIST_ERROR_CODES_INFO
{
    N(MISTAKES) = 12,
};

//-----------------------------------------------------------------------------

const Error_info error_arr[]
{
    {E(MEM_DATA),   "ERROR - null pointer to data                  \n"},
    {E(OVERFLOW),   "ERROR - list overflow                         \n"},
    {E(CAPACITY),   "ERROR - incorrect capacity value (below zero) \n"},
    {E(SIZE),       "ERROR - incorrect size value (below zero)     \n"},
    {E(DBG_FILE),   "ERROR - incorrect dbg FILE pointer            \n"},
    {E(DOT_FILE),   "ERROR - incorrect dot FILE pointer            \n"},
    {E(NULL_ELEM),  "ERROR - nul elem error                        \n"},
    {E(RESIZE),     "ERROR - wrong resize parameter                \n"},
    {E(DATA_VALUE), "ERROR - wrong data values                     \n"},
    {E(PUSH),       "ERROR - push in incorrect position            \n"},
    {E(POP),        "ERROR - pop - incorrect position              \n"},
    {E(GET_POS),    "ERROR - get_logc_position - incorrect position\n"},
};

//-----------------------------------------------------------------------------

void  list_push_right (List *Lst, double value, int insert_pos);

void  list_push_head  (List *Lst, double value);

void  list_push_tail  (List *Lst, double value);

void  free_space_ctor (List *Lst);

int   init_node       (List *Lst);

void  list_resize     (List *Lst, int opt_resize);

void  list_dtor       (List *Lst);

void  list_info_dtor  (List *Lst);

int   get_logic_pos   (List* Lst, int logic_pos);

void  list_linerize   (List* Lst);

int   list_ctor_      (List       *Lst,       int capacity_ctor, const char* lst_name,
                       const char* file_name, int lst_line                            );

void  list_info_ctor  (List       *Lst,       const char* lst_name,
                       const char* file_name, int         lst_line );

void  debug_list      (List *Lst);

void *recalloc        (void *buffer, int capacity, int size, int size_of_type);

void  list_pop        (List *Lst, int del_pos);

void  handle_errors   (List *Lst);

int   verificate_list (List *Lst);

void  make_list_graph (List *Lst);

//-----------------------------------------------------------------------------

#endif //LIST_H
