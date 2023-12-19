#include "customcontainer.h"
#include "item.h"

// Too lazy to declare that in headers like sane people.
extern struct Item *widgets[2];
extern int focused;

G_DEFINE_TYPE(TestCustomContainerAccessible, test_custom_container_accessible, GTK_TYPE_WIDGET_ACCESSIBLE)

static void accessible_initialize(AtkObject *obj, gpointer data);
static gint get_n_children(AtkObject *self);
static AtkObject *ref_child(AtkObject *self, gint child);
static void state_change(AtkObject *self, const gchar *state, gboolean res, gpointer data);
static AtkStateSet *ref_state_set(AtkObject *self);

// Standard gobject initialization routines.
void test_custom_container_accessible_class_init(TestCustomContainerAccessibleClass *cls)
{
    AtkObjectClass *a = ATK_OBJECT_CLASS(cls);
    a->initialize = accessible_initialize;
    a->get_n_children = get_n_children;
    a->ref_child = ref_child;

    // Suppress focus events for that widget even though it's focusable, explanation below.
    a->focus_event = NULL;
    a->ref_state_set = ref_state_set;
}

void test_custom_container_accessible_init(TestCustomContainerAccessible *self)
{
}

// Post initialization, mainly setting role of the object as visible to screenreader.
void accessible_initialize(AtkObject *obj, gpointer data)
{
    ATK_OBJECT_CLASS(test_custom_container_accessible_parent_class)->initialize(obj, data);
    atk_object_set_role(obj, ATK_ROLE_ROOT_PANE);
    g_signal_connect(G_OBJECT(obj), "state-change::focused", G_CALLBACK(state_change), NULL);
}

// Indicates two children, even though the peer widget has none from gtk perspective.
gint get_n_children(AtkObject *self)
{
    return 2;
}

// Gives out accessible objects for child buttons.
AtkObject *ref_child(AtkObject *self, gint child)
{
    if (child < 0 || child > 1)
    {
        return NULL;
    }

    AtkObject *o = item_get_accessible(widgets[child]);

    if (o)
    {
        g_object_ref(G_OBJECT(o));
    }
    return o;
}

// This method overrides GtkWidgetAccessible's method, but
// eats everything related to focus.
// It's because actual focus is on buttons and we made container focusable just to make it accept
// keyboard events, but screenreader shouldn't see this.
AtkStateSet *ref_state_set(AtkObject *self)
{
    AtkStateSet *s = ATK_OBJECT_CLASS(test_custom_container_accessible_parent_class)->ref_state_set(self);
    atk_state_set_remove_state(s, ATK_STATE_FOCUSED);
    atk_state_set_remove_state(s, ATK_STATE_FOCUSABLE);
    return s;
}

// For the same reason as above, suppress focus change events coming.
void state_change(AtkObject *self, const gchar *state, gboolean res, gpointer data)
{
    g_signal_stop_emission_by_name(self, "state-change::focused");
}

G_DEFINE_TYPE(TestCustomContainer, test_custom_container, GTK_TYPE_DRAWING_AREA)

static gboolean focus_in(GtkWidget *widget, GdkEventFocus *event);
static gboolean focus_out(GtkWidget *widget, GdkEventFocus *event);

void test_custom_container_class_init(TestCustomContainerClass *cls)
{
    gtk_widget_class_set_accessible_type(GTK_WIDGET_CLASS(cls), TEST_TYPE_CUSTOM_CONTAINER_ACCESSIBLE);
    GTK_WIDGET_CLASS(cls)->focus_in_event = focus_in;
    GTK_WIDGET_CLASS(cls)->focus_out_event = focus_out;
}

void test_custom_container_init(TestCustomContainer *self)
{
}

// Handle focus coming to canvas.
// Focus is permanently grabbed but it can be lost or regained when switching windows.
gboolean focus_in(GtkWidget *widget, GdkEventFocus *event)
{
    GTK_WIDGET_CLASS(test_custom_container_parent_class)->focus_in_event(widget, event);
    item_focus(widgets[focused]);
    return FALSE;
}

// Handle focus leaving parent canvas, usually by switching windows.
gboolean focus_out(GtkWidget *widget, GdkEventFocus *event)
{
    GTK_WIDGET_CLASS(test_custom_container_parent_class)->focus_out_event(widget, event);
    item_release_focus(widgets[focused]);
    return FALSE;
}

// Constructor.
GtkWidget *test_custom_container_new(void)
{
    return GTK_WIDGET(g_object_new(TEST_TYPE_CUSTOM_CONTAINER, NULL));
}