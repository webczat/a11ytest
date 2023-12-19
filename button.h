#ifndef BUTTON_H
#define BUTTON_H
#include "item.h"
#include <atk/atk.h>

// A button item (completely unrelated to gtk), has a click event.
struct Button
{
    struct Item item;
    char *label;
    void (*click)(struct Button *b);
};

struct Button *button_new(void *parent, char *label);
void button_on_click(struct Button *b, void (*handler)(struct Button *));

// Button's associated accessibility object deriving from AtkObbject, which can expose it to a screenreader.
#define TEST_TYPE_BUTTON_ACCESSIBLE test_button_accessible_get_type()
G_DECLARE_DERIVABLE_TYPE(TestButtonAccessible, test_button_accessible, TEST, BUTTON_ACCESSIBLE, AtkObject)

struct _TestButtonAccessibleClass
{
    AtkObjectClass parent;
};
#endif