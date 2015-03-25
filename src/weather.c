#include <pebble.h>

#define CELSIUS 0
#define FAHRENHEIT 1

void update_weather_conditions_display(uint32_t weather_state);
void update_weather_temperature_display();
void update_weather();

uint32_t s_weather_state = RESOURCE_ID_IMAGE_ALERT;
static BitmapLayer *s_conditions_layer;
static TextLayer *s_temperature_layer;
static GBitmap *s_conditions_bitmap;
int s_temperature = 0;
uint32_t s_temperature_unit = CELSIUS;
int s_temperature_showing = 0;
int s_weather_showing = 0;

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

  update_weather();
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

void update_weather_temperature_display() {
  static char temperature_buffer[8];

  if (s_temperature_unit == CELSIUS) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", s_temperature);
  } else {
    int temperature = (s_temperature * 9 / 5) + 32;
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°F", temperature);
  }
  text_layer_set_text(s_temperature_layer, temperature_buffer);
}

void update_weather() {
  if (s_temperature_showing == 0) {
    layer_set_hidden(text_layer_get_layer(s_temperature_layer), false);
  } else {
    layer_set_hidden(text_layer_get_layer(s_temperature_layer), true);
  }
  if (s_weather_showing == 0) {
    layer_set_hidden(bitmap_layer_get_layer(s_conditions_layer), false);
  } else {
    layer_set_hidden(bitmap_layer_get_layer(s_conditions_layer), true);
  }
}
