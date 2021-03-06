#include <pebble.h>

#define CELSIUS 0
#define FAHRENHEIT 1

#define FORECAST_HOURLY 1
#define FORECAST_DAILY  2

void update_weather_conditions_display(int weather_index);
void update_weather_temperature_display();
void update_weather(int update_type);
void get_weather();

static TextLayer *s_temperature_text_layer;
int g_temperature = 0;
uint32_t g_temperature_unit = CELSIUS;
int g_temperature_showing = 0;
int g_weather_showing = 0;
int g_forecast_type = FORECAST_HOURLY;

#ifdef PBL_COLOR
void get_forecast();
static Layer *s_temperature_layer;
static BitmapLayer *s_temperature_scale_layer;
static GBitmap *s_temperature_scale_bitmap;
static TextLayer *s_temperature_scale_text_layer[3];
static TextLayer *s_forecast_text_layer[6];
static Layer *s_forecast_temperature_layer[6];
static BitmapLayer *s_forecast_conditions_layer[6];
static GBitmap *s_forecast_bitmap[6];

extern void set_weather_background(uint32_t background_resource);
static void temp_scale_update_proc(Layer *layer, GContext *ctx);
static void forecast_temperature_update_proc(Layer *layer, GContext *ctx);
int16_t temp_width[144] = {
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
int temp_value[2][3] = {{0, 10, 20}, {30, 50, 70}};
int temp_scale_y_offset[3] = {123, 73, 23};
int g_forecast_title[6];
int g_forecast_temp_min[6];
int g_forecast_temp_max[6];
int g_forecast_weather[6];
int g_night_time = 0;
static uint32_t s_weather_state = RESOURCE_ID_IMAGE_BACKGROUND_DAY_QUERY;
static int s_weather_index = 0;
static uint32_t weather_resource_day[13] = {
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_QUERY,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_ALERT,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_LOCATION,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_CLEAR,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_CLOUD_FEW,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_CLOUD_SCATTERED,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_CLOUD_BROKEN,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_THUNDERSTORM,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_RAIN_LIGHT,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_RAIN_HEAVY,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_SNOW,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_MIST,
  RESOURCE_ID_IMAGE_BACKGROUND_DAY_WIND
};
static uint32_t weather_resource_night[13] = {
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_QUERY,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_ALERT,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_LOCATION,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_CLEAR,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_CLOUD_FEW,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_CLOUD_SCATTERED,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_CLOUD_BROKEN,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_THUNDERSTORM,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_RAIN_LIGHT,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_RAIN_HEAVY,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_SNOW,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_MIST,
  RESOURCE_ID_IMAGE_BACKGROUND_NIGHT_WIND
};
static uint32_t forecast_resource_day[13] = {
  RESOURCE_ID_IMAGE_FORECAST_DAY_QUERY,
  RESOURCE_ID_IMAGE_FORECAST_DAY_ALERT,
  RESOURCE_ID_IMAGE_FORECAST_DAY_LOCATION,
  RESOURCE_ID_IMAGE_FORECAST_DAY_CLEAR,
  RESOURCE_ID_IMAGE_FORECAST_DAY_CLOUD_FEW,
  RESOURCE_ID_IMAGE_FORECAST_DAY_CLOUD_SCATTERED,
  RESOURCE_ID_IMAGE_FORECAST_DAY_CLOUD_BROKEN,
  RESOURCE_ID_IMAGE_FORECAST_DAY_THUNDERSTORM,
  RESOURCE_ID_IMAGE_FORECAST_DAY_RAIN_LIGHT,
  RESOURCE_ID_IMAGE_FORECAST_DAY_RAIN_HEAVY,
  RESOURCE_ID_IMAGE_FORECAST_DAY_SNOW,
  RESOURCE_ID_IMAGE_FORECAST_DAY_MIST,
  RESOURCE_ID_IMAGE_FORECAST_DAY_WIND
};
static uint32_t forecast_resource_night[13] = {
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_QUERY,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_ALERT,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_LOCATION,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_CLEAR,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_CLOUD_FEW,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_CLOUD_SCATTERED,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_CLOUD_BROKEN,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_THUNDERSTORM,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_RAIN_LIGHT,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_RAIN_HEAVY,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_SNOW,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_MIST,
  RESOURCE_ID_IMAGE_FORECAST_NIGHT_WIND
};
#else
static BitmapLayer *s_conditions_layer;
static GBitmap *s_conditions_bitmap;
static uint32_t s_weather_state = RESOURCE_ID_IMAGE_QUERY;
static uint32_t weather_resource[13] = {
  RESOURCE_ID_IMAGE_QUERY,
  RESOURCE_ID_IMAGE_ALERT,
  RESOURCE_ID_IMAGE_LOCATION,
  RESOURCE_ID_IMAGE_CLEAR,
  RESOURCE_ID_IMAGE_CLOUDS_FEW,
  RESOURCE_ID_IMAGE_CLOUDS_SCATTERED,
  RESOURCE_ID_IMAGE_CLOUDS_BROKEN,
  RESOURCE_ID_IMAGE_THUNDERSTORM,
  RESOURCE_ID_IMAGE_RAIN_LIGHT,
  RESOURCE_ID_IMAGE_RAIN_HEAVY,
  RESOURCE_ID_IMAGE_SNOW,
  RESOURCE_ID_IMAGE_MIST,
  RESOURCE_ID_IMAGE_WIND
};
#endif

void setup_weather(Layer *root) {
  // Create temperature Layers
#ifdef PBL_COLOR
  s_temperature_layer = layer_create(GRect(0, 0, 70, 144));
  layer_add_child(root, s_temperature_layer);
  layer_set_update_proc(s_temperature_layer, temp_scale_update_proc);
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

  s_temperature_scale_layer = bitmap_layer_create(GRect(0, 0, 4, 144));
  bitmap_layer_set_compositing_mode(s_temperature_scale_layer, GCompOpSet);
  s_temperature_scale_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TEMP_SCALE);
  bitmap_layer_set_bitmap(s_temperature_scale_layer, s_temperature_scale_bitmap);
  layer_add_child(root, bitmap_layer_get_layer(s_temperature_scale_layer));

  for (int i = 0; i < 3; i++) {
    s_temperature_scale_text_layer[i] = text_layer_create(GRect(2, temp_scale_y_offset[i], 10, 9));
    text_layer_set_font(s_temperature_scale_text_layer[i], fonts_get_system_font(FONT_KEY_GOTHIC_09));
    text_layer_set_text_color(s_temperature_scale_text_layer[i], GColorBlack);
    text_layer_set_text_alignment(s_temperature_scale_text_layer[i], GTextAlignmentRight);
    text_layer_set_background_color(s_temperature_scale_text_layer[i], GColorClear);
    layer_add_child(root, text_layer_get_layer(s_temperature_scale_text_layer[i]));
  }

  for (int i = 0; i < 6; i++) {
    s_forecast_conditions_layer[i] = bitmap_layer_create(GRect((i * 24), 144, 24, 14));
    layer_add_child(root, bitmap_layer_get_layer(s_forecast_conditions_layer[i]));

    s_forecast_temperature_layer[i] = layer_create(GRect((i * 24), 158, 24, 10));
    layer_set_update_proc(s_forecast_temperature_layer[i], forecast_temperature_update_proc);
    layer_add_child(root, s_forecast_temperature_layer[i]);

    s_forecast_text_layer[i] = text_layer_create(GRect((i * 24), 158, 10, 10));
    text_layer_set_font(s_forecast_text_layer[i], fonts_get_system_font(FONT_KEY_GOTHIC_09));
    text_layer_set_text_color(s_forecast_text_layer[i], GColorBlack);
    text_layer_set_text_alignment(s_forecast_text_layer[i], GTextAlignmentRight);
    text_layer_set_background_color(s_forecast_text_layer[i], GColorClear);
    layer_add_child(root, text_layer_get_layer(s_forecast_text_layer[i]));
  }

  s_temperature_text_layer = text_layer_create(GRect(2, 0, 26, 10));
  text_layer_set_font(s_temperature_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_09));
  text_layer_set_text_color(s_temperature_text_layer, GColorBlack);
  text_layer_set_text_alignment(s_temperature_text_layer, GTextAlignmentRight);
