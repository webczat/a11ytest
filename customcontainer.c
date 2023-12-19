#include "customcontainer.h"
#include "item.h"

extern struct Item *widgets[2];
extern int focused;

G_DEFINE_TYPE(TestCustomContainerAccessible, test_custom_container_accessible, GTK_TYPE_WIDGET_ACCESSIBLE)

static void accessible_initialize(AtkObject *obj, gpointer data);
static gint get_n_children(AtkObject *self);
static AtkObject *ref_child(AtkObject *self, gint child);
static void state_change(AtkObject *self, const gchar *state, gboolean res, gpointer data);
static AtkStateSet *ref_state_set(AtkObject *self);

void test_custom_container_accessible_class_init(TestCustomContainerAccessibleClass *cls)
{
    AtkObjectClass *a = ATK_OBJECT_CLASS(cls);
    a->initialize = accessible_initialize;
    a->get_n_children = get_n_children;
    a->ref_child = ref_child;
    a->focus_event = NULL;
    a->ref_state_set = ref_state_set;
}

void test_custom_container_accessible_init(TestCustomContainerAccessible *self)
{
}

void accessible_initialize(AtkObject *obj, gpointer data)
{
    ATK_OBJECT_CLASS(test_custom_container_accessible_parent_class)->initialize(obj, data);
    atk_object_set_role(obj, ATK_ROLE_ROOT_PANE);
    g_signal_connect(G_OBJECT(obj), "state-change::focused", G_CALLBACK(state_change), NULL);
}

gint get_n_children(AtkObject *self)
{
    return 2;
}

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

AtkStateSet *ref_state_set(AtkObject *self)
{
    AtkStateSet *s = ATK_OBJECT_CLASS(test_custom_container_accessible_parent_class)->ref_state_set(self);
    atk_state_set_remove_state(s, ATK_STATE_FOCUSED);
    atk_state_set_remove_state(s, ATK_STATE_FOCUSABLE);
    return s;
}

void state_change(AtkObject *self, const gchar *state, gboolean res, gpointer data)
{
    printf("boo\n");
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

gboolean focus_in(GtkWidget *widget, GdkEventFocus *event)
{
    GTK_WIDGET_CLASS(test_custom_container_parent_class)->focus_in_event(widget, event);
    item_focus(widgets[focused]);
    return FALSE;
}

gboolean focus_out(GtkWidget *widget, GdkEventFocus *event)
{
    GTK_WIDGET_CLASS(test_custom_container_parent_class)->focus_out_event(widget, event);
    item_release_focus(widgets[focused]);
    return FALSE;
}

GtkWidget *test_custom_container_new(void)
{
    return GTK_WIDGET(g_object_new(TEST_TYPE_CUSTOM_CONTAINER, NULL));
}