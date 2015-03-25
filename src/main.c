#include <pebble.h>

extern void setup_main_window();
extern void setup_time_service();
extern void setup_remote_services();
extern void teardown_main_window();
extern void load_config();
extern void save_config();

static void setup_app();
static void teardown_app();

int main(void) {
  setup_app();
  app_event_loop();
  teardown_app();
}

static void setup_app() {
  load_config();
  setup_main_window();
  setup_time_service();
  setup_remote_services();
}

static void teardown_app() {
  teardown_main_window();
  save_config();
}
