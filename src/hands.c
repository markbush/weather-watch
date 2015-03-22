#include <pebble.h>

extern void update_time(struct tm *tick_time);
extern struct tm *pebble_time;

static void hands_update_proc(Layer *layer, GContext *ctx);

Layer *s_hands_layer;

static GPath *s_minute_hand_path;
static const GPathInfo MINUTE_HAND = {
    3, (GPoint []){
        {-2, -55},
        {0, -64},
        {2, -55}
    }
};
static GPath *s_hour_hand_path;
static const GPathInfo HOUR_HAND = {
    3, (GPoint []){
        {-6, -55},
        {0, -62},
        {6, -55}
    }
};

void setup_hands(Layer *root) {
  s_hands_layer = layer_create(GRect(0, 0, 144, 168));
  layer_add_child(root, s_hands_layer);

  s_minute_hand_path = gpath_create(&MINUTE_HAND);
  gpath_move_to(s_minute_hand_path, GPoint(72, 84));
  s_hour_hand_path = gpath_create(&HOUR_HAND);
  gpath_move_to(s_hour_hand_path, GPoint(72, 84));

  layer_set_update_proc(s_hands_layer, hands_update_proc);

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
  layer_destroy(s_hands_layer);
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  if (!pebble_time) return;

  int32_t minute_angle = TRIG_MAX_ANGLE * pebble_time->tm_min / 60;
  int32_t hour_angle = (TRIG_MAX_ANGLE * (((pebble_time->tm_hour % 12) * 6) + (pebble_time->tm_min / 10))) / (12 * 6);

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);

  gpath_rotate_to(s_minute_hand_path, minute_angle);
  gpath_draw_outline(ctx, s_minute_hand_path);
  gpath_draw_filled(ctx, s_minute_hand_path);

  gpath_rotate_to(s_hour_hand_path, hour_angle);
  gpath_draw_outline(ctx, s_hour_hand_path);
  gpath_draw_filled(ctx, s_hour_hand_path);
}

