#include <pebble.h>

#define BLUETOOTH_CONNECTED 1
#define BLUETOOTH_DISCONNECTED 0
#define BLUETOOTH_UNKNOWN 255

#define DISCONNECT_NO_ACTION 0
#define DISCONNECT_SHORT 1
#define DISCONNECT_LONG 2
#define DISCONNECT_DOUBLE 3

static void bluetooth_update_proc(Layer *layer, GContext *ctx);
static void bluetooth_changed_callback(bool connected);
void update_bluetooth();

static Layer *s_bluetooth_layer;
uint8_t s_bluetooth_state = BLUETOOTH_UNKNOWN;
int s_bluetooth_showing = 0;
int s_bluetooth_disconnect = DISCONNECT_NO_ACTION;

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
static GColor fg, bg;

void setup_bluetooth(Layer *root) {
#ifdef PBL_COLOR
  s_bluetooth_layer = layer_create(GRect(132, 17, 8, 12));
  fg = GColorBlue;
  bg = GColorYellow;
#else
  s_bluetooth_layer = layer_create(GRect(4, 4, 8, 12));
  fg = GColorWhite;
  bg = GColorBlack;
#endif
  s_bluetooth_path = gpath_create(&BLUETOOTH_ICON);
  layer_set_update_proc(s_bluetooth_layer, bluetooth_update_proc);
  layer_add_child(root, s_bluetooth_layer);
  bluetooth_changed_callback(bluetooth_connection_service_peek() == BLUETOOTH_CONNECTED);
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
    graphics_context_set_stroke_color(ctx, fg);
  } else {
    graphics_context_set_stroke_color(ctx, bg);
  }
  gpath_draw_outline(ctx, s_bluetooth_path);
}

static void bluetooth_changed_callback(bool connected) {
  uint8_t new_bluetooth_state = connected ? BLUETOOTH_CONNECTED : BLUETOOTH_DISCONNECTED;
  if (new_bluetooth_state != s_bluetooth_state) {
    s_bluetooth_state = new_bluetooth_state;
    update_bluetooth();
    layer_mark_dirty(s_bluetooth_layer);
    if (s_bluetooth_state == BLUETOOTH_DISCONNECTED && s_bluetooth_disconnect != DISCONNECT_NO_ACTION) {
      switch (s_bluetooth_disconnect) {
      case DISCONNECT_SHORT:
        vibes_short_pulse();
        break;
      case DISCONNECT_LONG:
        vibes_long_pulse();
        break;
      case DISCONNECT_DOUBLE:
        vibes_double_pulse();
        break;
      }
    }
  }
}

void update_bluetooth() {
  if (s_bluetooth_showing == 0) {
    layer_set_hidden(s_bluetooth_layer, false);
  } else {
    layer_set_hidden(s_bluetooth_layer, true);
  }
}