#else
  // Create conditions Layer
  s_conditions_layer = bitmap_layer_create(GRect(56, 100, 32, 32));
  s_conditions_bitmap = gbitmap_create_with_resource(s_weather_state);
  bitmap_layer_set_bitmap(s_conditions_layer, s_conditions_bitmap);
  layer_add_child(root, bitmap_layer_get_layer(s_conditions_layer));

  s_temperature_text_layer = text_layer_create(GRect(0, 30, 144, 28));
  text_layer_set_font(s_temperature_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(s_temperature_text_layer, GColorWhite);
  text_layer_set_text_alignment(s_temperature_text_layer, GTextAlignmentCenter);
#endif
  text_layer_set_background_color(s_temperature_text_layer, GColorClear);
  layer_add_child(root, text_layer_get_layer(s_temperature_text_layer));

  update_weather(0);
}

void teardown_weather() {
  text_layer_destroy(s_temperature_text_layer);
#ifdef PBL_COLOR
  layer_destroy(s_temperature_layer);
  gbitmap_destroy(s_temperature_scale_bitmap);
  bitmap_layer_destroy(s_temperature_scale_layer);
  for (int i = 0; i < 3; i++) {
    text_layer_destroy(s_temperature_scale_text_layer[i]);
  }
  for (int i = 0; i < 6; i++) {
    text_layer_destroy(s_forecast_text_layer[i]);
    layer_destroy(s_forecast_temperature_layer[i]);
    bitmap_layer_destroy(s_forecast_conditions_layer[i]);
    if (s_forecast_bitmap[i]) {
      gbitmap_destroy(s_forecast_bitmap[i]);
    }
  }
#else
  gbitmap_destroy(s_conditions_bitmap);
  bitmap_layer_destroy(s_conditions_layer);
#endif
}

