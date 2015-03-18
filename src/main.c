#include <pebble.h>

extern void setup_main_window();
extern void setup_time_service();
extern void setup_weather_service();
extern void teardown_main_window();

static void setup_app();
static void teardown_app();

int main(void) {
  setup_app();
  app_event_loop();
  teardown_app();
}

static void setup_app() {
  setup_main_window();
  setup_time_service();
  setup_weather_service();
}

static void teardown_app() {
  teardown_main_window();
}
