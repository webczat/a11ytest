#ifndef CUSTOMCONTAINER_H
#define CUSTOMCONTAINER_H
#include <gtk/gtk.h>
#include <gtk/gtk-a11y.h>

// Type deriving from GtkWidgetAccessible which is a peer object to the custom container widget, which provides accessibility information.
#define TEST_TYPE_CUSTOM_CONTAINER_ACCESSIBLE test_custom_container_accessible_get_type()
G_DECLARE_DERIVABLE_TYPE(TestCustomContainerAccessible, test_custom_container_accessible, TEST, CUSTOM_CONTAINER_ACCESSIBLE, GtkWidgetAccessible)

struct _TestCustomContainerAccessibleClass
{
    GtkWidgetAccessibleClass accessibleClass;
};

// Custom container is a gtk drawing area which is modified to pretend being a widget container for non gtk widgets.
#define TEST_TYPE_CUSTOM_CONTAINER test_custom_container_get_type()
G_DECLARE_DERIVABLE_TYPE(TestCustomContainer, test_custom_container, TEST, CUSTOM_CONTAINER, GtkDrawingArea)

struct _TestCustomContainerClass
{
    GtkDrawingAreaClass drawingAreaClass;
};

GtkWidget *test_custom_container_new(void);
#endif