void update_weather_conditions_display(int weather_index) {
  static int failures = 0;
  uint32_t weather_state;
#ifdef PBL_COLOR
  if (g_night_time) {
    weather_state = weather_resource_night[weather_index];
  } else {
    weather_state = weather_resource_day[weather_index];
  }
  s_weather_index = weather_index;
#else
  weather_state = weather_resource[weather_index];
#endif

  if (weather_index <= 2) {
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
    set_weather_background(s_weather_state);
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
#ifdef PBL_COLOR
  static char temp_scale_value_buffer[3][4];
  if (g_temperature_unit == CELSIUS) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d C", g_temperature);
  } else {
    int temperature = (g_temperature * 9 / 5) + 32;
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d F", temperature);
  }
  text_layer_set_text(s_temperature_text_layer, temperature_buffer);

  for (int i = 0; i < 3; i++) {
    int temp_mark = temp_value[g_temperature_unit][i];
    snprintf(temp_scale_value_buffer[i], sizeof(temp_scale_value_buffer), "%d", temp_mark);
    text_layer_set_text(s_temperature_scale_text_layer[i], temp_scale_value_buffer[i]);
  }
  layer_mark_dirty(s_temperature_layer);
#else
  if (s_weather_state == RESOURCE_ID_IMAGE_ALERT
    || s_weather_state == RESOURCE_ID_IMAGE_LOCATION
    || s_weather_state == RESOURCE_ID_IMAGE_QUERY) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "?");
  } else {
    if (g_temperature_unit == CELSIUS) {
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°C", g_temperature);
    } else {
      int temperature = (g_temperature * 9 / 5) + 32;
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%d°F", temperature);
    }
  }
  text_layer_set_text(s_temperature_text_layer, temperature_buffer);
