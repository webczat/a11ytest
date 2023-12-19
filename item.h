#ifndef ITEM_H
#define ITEM_H
#include <atk/atk.h>
#include <gdk/gdk.h>

// I pretend to have a widget class hierarchy, that's the base.
// Base handles focus in/out, keys and such like.
// It also has relation to an accessibility object.
struct Item
{
    void (*focus)(struct Item *);
    void (*release_focus)(struct Item *);
    gboolean (*key_down)(struct Item *, GdkEvent *event);
    AtkObject *accessible;
    gboolean visible;
    gboolean focused;
};

void item_focus(struct Item *i);
void item_release_focus(struct Item *i);
AtkObject *item_get_accessible(struct Item *i);
#endif