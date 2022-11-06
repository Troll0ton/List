#include "../include/list.h"

//-----------------------------------------------------------------------------

int main ()
{
    List Lst1 = { 0 };

    list_ctor (&Lst1, 15);

    list_push_head (&Lst1, 300);
    list_push_head (&Lst1, 200);
    list_push_head (&Lst1, 100);
    list_push_tail (&Lst1, 400);
    list_push_tail (&Lst1, 500);
    list_push_right (&Lst1, 600, 5);

    list_pop (&Lst1, 5);
    list_pop (&Lst1, 4);

    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_head (&Lst1, 656);
    list_push_tail (&Lst1, 222);

    list_pop (&Lst1, 18);
    list_pop (&Lst1, 17);
    list_pop (&Lst1, 16);
    list_pop (&Lst1, 15);
    list_pop (&Lst1, 14);
    list_pop (&Lst1, 13);
    list_pop (&Lst1, 12);
    list_pop (&Lst1, 11);
    list_pop (&Lst1, 10);
    list_pop (&Lst1, 9);
    list_pop (&Lst1, 8);
    list_pop (&Lst1, 7);

    debug_list (&Lst1);

    list_dtor (&Lst1);

    return 0;
}

//-----------------------------------------------------------------------------

