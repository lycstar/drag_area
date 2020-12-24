#include "include/drag_area/drag_area_plugin.h"
#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
gint kButton;
gint kRootX;
gint kRootY;
guint32 kTime;
const char kChannelName[] = "flutter/drag_area";
const char kDragWindowMethodMethod[] = "dragWindow";
struct _FlDragAreaPlugin {
	GObject parent_instance;
	FlPluginRegistrar* registrar;
	// Connection to Flutter engine.
	FlMethodChannel* channel;
	// Requested window geometry.
	GdkGeometry window_geometry;
}
;
G_DEFINE_TYPE(FlDragAreaPlugin, fl_drag_area_plugin, g_object_get_type())
// Gets the window being controlled.
GtkWindow* get_window(FlDragAreaPlugin* self) {
	FlView* view = fl_plugin_registrar_get_view(self->registrar);
	if (view == nullptr) return nullptr;
	return GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(view)));
}
gboolean window_drag(GtkWidget *widget, GdkEventButton *event, GdkWindowEdge edge) {
	kButton=event->button;
	kRootX=event->x_root;
	kRootY=event->y_root;
	kTime=event->time;
	return FALSE;
}
static FlMethodResponse* drag_window(FlDragAreaPlugin* self) {
	GtkWindow* window= get_window(self);
	if(kButton == 1) {
		gtk_window_begin_move_drag(window, kButton, kRootX, kRootY, kTime);
	}
	return FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
}
// Called when a method call is received from Flutter.
static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
	FlDragAreaPlugin* self = FL_DRAG_AREA_PLUGIN(user_data);
	const gchar* method = fl_method_call_get_name(method_call);
	g_autoptr(FlMethodResponse) response = nullptr;
	if (strcmp(method, kDragWindowMethodMethod) == 0) {
		response = drag_window(self);
	} else {
		response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
	}
	g_autoptr(GError) error = nullptr;
	if (!fl_method_call_respond(method_call, response, &error))
			    g_warning("Failed to send method call response: %s", error->message);
}
static void fl_drag_area_plugin_dispose(GObject* object) {
	FlDragAreaPlugin* self = FL_DRAG_AREA_PLUGIN(object);
	g_clear_object(&self->registrar);
	g_clear_object(&self->channel);
	G_OBJECT_CLASS(fl_drag_area_plugin_parent_class)->dispose(object);
}
static void fl_drag_area_plugin_class_init(FlDragAreaPluginClass* klass) {
	G_OBJECT_CLASS(klass)->dispose = fl_drag_area_plugin_dispose;
}
static void fl_drag_area_plugin_init(FlDragAreaPlugin* self) {
	self->window_geometry.min_width = -1;
	self->window_geometry.min_height = -1;
	self->window_geometry.max_width = -1;
	self->window_geometry.max_height = -1;
}
FlDragAreaPlugin* fl_drag_area_plugin_new(FlPluginRegistrar* registrar) {
	FlDragAreaPlugin* self = FL_DRAG_AREA_PLUGIN(
			      g_object_new(fl_drag_area_plugin_get_type(), nullptr));
	self->registrar = FL_PLUGIN_REGISTRAR(g_object_ref(registrar));
	g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
	self->channel =
			      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
			                            kChannelName, FL_METHOD_CODEC(codec));
	fl_method_channel_set_method_call_handler(self->channel, method_call_cb,
			                                            g_object_ref(self), g_object_unref);
	FlView* view = fl_plugin_registrar_get_view(self->registrar);
	if (view != nullptr) {
		g_signal_connect(GTK_WIDGET(view), "button-press-event", G_CALLBACK(window_drag), NULL);
	}
	return self;
}
void drag_area_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
	FlDragAreaPlugin* plugin = fl_drag_area_plugin_new(registrar);
	g_object_unref(plugin);
}