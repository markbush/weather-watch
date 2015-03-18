#include <pebble.h>

#define BATTERY_CHARGING 253
#define BATTERY_CHARGED  254
#define BATTERY_UNKNOWN  255

static void battery_update_proc(Layer *layer, GContext *ctx);
static void battery_changed_callback(BatteryChargeState charge_state);

static Layer *s_battery_layer;
static uint8_t s_battery_state = BATTERY_UNKNOWN;

static GPath *s_battery_path;
static const GPathInfo BATTERY_ICON = {
    10, (GPoint []){
        {0, 2},
        {15, 2},
        {15, 7},
        {16, 7},
        {16, 4},
        {17, 4},
        {17, 8},
        {15, 8},
        {15, 10},
        {0, 10}
    }
};
static GPath *s_charge_path;
static const GPathInfo CHARGE_ICON = {
    9, (GPoint []){
        {10, 0},
        {9, 5},
        {11, 6},
        {5, 12},
        {5, 11},
        {6, 6},
        {4, 6},
        {4, 5},
        {9, 0}
    }
};

void setup_battery(Layer *root) {
  s_battery_layer = layer_create(GRect(123, 2, 19, 15));
  s_battery_path = gpath_create(&BATTERY_ICON);
  s_charge_path = gpath_create(&CHARGE_ICON);
  layer_set_update_proc(s_battery_layer, battery_update_proc);
  layer_add_child(root, s_battery_layer);
  battery_changed_callback(battery_state_service_peek());
  battery_state_service_subscribe(&battery_changed_callback);
}

void teardown_battery() {
  if (s_battery_path) {
    gpath_destroy(s_battery_path);
    s_battery_path = NULL;
  }
  if (s_charge_path) {
    gpath_destroy(s_charge_path);
    s_charge_path = NULL;
  }
  layer_destroy(s_battery_layer);
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorWhite);
  gpath_draw_outline(ctx, s_battery_path);

  GRect battery_fill = GRect(2, 4, 12, 5);
  switch (s_battery_state) {
    case BATTERY_CHARGING:
    case BATTERY_CHARGED:
      graphics_context_set_fill_color(ctx, GColorWhite);
      break;

    case BATTERY_UNKNOWN:
      graphics_context_set_fill_color(ctx, GColorBlack);
      break;

    default:
      graphics_context_set_fill_color(ctx, GColorWhite);
      battery_fill.size.w = ((int)(battery_fill.size.w) * (int)s_battery_state)/100;
  }
  graphics_fill_rect(ctx, battery_fill, 0, 0);

  if (s_battery_state == BATTERY_CHARGING ||
      s_battery_state == BATTERY_CHARGED) {
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_fill_color(ctx, GColorWhite);
    gpath_draw_filled(ctx, s_charge_path);
    gpath_draw_outline(ctx, s_charge_path);
  }
}

static void battery_changed_callback(BatteryChargeState charge_state) {
  uint8_t battery_state = BATTERY_UNKNOWN;
  if (charge_state.is_charging) {
    battery_state = BATTERY_CHARGING;
  } else if (charge_state.is_plugged) {
    battery_state = BATTERY_CHARGED;
  } else if (charge_state.charge_percent >= 90) {
    battery_state = 100;
  } else {
    battery_state = charge_state.charge_percent;
  }

  if (battery_state != s_battery_state) {
    s_battery_state = battery_state;
    layer_mark_dirty(s_battery_layer);
  }
}
