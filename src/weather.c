#include <pebble.h>

void update_weather_conditions_display(uint32_t weather_state);
void update_weather_temperature_display(int temperature);

uint32_t s_weather_state = RESOURCE_ID_IMAGE_ALERT;
static BitmapLayer *s_conditions_layer;
static TextLayer *s_temperature_layer;
static GBitmap *s_conditions_bitmap;

void setup_weather(Layer *root) {
  // Create temperature Layer
  s_temperature_layer = text_layer_create(GRect(0, 30, 144, 28));
  text_layer_set_background_color(s_temperature_layer, GColorClear);
  text_layer_set_text_color(s_temperature_layer, GColorWhite);
  text_layer_set_font(s_temperature_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_temperature_layer));

  // Create conditions Layer
  s_conditions_layer = bitmap_layer_create(GRect(48, 92, 48, 48));
  s_conditions_bitmap = gbitmap_create_with_resource(s_weather_state);
  bitmap_layer_set_bitmap(s_conditions_layer, s_conditions_bitmap);
  layer_add_child(root, bitmap_layer_get_layer(s_conditions_layer));
}

void teardown_weather() {
  text_layer_destroy(s_temperature_layer);
  gbitmap_destroy(s_conditions_bitmap);
  bitmap_layer_destroy(s_conditions_layer);
}

void update_weather_conditions_display(uint32_t weather_state) {
  if (weather_state != s_weather_state) {
    if (s_conditions_bitmap) {
      gbitmap_destroy(s_conditions_bitmap);
    }
    s_weather_state = weather_state;
    s_conditions_bitmap = gbitmap_create_with_resource(s_weather_state);
    bitmap_layer_set_bitmap(s_conditions_layer, s_conditions_bitmap);
  }  
}

void update_weather_temperature_display(int temperature) {
  static char temperature_buffer[8];

  snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", temperature);
  text_layer_set_text(s_temperature_layer, temperature_buffer);
}