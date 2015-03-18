#include <pebble.h>

#define BLUETOOTH_CONNECTED 1
#define BLUETOOTH_DISCONNECTED 0
#define BLUETOOTH_UNKNOWN 255

static void bluetooth_update_proc(Layer *layer, GContext *ctx);
static void bluetooth_changed_callback(bool connected);

static Layer *s_bluetooth_layer;
uint8_t s_bluetooth_state = BLUETOOTH_UNKNOWN;

static GPath *s_bluetooth_path;
static const GPathInfo BLUETOOTH_ICON = {
    6, (GPoint []){
        {-1, 2},
        {6, 9},
        {3, 12},
        {3, 0},
        {6, 3},
        {-1, 10}
    }
};

void setup_bluetooth(Layer *root) {
  s_bluetooth_layer = layer_create(GRect(4, 4, 144, 12));
  s_bluetooth_path = gpath_create(&BLUETOOTH_ICON);
  layer_set_update_proc(s_bluetooth_layer, bluetooth_update_proc);
  layer_add_child(root, s_bluetooth_layer);
  bluetooth_connection_service_subscribe(&bluetooth_changed_callback);
}

void teardown_bluetooth() {
  if (s_bluetooth_path) {
    gpath_destroy(s_bluetooth_path);
    s_bluetooth_path = NULL;
  }
  layer_destroy(s_bluetooth_layer);
}

static void bluetooth_update_proc(Layer *layer, GContext *ctx) {
  if (s_bluetooth_state == BLUETOOTH_UNKNOWN) {
    s_bluetooth_state = bluetooth_connection_service_peek();
  }

  if (s_bluetooth_state == BLUETOOTH_CONNECTED) {
    graphics_context_set_stroke_color(ctx, GColorWhite);
  } else {
    graphics_context_set_stroke_color(ctx, GColorBlack);
  }
  gpath_draw_outline(ctx, s_bluetooth_path);
}

static void bluetooth_changed_callback(bool connected) {
  uint8_t new_bluetooth_state = connected ? BLUETOOTH_CONNECTED : BLUETOOTH_DISCONNECTED;
  if (new_bluetooth_state != s_bluetooth_state) {
    s_bluetooth_state = new_bluetooth_state;
    layer_mark_dirty(s_bluetooth_layer);
  }
}
