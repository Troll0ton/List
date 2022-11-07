#include "../include/list.h"

//-----------------------------------------------------------------------------

int main ()
{
    List Lst = { 0 };

    if(list_ctor (&Lst, 15) == ERROR_LST)
    {
        printf ("ERROR - CTOR\n");

        return -1;
    }

    list_push_head (&Lst, 300);
    list_push_head (&Lst, 200);
    list_push_head (&Lst, 100);

    debug_list (&Lst);

    list_push_tail (&Lst, 400);
    list_push_tail (&Lst, 500);
    list_push_right (&Lst, 600, 5);

    list_pop (&Lst, 5);
    list_pop (&Lst, 4);

    list_push_head (&Lst, 656);

    debug_list (&Lst);

    rewrite_lst_lgc (&Lst);
    list_pop (&Lst, 1);
    list_pop (&Lst, 5);

    rewrite_lst_lgc (&Lst);

    make_list_graph (&Lst);

    debug_list (&Lst);

    list_dtor (&Lst);

    return 0;
}

//-----------------------------------------------------------------------------

