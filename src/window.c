#include <pebble.h>

extern void setup_hands(Layer *root);
extern void teardown_hands();

extern void setup_date(Layer *root);
extern void teardown_date();

extern void setup_weather(Layer *root);
extern void teardown_weather();

extern void setup_bluetooth(Layer *root);
extern void teardown_bluetooth();

extern void setup_battery(Layer *root);
extern void teardown_battery();

static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void setup_background(Layer *root);
static void teardown_background();

static Window *s_main_window;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

void setup_main_window() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);  
}

void teardown_main_window() {
  window_destroy(s_main_window);
}

static void main_window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  setup_background(root_layer);
  setup_date(root_layer);
  setup_weather(root_layer);
  setup_hands(root_layer);
  setup_bluetooth(root_layer);
  setup_battery(root_layer);
}

static void main_window_unload(Window *window) {
  teardown_battery();
  teardown_bluetooth();
  teardown_hands();
  teardown_weather();
  teardown_date();
  teardown_background();
}

static void setup_background(Layer *root) {
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(root, bitmap_layer_get_layer(s_background_layer));
}

static void teardown_background() {
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
}