#include <pebble.h>

void update_date_display(struct tm *pebble_time);

static TextLayer *s_date_layer;

void setup_date(Layer *root) {
  s_date_layer = text_layer_create(GRect(0, 58, 144, 28));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_date_layer));
}

void teardown_date() {
  text_layer_destroy(s_date_layer);
}

void update_date_display(struct tm *pebble_time) {
  // Create a long-lived buffer
  static char day_date_buffer[] = "Mon 31 Mar";

  strftime(day_date_buffer, sizeof("Mon 31 Mar"), "%a %e %b", pebble_time);
  text_layer_set_text(s_date_layer, day_date_buffer);
}