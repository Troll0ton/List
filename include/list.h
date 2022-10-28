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

#define list_ctor(lst, capacity) list_ctor_ (lst, capacity, #lst, __FILE__,  __LINE__)
//#define list_dump(lst)           stack_dump_ (lst,                 __FILE__,  __LINE__)

//-----------------------------------------------------------------------------

#define lst_increase 1
#define lst_decrease 0

//-----------------------------------------------------------------------------

enum ERROR_CODES_L
{
    ERR_CAP     = (1 << 1),
    ERR_RESIZE  = (1 << 2),
};

//-----------------------------------------------------------------------------

struct Node
{
   int key;
   double data;

   struct Node *next;
   struct Node *prev;
};

//-----------------------------------------------------------------------------

typedef struct ListInfo
{
    const char *name;
    const char *file;
    FILE       *dbg_file;
    int         line;
    int32_t     error_codes;
    char       *cur_status;
} ListInfo;

//-----------------------------------------------------------------------------

typedef struct List
{
    ListInfo  List_info;
    Node     *head;
    Node     *buffer;
    Node     *tail;
    int       capacity;
    int       size;
} List;

//-----------------------------------------------------------------------------

typedef struct Error_info
{
    int32_t error_code;
    char   *error_output;
} Error_info;

//-----------------------------------------------------------------------------

const struct Error_info error_arr_l[]
{
    {ERR_CAP,    "ERROR - incorrect capacity value (below zero)                      \n"},
    {ERR_RESIZE, "ERROR - incorrect resize parameter                                 \n"},
};

//-----------------------------------------------------------------------------

void list_push   (List *lst, double elem, int pos);

int  list_resize (List *lst, int opt_resize);

void list_dtor   (List *lst);

int  list_ctor_  (List *lst,             int capacity_ctor, const char* lst_name,
                  const char* file_name, int lst_line                            );

void debug_list  (List *lst);

void *recalloc   (void *buffer, int capacity, int size);

//-----------------------------------------------------------------------------

#endif //LIST_H
