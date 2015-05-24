#include <pebble.h>

#define CELSIUS 0
#define FAHRENHEIT 1

void update_weather_conditions_display(uint32_t weather_state);
void update_weather_temperature_display();
void update_weather();

uint32_t s_weather_state = RESOURCE_ID_IMAGE_QUERY;
static BitmapLayer *s_conditions_layer;
static TextLayer *s_temperature_layer;
static GBitmap *s_conditions_bitmap;
int s_temperature = 0;
uint32_t s_temperature_unit = CELSIUS;
int s_temperature_showing = 0;
int s_weather_showing = 0;

void setup_weather(Layer *root) {
  // Create temperature Layer
#ifdef PBL_COLOR
  s_temperature_layer = text_layer_create(GRect(2, 0, 26, 10));
  text_layer_set_font(s_temperature_layer, fonts_get_system_font(FONT_KEY_GOTHIC_09));
  text_layer_set_text_color(s_temperature_layer, GColorBlack);
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentRight);
#else
  s_temperature_layer = text_layer_create(GRect(0, 30, 144, 28));
  text_layer_set_font(s_temperature_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(s_temperature_layer, GColorWhite);
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentCenter);
#endif
  text_layer_set_background_color(s_temperature_layer, GColorClear);
  layer_add_child(root, text_layer_get_layer(s_temperature_layer));

#ifdef PBL_COLOR
#else
  // Create conditions Layer
  s_conditions_layer = bitmap_layer_create(GRect(56, 100, 32, 32));
  s_conditions_bitmap = gbitmap_create_with_resource(s_weather_state);
  bitmap_layer_set_bitmap(s_conditions_layer, s_conditions_bitmap);
  layer_add_child(root, bitmap_layer_get_layer(s_conditions_layer));
#endif

  update_weather();
}

void teardown_weather() {
  text_layer_destroy(s_temperature_layer);
#ifdef PBL_COLOR
#else
  gbitmap_destroy(s_conditions_bitmap);
  bitmap_layer_destroy(s_conditions_layer);
#endif
}

void update_weather_conditions_display(uint32_t weather_state) {
  static int failures = 0;

  if (weather_state == RESOURCE_ID_IMAGE_ALERT
      || weather_state == RESOURCE_ID_IMAGE_LOCATION
      || weather_state == RESOURCE_ID_IMAGE_QUERY) {
    if (failures < 3) {
      failures += 1;
      // Ignore up to 3 failures
      weather_state = s_weather_state;
    }
  } else {
    failures = 0;
  }

  if (weather_state != s_weather_state) {
    s_weather_state = weather_state;
#ifdef PBL_COLOR
#else
    if (s_conditions_bitmap) {
      gbitmap_destroy(s_conditions_bitmap);
    }
    s_conditions_bitmap = gbitmap_create_with_resource(s_weather_state);
    bitmap_layer_set_bitmap(s_conditions_layer, s_conditions_bitmap);
#endif
  }
}

void update_weather_temperature_display() {
  static char temperature_buffer[8];

  if (s_weather_state == RESOURCE_ID_IMAGE_ALERT
      || s_weather_state == RESOURCE_ID_IMAGE_LOCATION
      || s_weather_state == RESOURCE_ID_IMAGE_QUERY) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "?");
  } else {
    if (s_temperature_unit == CELSIUS) {
#ifdef PBL_COLOR
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%d C", s_temperature);
#else
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", s_temperature);
#endif
    } else {
      int temperature = (s_temperature * 9 / 5) + 32;
#ifdef PBL_COLOR
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%d F", temperature);
#else
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°F", temperature);
#endif
    }
  }
  text_layer_set_text(s_temperature_layer, temperature_buffer);
}

void update_weather() {
#ifdef PBL_COLOR
#else
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
#endif
}
