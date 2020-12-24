//
//  Generated file. Do not edit.
//

#include "generated_plugin_registrant.h"

#include <drag_area/drag_area_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) drag_area_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "DragAreaPlugin");
  drag_area_plugin_register_with_registrar(drag_area_registrar);
}
