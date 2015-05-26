#include <pebble.h>

#define BATTERY_CHARGING 253
#define BATTERY_CHARGED  254
#define BATTERY_UNKNOWN  255

#define SHOW_ALWAYS 0
#define SHOW_IF_NOT_GREATER_90 1
#define SHOW_IF_NOT_GREATER_80 2
#define SHOW_IF_NOT_GREATER_70 3
#define SHOW_IF_NOT_GREATER_60 4
#define SHOW_IF_NOT_GREATER_50 5
#define SHOW_IF_NOT_GREATER_40 6
#define SHOW_IF_NOT_GREATER_30 7
#define SHOW_IF_NOT_GREATER_20 8
#define SHOW_IF_NOT_GREATER_10 9
#define SHOW_CHARGING 10
#define SHOW_NEVER 11

static void battery_update_proc(Layer *layer, GContext *ctx);
static void battery_changed_callback(BatteryChargeState charge_state);
void update_battery();

static Layer *s_battery_layer;
static int s_battery_state = BATTERY_UNKNOWN;
int g_battery_showing = SHOW_ALWAYS;

static GPath *s_battery_path;
static const GPathInfo BATTERY_ICON = {
    8, (GPoint []){
        {0, 2},
        {13, 2},
        {13, 6},
        {14, 6},
        {14, 5},
        {13, 5},
        {13, 9},
        {0, 9}
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
static GColor fg, bg, warn;

void setup_battery(Layer *root) {
#ifdef PBL_COLOR
  fg = GColorBlack;
  bg = GColorYellow;
  warn = GColorRed;
#else
  fg = GColorWhite;
  bg = GColorBlack;
  warn = bg;
#endif
  s_battery_layer = layer_create(GRect(121, 2, 19, 15));
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
  graphics_context_set_stroke_color(ctx, fg);

  GRect battery_fill = GRect(2, 4, 10, 4);
  switch (s_battery_state) {
    case BATTERY_CHARGING:
    case BATTERY_CHARGED:
      graphics_context_set_fill_color(ctx, fg);
      break;

    case BATTERY_UNKNOWN:
      graphics_context_set_fill_color(ctx, warn);
      break;

    default:
      graphics_context_set_fill_color(ctx, fg);
      battery_fill.size.w = ((int)(battery_fill.size.w) * (int)(s_battery_state))/100;
      if (battery_fill.size.w < 1) {
        // basalt sets state to 0 when it gets down to 10!
        battery_fill.size.w = 1;
      }
#ifdef PBL_COLOR
      if (s_battery_state <= 20) {
        graphics_context_set_fill_color(ctx, warn);
        graphics_context_set_stroke_color(ctx, warn);
      }
#endif
  }
  gpath_draw_outline(ctx, s_battery_path);
  graphics_fill_rect(ctx, battery_fill, 0, 0);

  if (s_battery_state == BATTERY_CHARGING ||
      s_battery_state == BATTERY_CHARGED) {
    graphics_context_set_stroke_color(ctx, bg);
    graphics_context_set_fill_color(ctx, fg);
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
  } else if (charge_state.charge_percent > 90) {
    battery_state = 100;
  } else {
    battery_state = (int)charge_state.charge_percent;
  }

  if (battery_state != s_battery_state) {
    s_battery_state = battery_state;
    update_battery();
    layer_mark_dirty(s_battery_layer);
  }
}

void update_battery() {
  bool hidden = false;
  switch (g_battery_showing) {
  case SHOW_NEVER:
    ;
    hidden = true;
    break;
  case SHOW_CHARGING:
    ;
    if (s_battery_state == BATTERY_CHARGING || s_battery_state == BATTERY_CHARGED) {
      hidden = false;
    } else {
      hidden = true;
    }
    break;
  case SHOW_ALWAYS:
    ;
    hidden = false;
    break;
  default:
    ;
    int check_level = (10 - g_battery_showing) * 10;
    if (s_battery_state <= check_level) {
      hidden = false;
    } else {
      hidden = true;
    }
    break;
  }
  layer_set_hidden(s_battery_layer, hidden);
}
