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

//-----------------------------------------------------------------------------

enum Pars
{
    LST_INCREASE = 1,
    LST_DECREASE = 0,
    DELETED_PAR  = -1,
    NANV         = 111,
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
    int       free;
    int       capacity;
    int       size;
} List;

//-----------------------------------------------------------------------------

void  list_push_right (List *Lst, double value, int insert_pos);

void  list_push_head  (List *Lst, double value);

void  list_push_tail  (List *Lst, double value);

void  free_space_ctor (List *Lst);

int   init_node       (List *Lst);

void  list_resize     (List *Lst, int opt_resize);

void  list_dtor       (List *Lst);

void  list_info_dtor  (List *Lst);

void  list_ctor_      (List       *Lst,       int capacity_ctor, const char* lst_name,
                       const char* file_name, int lst_line                            );

void  list_info_ctor  (List       *Lst,       const char* lst_name,
                       const char* file_name, int         lst_line );

void  debug_list      (List *Lst);

void *recalloc        (void *buffer, int capacity, int size, int size_of_type);

void  list_pop        (List *Lst, int del_pos);

//-----------------------------------------------------------------------------

#endif //LIST_H