#endif
}

void update_weather(int update_type) {
#ifdef PBL_COLOR
  static int night_time = 0;
#endif

  if (update_type == 0) {
    if (g_temperature_showing == 0) {
      layer_set_hidden(text_layer_get_layer(s_temperature_text_layer), false);
    } else {
      layer_set_hidden(text_layer_get_layer(s_temperature_text_layer), true);
    }
  }
#ifdef PBL_COLOR
  static char forecast_title_buffer[6][4];
  for (int i = 0; i < 6; i++) {
    if (update_type == 1) {
      snprintf(forecast_title_buffer[i], sizeof(forecast_title_buffer), "%d", g_forecast_title[i]);
      text_layer_set_text(s_forecast_text_layer[i], forecast_title_buffer[i]);
    } else if (update_type == 2) {
      layer_mark_dirty(s_forecast_temperature_layer[i]);
    } else if (update_type == 3) {
      if (s_forecast_bitmap[i]) {
        gbitmap_destroy(s_forecast_bitmap[i]);
      }
      int forecast_resource;
      int forecast_index = g_forecast_weather[i];
      if (forecast_index >= 1000) {
        forecast_resource = forecast_resource_night[forecast_index - 1000];
      } else {
        forecast_resource = forecast_resource_day[forecast_index];
      }
      s_forecast_bitmap[i] = gbitmap_create_with_resource(forecast_resource);
      bitmap_layer_set_bitmap(s_forecast_conditions_layer[i], s_forecast_bitmap[i]);
    }
  }
  if (update_type == 4 && g_night_time != night_time) {
    night_time = g_night_time;
    update_weather_conditions_display(s_weather_index);
  }
#else
  if (update_type == 0) {
    if (g_weather_showing == 0) {
      layer_set_hidden(bitmap_layer_get_layer(s_conditions_layer), false);
    } else {
      layer_set_hidden(bitmap_layer_get_layer(s_conditions_layer), true);
    }
  }
#endif
}

void get_weather() {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint8(iter, 0, 0);
  app_message_outbox_send();
}

#ifdef PBL_COLOR
void get_forecast() {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint8(iter, 0, g_forecast_type);
  app_message_outbox_send();
}

static int temp_to_scale(int temperature) {
  int temp_scale = (temperature + 4) * 144 / 29;
  if (temp_scale < 0) {
    temp_scale = 0;
  }
  if (temp_scale > 143) {
    temp_scale = 143;
  }
  return temp_scale;
}

static int scale_to_index(int scale) {
  int colour_index = scale / 11;
  if (colour_index > 12) {
    colour_index = 12;
  }
  return colour_index;
}

static void temp_scale_update_proc(Layer *layer, GContext *ctx) {
  int temp_scale = temp_to_scale(g_temperature);
  for (int i = 0; i < temp_scale; i++) {
    int line_width = temp_width[i];
    if (line_width > 0) {
      int colour_index = scale_to_index(i);
      graphics_context_set_stroke_color(ctx, temp_colour[colour_index]);
      graphics_draw_line(ctx, GPoint(0, 143-i), GPoint(line_width, 143-i));
    }
  }
}

static void forecast_temperature_update_proc(Layer *layer, GContext *ctx) {
  for (int i = 0; i < 6; i++) {
    if (layer == s_forecast_temperature_layer[i]) {
      int temp_scale = temp_to_scale(g_forecast_temp_max[i]);
      int colour_index = scale_to_index(temp_scale);
      graphics_context_set_fill_color(ctx, temp_colour[colour_index]);
      graphics_fill_rect(ctx, GRect(0, 0, 24, 5), 0, GCornerNone);

      temp_scale = temp_to_scale(g_forecast_temp_min[i]);
      colour_index = scale_to_index(temp_scale);
      graphics_context_set_fill_color(ctx, temp_colour[colour_index]);
      graphics_fill_rect(ctx, GRect(0, 5, 24, 5), 0, GCornerNone);
    }
  }
}
#endif
