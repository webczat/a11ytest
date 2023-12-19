#include <gtk/gtk.h>
#include <stdio.h>
#include "button.h"
#include "customcontainer.h"

struct Item *widgets[2];
int focused = -1;

static void focus(int index);
static gboolean keydown(GtkWidget *widget, GdkEvent *event, gpointer userdata);
static void quit_clicked(struct Button *b);
static void test_clicked(struct Button *b);
static gboolean focus_signal(GtkWidget *widget, GtkDirectionType direction, gpointer data);

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    GtkWindow *win = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_title(win, "accessibility test");
    GtkWidget *container = test_custom_container_new();
    gtk_container_add(GTK_CONTAINER(win), container);
    widgets[0] = (struct Item *)button_new(container, "quit");
    widgets[1] = (struct Item *)button_new(container, "test");
    button_on_click((struct Button *)widgets[0], quit_clicked);
    button_on_click((struct Button *)widgets[1], test_clicked);
    gtk_widget_add_events(container, GDK_KEY_PRESS_MASK);
    gtk_widget_set_can_focus(container, TRUE);
    gtk_widget_grab_focus(container);
    g_signal_connect(G_OBJECT(container), "focus", G_CALLBACK(focus_signal), NULL);
    g_signal_connect(G_OBJECT(container), "key-press-event", G_CALLBACK(keydown), NULL);
    gtk_widget_show_all(GTK_WIDGET(win));
    focus(0);
    gtk_main();
}

gboolean focus_signal(GtkWidget *widget, GtkDirectionType direction, gpointer data)
{
    if (direction == GTK_DIR_TAB_FORWARD || direction == GTK_DIR_TAB_BACKWARD)
    {
        int f = focused == 0 ? 1 : 0;
        focus(f);
    }
    return TRUE;
}

static void focus(int index)
{
    if (index < 0 || index > 1)
    {
        index = 0;
    }
    if (focused != -1)
    {
        item_release_focus(widgets[focused]);
    }
    focused = index;
    item_focus(widgets[focused]);
}

gboolean keydown(GtkWidget *widget, GdkEvent *event, gpointer userdata)
{
    return widgets[focused]->key_down(widgets[focused], event);
}

void quit_clicked(struct Button *b)
{
    gtk_main_quit();
}

void test_clicked(struct Button *b)
{
    printf("Test button clicked.\n");
}
