#ifndef CUSTOMCONTAINER_H
#define CUSTOMCONTAINER_H
#include <gtk/gtk.h>
#include <gtk/gtk-a11y.h>

#define TEST_TYPE_CUSTOM_CONTAINER_ACCESSIBLE test_custom_container_accessible_get_type()
G_DECLARE_DERIVABLE_TYPE(TestCustomContainerAccessible, test_custom_container_accessible, TEST, CUSTOM_CONTAINER_ACCESSIBLE, GtkWidgetAccessible)

struct _TestCustomContainerAccessibleClass
{
    GtkWidgetAccessibleClass accessibleClass;
};

#define TEST_TYPE_CUSTOM_CONTAINER test_custom_container_get_type()
G_DECLARE_DERIVABLE_TYPE(TestCustomContainer, test_custom_container, TEST, CUSTOM_CONTAINER, GtkDrawingArea)

struct _TestCustomContainerClass
{
    GtkDrawingAreaClass drawingAreaClass;
};

GtkWidget *test_custom_container_new(void);
#endif