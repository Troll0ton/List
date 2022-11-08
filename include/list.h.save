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

#define list_ctor(Lst, capacity) list_ctor_ (Lst, capacity, #Lst, __FILE__,  __LINE__)

#define dot_print(...) list_ctor_ (Lst, __VA_ARGS__)

//-----------------------------------------------------------------------------

enum Pars
{
    LST_INCREASE = 1,
    LST_DECREASE = 0,
    DELETED_PAR  = -1,
    POISON       = 0xDEBEBA,
    NUM_OF_MIS   = 7,
    QUOTES       = 34,
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
    FILE       *dbg_file;
    FILE       *dot_file;
    int         line;
    int         error_codes;
    char       *cur_status;
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

enum ERROR_CODES
{
    ERR_MEMDATA  = (1 << 1),
    ERR_OVERF    = (1 << 2),
    ERR_CAP      = (1 << 3),
    ERR_SIZE     = (1 << 4),
    ERR_DBGFILE  = (1 << 5),
    ERR_DOTFILE  = (1 << 6),
    ERR_NUL_ELEM = (1 << 7),

    ERROR_LST    = 0xBADBED,
};

//-----------------------------------------------------------------------------

const Error_info error_arr[]
{
    {ERR_MEMDATA,  "ERROR - null pointer to data                  \n"},
    {ERR_OVERF,    "ERROR - list overflow                         \n"},
    {ERR_CAP,      "ERROR - incorrect capacity value (below zero) \n"},
    {ERR_SIZE,     "ERROR - incorrect size value (below zero)     \n"},
    {ERR_DBGFILE,  "ERROR - incorrect dbg FILE pointer            \n"},
    {ERR_DOTFILE,  "ERROR - incorrect dot FILE pointer            \n"},
    {ERR_NUL_ELEM, "ERROR - nul elem error                        \n"},
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

void  rewrite_lst_lgc (List* Lst);

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
