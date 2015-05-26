#include <pebble.h>

extern void update_time(struct tm *tick_time);
extern struct tm g_pebble_time;

static void hands_update_proc(Layer *layer, GContext *ctx);

Layer *g_hands_layer;

static GPath *s_minute_hand_path;
static GPath *s_hour_hand_path;
#ifdef PBL_COLOR
static const GPathInfo MINUTE_HAND = {
    3, (GPoint []){
        {-2, -46},
        {0, -64},
        {2, -46}
    }
};
static const GPathInfo HOUR_HAND = {
    3, (GPoint []){
        {-6, -47},
        {0, -62},
        {6, -47}
    }
};
#else
static const GPathInfo MINUTE_HAND = {
    3, (GPoint []){
        {-2, -52},
        {0, -64},
        {2, -52}
    }
};
static const GPathInfo HOUR_HAND = {
    3, (GPoint []){
        {-6, -52},
        {0, -62},
        {6, -52}
    }
};
#endif
static GColor fg;

void setup_hands(Layer *root) {
#ifdef PBL_COLOR
  g_hands_layer = layer_create(GRect(0, 0, 144, 144));
  fg = GColorRed;
#else
  g_hands_layer = layer_create(GRect(0, 0, 144, 168));
  fg = GColorWhite;
#endif
  layer_add_child(root, g_hands_layer);

  s_minute_hand_path = gpath_create(&MINUTE_HAND);
  s_hour_hand_path = gpath_create(&HOUR_HAND);
#ifdef PBL_COLOR
  gpath_move_to(s_minute_hand_path, GPoint(72, 72));
  gpath_move_to(s_hour_hand_path, GPoint(72, 72));
#else
  gpath_move_to(s_minute_hand_path, GPoint(72, 84));
  gpath_move_to(s_hour_hand_path, GPoint(72, 84));
#endif

  layer_set_update_proc(g_hands_layer, hands_update_proc);

  // Initialise clock
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  update_time(tick_time);
}

void teardown_hands() {
  if (s_minute_hand_path) {
    gpath_destroy(s_minute_hand_path);
    s_minute_hand_path = NULL;
  }
  if (s_hour_hand_path) {
    gpath_destroy(s_hour_hand_path);
    s_hour_hand_path = NULL;
  }
  layer_destroy(g_hands_layer);
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  int32_t minute_angle = TRIG_MAX_ANGLE * g_pebble_time.tm_min / 60;
  int32_t hour_angle = (TRIG_MAX_ANGLE * (((g_pebble_time.tm_hour % 12) * 6) + (g_pebble_time.tm_min / 10))) / (12 * 6);

  graphics_context_set_fill_color(ctx, fg);
  graphics_context_set_stroke_color(ctx, fg);

  gpath_rotate_to(s_minute_hand_path, minute_angle);
  gpath_draw_outline(ctx, s_minute_hand_path);
  gpath_draw_filled(ctx, s_minute_hand_path);

  gpath_rotate_to(s_hour_hand_path, hour_angle);
  gpath_draw_outline(ctx, s_hour_hand_path);
  gpath_draw_filled(ctx, s_hour_hand_path);
}
