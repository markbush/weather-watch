#include <pebble.h>

#define CELSIUS 0
#define FAHRENHEIT 1

void update_weather_conditions_display(uint32_t weather_state);
void update_weather_temperature_display();
void update_weather();

uint32_t s_weather_state = RESOURCE_ID_IMAGE_QUERY;
static TextLayer *s_temperature_layer;
int s_temperature = 0;
uint32_t s_temperature_unit = CELSIUS;
int s_temperature_showing = 0;
int s_weather_showing = 0;

#ifdef PBL_COLOR
static Layer *s_temperature_scale;
static void temp_scale_update_proc(Layer *layer, GContext *ctx);
int16_t temp_width[] = {
  69, 69, 63, 56, 52, 49, 46, 42, 42, 39, 34, 34, 34, 33, 31, 29,
  28, 27, 26, 24, 24, 23, 22, 21, 20, 18, 18, 17, 16, 15, 14, 14,
  13, 13, 12,  9,  9,  9,  9,  9,  8,  8 , 8,  6,  6,  6,  6,  5,
   5,  5,  4,  4,  4,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,
   1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,
   2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  5,  5,
   5,  6,  6,  6,  6,  8,  8,  8,  9,  9,  9,  9,  9, 12, 13, 13,
  14, 14, 15, 16, 17, 18, 18, 20, 21, 22, 23, 24, 24, 26, 27, 28,
  29, 31, 33, 34, 34, 34, 39, 42, 42, 46, 49, 52, 56, 63, 69, 69
};
GColor temp_colour[13];
#else
static BitmapLayer *s_conditions_layer;
static GBitmap *s_conditions_bitmap;
#endif

void setup_weather(Layer *root) {
  // Create temperature Layers
#ifdef PBL_COLOR
  s_temperature_scale = layer_create(GRect(0, 0, 70, 144));
  layer_add_child(root, s_temperature_scale);
  layer_set_update_proc(s_temperature_scale, temp_scale_update_proc);
  temp_colour[0] = GColorWhite;
  temp_colour[1] = GColorCeleste;
  temp_colour[2] = GColorElectricBlue;
  temp_colour[3] = GColorCyan;
  temp_colour[4] = GColorVividCerulean;
  temp_colour[5] = GColorBlueMoon;
  temp_colour[6] = GColorBlue;
  temp_colour[7] = GColorElectricUltramarine;
  temp_colour[8] = GColorVividViolet;
  temp_colour[9] = GColorMagenta;
  temp_colour[10] = GColorFashionMagenta;
  temp_colour[11] = GColorFolly;
  temp_colour[12] = GColorRed;

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
  layer_destroy(s_temperature_scale);
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
#ifdef PBL_COLOR
  layer_mark_dirty(s_temperature_scale);
#endif
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

#ifdef PBL_COLOR
static void temp_scale_update_proc(Layer *layer, GContext *ctx) {
  int temp_scale = (s_temperature + 4) * 144 / 29;
  if (temp_scale < 0) {
    temp_scale = 0;
  }
  if (temp_scale > 143) {
    temp_scale = 143;
  }
  for (int i = 0; i < temp_scale; i++) {
    int line_width = temp_width[i];
    if (line_width > 0) {
      int colour_index = i / 11;
      if (colour_index > 12) {
        colour_index = 12;
      }
      graphics_context_set_stroke_color(ctx, temp_colour[colour_index]);
      graphics_draw_line(ctx, GPoint(0, 143-i), GPoint(line_width, 143-i));
    }
  }
}
#endif
