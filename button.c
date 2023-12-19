#include "button.h"
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>

// Lazy...
extern struct Item *widgets[2];

typedef struct
{
    // Association of accessible to real button.
    struct Button *button;
} TestButtonAccessiblePrivate;

static void component_iface_init(AtkComponentIface *self, gpointer data);
static void action_iface_init(AtkActionIface *self, gpointer data);

G_DEFINE_TYPE_WITH_CODE(TestButtonAccessible, test_button_accessible, ATK_TYPE_OBJECT, G_ADD_PRIVATE(TestButtonAccessible) G_IMPLEMENT_INTERFACE(ATK_TYPE_COMPONENT, component_iface_init) G_IMPLEMENT_INTERFACE(ATK_TYPE_ACTION, action_iface_init))

static void accessible_initialize(AtkObject *self, gpointer data);
static gint accessible_get_index_in_parent(AtkObject *self);
static AtkStateSet *accessible_ref_state_set(AtkObject *self);
static AtkLayer component_get_layer(AtkComponent *self);
static void component_get_extents(AtkComponent *component, gint *x, gint *y, gint *width, gint *height, AtkCoordType coordtype);
static gint action_get_n_actions(AtkAction *self);
static const gchar *action_get_name(AtkAction *self, gint i);
static const gchar *action_get_localized_name(AtkAction *self, gint i);
static gboolean action_do_action(AtkAction *self, gint i);

// Standard gobject initialization for accessible object.
void test_button_accessible_class_init(TestButtonAccessibleClass *cls)
{
    cls->parent.initialize = accessible_initialize;
    cls->parent.get_index_in_parent = accessible_get_index_in_parent;
    cls->parent.ref_state_set = accessible_ref_state_set;
}

void test_button_accessible_init(TestButtonAccessible *self)
{
}

// Expose basic button characteristic to screenreader, and associate with peer widget.
void accessible_initialize(AtkObject *self, gpointer data)
{
    TestButtonAccessiblePrivate *priv = test_button_accessible_get_instance_private(TEST_BUTTON_ACCESSIBLE(self));
    priv->button = (struct Button *)data;
    atk_object_set_role(self, ATK_ROLE_PUSH_BUTTON);
    atk_object_set_name(self, priv->button->label);
}

