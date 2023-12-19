#include "item.h"
#include <stddef.h>

AtkObject *item_get_accessible(struct Item *i)
{
    return i->accessible;
}

void item_focus(struct Item *i)
{
    if (i->focus != NULL)
    {
        i->focus(i);
    }
}

void item_release_focus(struct Item *i)
{
    if (i->release_focus != NULL)
    {
        i->release_focus(i);
    }
}