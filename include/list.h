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
    LST_INCREASE 1
    LST_DECREASE 0
};

//-----------------------------------------------------------------------------

enum ERROR_LCODES
{
    ERR_CAP     = (1 << 1),
    ERR_DATA    = (1 << 2),
    ERR_RESIZE  = (1 << 3),
};

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

typedef struct Error_info
{
    int     error_code;
    char   *error_output;
} Error_info;

//-----------------------------------------------------------------------------

const struct Error_info error_arr_l[]
{
    {ERR_CAP,    "ERROR - incorrect capacity value (below zero)                      \n"},
    {ERR_DATA,   "ERROR - wrong calloc working                                       \n"},
    {ERR_RESIZE, "ERROR - incorrect resize parameter                                 \n"},
};

//-----------------------------------------------------------------------------

typedef struct List
{
    List_info Info;
    double   *buffer;
    int      *next;
    int      *prev;
    int       head;
    int       tail;
    int       free;
    int       capacity;
    int       size;
} List;

//-----------------------------------------------------------------------------

void list_push_in   (List *Lst, double elem, int pos);

void list_push_head (List *Lst, double elem);

void list_resize    (List *Lst, int opt_resize);

void list_dtor      (List *Lst);

int  list_ctor_     (List *Lst,             int capacity_ctor, const char* lst_name,
                     const char* file_name, int lst_line                            );

void debug_list     (List *Lst);

void *recalloc      (void *buffer, int capacity, int size);

void list_pop       (List *Lst, int pos);

//-----------------------------------------------------------------------------

#endif //LIST_H
