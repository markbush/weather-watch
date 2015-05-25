#include <pebble.h>

extern void update_date_display();
extern void get_weather();
#ifdef PBL_COLOR
extern void get_forecast();
#endif
extern Layer *s_hands_layer;
extern TextLayer *s_date_layer;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void update_time(struct tm *tick_time);

struct tm pebble_time;

void setup_time_service() {
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
  // Get weather update every 10 minutes
  if (tick_time->tm_min % 10 == 0) {
    get_weather();
  }
#ifdef PBL_COLOR
  // Get weather forecast every half hour
  // but offset from weather update
  //if (tick_time->tm_min % 30 == 3) {
    get_forecast();
  //}
#endif
}

void update_time(struct tm *tick_time) {
  memcpy(&pebble_time, tick_time, sizeof(struct tm));
  //pebble_time = tick_time;

  layer_mark_dirty(s_hands_layer);
  update_date_display();
}