// Considering canvas has two children, it's a dumb way to tell which one we are.
gint accessible_get_index_in_parent(AtkObject *self)
{
    TestButtonAccessible *a = TEST_BUTTON_ACCESSIBLE(self);
    TestButtonAccessiblePrivate *priv = test_button_accessible_get_instance_private(a);
    if (priv->button == (struct Button *)widgets[0])
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Gives out current button states, like visible focused etc.
AtkStateSet *accessible_ref_state_set(AtkObject *self)
{
    TestButtonAccessible *a = TEST_BUTTON_ACCESSIBLE(self);
    TestButtonAccessiblePrivate *priv = test_button_accessible_get_instance_private(a);
    AtkStateSet *states = atk_state_set_new();

    // Below states are always there and don't update.
    AtkStateType sa[] = {ATK_STATE_VISIBLE, ATK_STATE_FOCUSABLE, ATK_STATE_ENABLED, ATK_STATE_SENSITIVE};
    atk_state_set_add_states(states, sa, sizeof(sa) / sizeof(AtkStateType));

    // If the item is currently visible on screen or not, we show that. Buttons cannot be hidden, but canvas can.
    if (priv->button->item.visible)
    {
        atk_state_set_add_state(states, ATK_STATE_SHOWING);
    }

    // Indicate active focus if any.
    if (priv->button->item.focused)
    {
        atk_state_set_add_state(states, ATK_STATE_FOCUSED);
    }
    return states;
}

void component_iface_init(AtkComponentIface *self, gpointer data)
{
    self->get_layer = component_get_layer;
    self->get_extents = component_get_extents;
}

AtkLayer component_get_layer(AtkComponent *self)
{
    return ATK_LAYER_WIDGET;
}

// Compute and give out the component's position and size... for the button which has no on-screen presence.
void component_get_extents(AtkComponent *self, gint *x, gint *y, gint *width, gint *height, AtkCoordType coordtype)
{
    gint local_x, local_y = 0;
    *width = 100;
    *height = 200;
    if (atk_object_get_index_in_parent(ATK_OBJECT(self)) == 0)
    {
        local_x = 0;
    }
    else
    {
        local_x = 100;
    }
    AtkObject *parent = atk_object_get_parent(ATK_OBJECT(self));
    GtkWidget *parent_widget = gtk_accessible_get_widget(GTK_ACCESSIBLE(parent));
    GtkWidget *window = gtk_widget_get_toplevel(parent_widget);
    if (coordtype == ATK_XY_PARENT)
    {
        *x = local_x;
        *y = local_y;
        return;
    }
    else if (coordtype == ATK_XY_WINDOW)
    {
        gtk_widget_translate_coordinates(parent_widget, window, local_x, local_y, x, y);
        return;
    }
    else
    {
        gtk_widget_translate_coordinates(parent_widget, window, local_x, local_y, x, y);
        gint win_x, win_y;
        gtk_window_get_position(GTK_WINDOW(window), &win_x, &win_y);
        (*x) += win_x;
        (*y) += win_y;
        return;
    }
}

void action_iface_init(AtkActionIface *self, gpointer data)
{
    self->get_n_actions = action_get_n_actions;
    self->get_localized_name = action_get_localized_name;
    self->get_name = action_get_name;
    self->do_action = action_do_action;
}

// Support for externally synthesized button click action (not a mouse click, but initiated by screenreader or ui testing possibly).
gint action_get_n_actions(AtkAction *self)
{
    return 1;
}

const gchar *action_get_name(AtkAction *self, gint i)
{
    if (i == 0)
    {
        return "click";
    }

    return NULL;
}

const gchar *action_get_localized_name(AtkAction *self, gint i)
{
    if (i == 0)
    {
        return "click";
    }

    return NULL;
}

gboolean action_do_action(AtkAction *self, gint i)
{
    if (i != 0)
    {
        return FALSE;
    }

    TestButtonAccessiblePrivate *priv = test_button_accessible_get_instance_private(TEST_BUTTON_ACCESSIBLE(self));
    struct Button *b = priv->button;
    if (b->click != NULL)
    {
        b->click(b);
    }
    return TRUE;
}

static gboolean button_keydown(struct Item *i, GdkEvent *event);
static void button_focus(struct Item *i);
static void button_release_focus(struct Item *i);
static void button_map(GtkWidget *parent, gpointer data);
static void button_unmap(GtkWidget *parent, gpointer data);

// Button constructor.
struct Button *button_new(void *parent, char *label)
{
    struct Button *b = (struct Button *)malloc(sizeof(struct Button));
    memset(b, 0, sizeof(struct Button));
    b->label = label;
    b->item.key_down = button_keydown;
    b->item.focus = button_focus;
    b->item.release_focus = button_release_focus;

    // Needed to realize when button is shown or not.
    g_signal_connect(G_OBJECT(parent), "map", G_CALLBACK(button_map), b);
    g_signal_connect(G_OBJECT(parent), "unmap", G_CALLBACK(button_unmap), b);

    // Create accessible peer, associate with canvas. Note button itself doesn't need that except signals connected.
    AtkObject *accessibleparent = ATK_OBJECT(gtk_widget_get_accessible(GTK_WIDGET(parent)));
    b->item.accessible = g_object_new(TEST_TYPE_BUTTON_ACCESSIBLE, NULL);
    TEST_BUTTON_ACCESSIBLE_GET_CLASS(b->item.accessible)->parent.initialize(b->item.accessible, b);
    atk_object_set_parent(ATK_OBJECT(b->item.accessible), accessibleparent);
    g_object_unref(G_OBJECT(accessibleparent));

    return b;
}

// On map and unmap, visibility flag is set. But because
// screenreader won't see that, an event is also sent to notify it.
void button_map(GtkWidget *parent, gpointer data)
{
    struct Button *b = (struct Button *)data;
    b->item.visible = TRUE;
    atk_object_notify_state_change(b->item.accessible, ATK_STATE_SHOWING, TRUE);
}

void button_unmap(GtkWidget *parent, gpointer data)
{
    struct Button *b = (struct Button *)data;
    b->item.visible = FALSE;
    atk_object_notify_state_change(b->item.accessible, ATK_STATE_SHOWING, FALSE);
}

// Button's key handler called only when button focused, reacts on space/enter and causes click via keyboard.
gboolean button_keydown(struct Item *i, GdkEvent *event)
{
    struct Button *b = (struct Button *)i;
    if (event->key.keyval == GDK_KEY_space || event->key.keyval == GDK_KEY_Return || event->key.keyval == GDK_KEY_KP_Enter)
    {
        if (b->click != NULL)
        {
            b->click(b);
        }
        return TRUE;
    }

    return FALSE;
}

// Called when button is focused.
void button_focus(struct Item *i)
{
    i->focused = TRUE;

    // Without explicit event notification, screenreader will be silent.
    atk_object_notify_state_change(i->accessible, ATK_STATE_FOCUSED, TRUE);
}

void button_release_focus(struct Item *i)
{
    i->focused = FALSE;
    atk_object_notify_state_change(i->accessible, ATK_STATE_FOCUSED, FALSE);
}

void button_on_click(struct Button *b, void (*handler)(struct Button *))
{
    b->click = handler;
}
