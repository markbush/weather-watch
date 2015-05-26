#include <pebble.h>

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_TEMP_UNIT 2
#define KEY_BATTERY 3
#define KEY_BLUETOOTH 4
#define KEY_DISCONNECT 5
#define KEY_LANGUAGE 6
#define KEY_SHOW_TEMP 7
#define KEY_SHOW_WEATHER 8
#define KEY_FORECAST_TYPE 9
#define KEY_FORECAST_TITLE_0 10
#define KEY_FORECAST_TITLE_1 11
#define KEY_FORECAST_TITLE_2 12
#define KEY_FORECAST_TITLE_3 13
#define KEY_FORECAST_TITLE_4 14
#define KEY_FORECAST_TITLE_5 15
#define KEY_FORECAST_TEMP_MIN_0 16
#define KEY_FORECAST_TEMP_MIN_1 17
#define KEY_FORECAST_TEMP_MIN_2 18
#define KEY_FORECAST_TEMP_MIN_3 19
#define KEY_FORECAST_TEMP_MIN_4 20
#define KEY_FORECAST_TEMP_MIN_5 21
#define KEY_FORECAST_TEMP_MAX_0 22
#define KEY_FORECAST_TEMP_MAX_1 23
#define KEY_FORECAST_TEMP_MAX_2 24
#define KEY_FORECAST_TEMP_MAX_3 25
#define KEY_FORECAST_TEMP_MAX_4 26
#define KEY_FORECAST_TEMP_MAX_5 27
#define KEY_FORECAST_WEATHER_0 28
#define KEY_FORECAST_WEATHER_1 29
#define KEY_FORECAST_WEATHER_2 30
#define KEY_FORECAST_WEATHER_3 31
#define KEY_FORECAST_WEATHER_4 32
#define KEY_FORECAST_WEATHER_5 33
#define KEY_NIGHT_TIME 34

extern void update_weather_conditions_display(uint32_t weather_state);
extern void update_weather_temperature_display();
extern void update_battery();
extern void update_bluetooth();
extern void update_date_display();
extern void update_weather(int update_type);
#ifdef PBL_COLOR
extern void get_forecast();

extern int g_forecast_title[6];
extern int g_forecast_temp_min[6];
extern int g_forecast_temp_max[6];
extern int g_forecast_weather[6];
#endif

static void inbox_received_callback(DictionaryIterator *iterator, void *context);
static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
static int weather_index_for_conditions(uint32_t conditions);
void setup_remote_services();
void load_config();
void save_config();

extern uint32_t g_temperature_unit;
extern int g_temperature;
extern int g_battery_showing;
extern int g_bluetooth_showing;
extern int g_bluetooth_disconnect;
extern int g_locale;
extern int g_temperature_showing;
extern int g_weather_showing;
extern int g_forecast_type;
extern int g_night_time;

void setup_remote_services() {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Initialise the service and retrieve initial conditions
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void load_config() {
  if (persist_exists(KEY_TEMP_UNIT)) {
    g_temperature_unit = persist_read_int(KEY_TEMP_UNIT);
  }
  if (persist_exists(KEY_BATTERY)) {
    g_battery_showing = persist_read_int(KEY_BATTERY);
  }
  if (persist_exists(KEY_BLUETOOTH)) {
    g_bluetooth_showing = persist_read_int(KEY_BLUETOOTH);
  }
  if (persist_exists(KEY_DISCONNECT)) {
    g_bluetooth_disconnect = persist_read_int(KEY_DISCONNECT);
  }
  if (persist_exists(KEY_LANGUAGE)) {
    g_locale = persist_read_int(KEY_LANGUAGE);
  }
  if (persist_exists(KEY_SHOW_TEMP)) {
    g_temperature_showing = persist_read_int(KEY_SHOW_TEMP);
  }
  if (persist_exists(KEY_SHOW_WEATHER)) {
    g_weather_showing = persist_read_int(KEY_SHOW_WEATHER);
  }
  if (persist_exists(KEY_FORECAST_TYPE)) {
    g_forecast_type = persist_read_int(KEY_FORECAST_TYPE);
  }
}

void save_config() {
  persist_write_int(KEY_TEMP_UNIT, g_temperature_unit);
  persist_write_int(KEY_BATTERY, g_battery_showing);
  persist_write_int(KEY_BLUETOOTH, g_bluetooth_showing);
  persist_write_int(KEY_DISCONNECT, g_bluetooth_disconnect);
  persist_write_int(KEY_LANGUAGE, g_locale);
  persist_write_int(KEY_SHOW_TEMP, g_temperature_showing);
  persist_write_int(KEY_SHOW_WEATHER, g_weather_showing);
  persist_write_int(KEY_FORECAST_TYPE, g_forecast_type);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t = dict_read_first(iterator);
  bool need_update_0 = false;
#ifdef PBL_COLOR
  int offset;
  bool need_update_1 = false;
  bool need_update_2 = false;
  bool need_update_3 = false;
  bool need_update_4 = false;
#endif

  // For all items
  while (t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_TEMPERATURE:
      ;
      g_temperature = (int)t->value->int32;
      update_weather_temperature_display();
      break;
    case KEY_CONDITIONS:
      ;
      int weather_index = weather_index_for_conditions((uint32_t)t->value->int32);
      update_weather_conditions_display(weather_index);
      update_weather_temperature_display();
      break;
    case KEY_TEMP_UNIT:
      g_temperature_unit = (uint32_t)t->value->int32;
      update_weather_temperature_display();
      break;
    case KEY_BATTERY:
      ;
      g_battery_showing = (int)t->value->int32;
      update_battery();
      break;
    case KEY_BLUETOOTH:
      ;
      g_bluetooth_showing = (int)t->value->int32;
      update_bluetooth();
      break;
    case KEY_DISCONNECT:
      ;
      g_bluetooth_disconnect = (int)t->value->int32;
      break;
    case KEY_LANGUAGE:
      ;
      g_locale = (int)t->value->int32;
      update_date_display();
      break;
    case KEY_SHOW_TEMP:
      ;
      g_temperature_showing = (int)t->value->int32;
      need_update_0 = true;
      break;
    case KEY_SHOW_WEATHER:
      ;
      g_weather_showing = (int)t->value->int32;
      need_update_0 = true;
      break;
#ifdef PBL_COLOR
    case KEY_FORECAST_TYPE:
      ;
      g_forecast_type = (int)t->value->int32;
      get_forecast();
      break;
    case KEY_FORECAST_TITLE_0:
    case KEY_FORECAST_TITLE_1:
    case KEY_FORECAST_TITLE_2:
    case KEY_FORECAST_TITLE_3:
    case KEY_FORECAST_TITLE_4:
    case KEY_FORECAST_TITLE_5:
      ;
      offset = t->key - KEY_FORECAST_TITLE_0;
      if (g_forecast_title[offset] != (int)t->value->int32) {
        g_forecast_title[offset] = (int)t->value->int32;
        need_update_1 = true;
      }
      break;
    case KEY_FORECAST_TEMP_MIN_0:
    case KEY_FORECAST_TEMP_MIN_1:
    case KEY_FORECAST_TEMP_MIN_2:
    case KEY_FORECAST_TEMP_MIN_3:
    case KEY_FORECAST_TEMP_MIN_4:
    case KEY_FORECAST_TEMP_MIN_5:
      ;
      offset = t->key - KEY_FORECAST_TEMP_MIN_0;
      if (g_forecast_temp_min[offset] != (int)t->value->int32) {
        g_forecast_temp_min[offset] = (int)t->value->int32;
        need_update_2 = true;
      }
      break;
    case KEY_FORECAST_TEMP_MAX_0:
    case KEY_FORECAST_TEMP_MAX_1:
    case KEY_FORECAST_TEMP_MAX_2:
    case KEY_FORECAST_TEMP_MAX_3:
    case KEY_FORECAST_TEMP_MAX_4:
    case KEY_FORECAST_TEMP_MAX_5:
      ;
      offset = t->key - KEY_FORECAST_TEMP_MAX_0;
      if (g_forecast_temp_max[offset] != (int)t->value->int32) {
        g_forecast_temp_max[offset] = (int)t->value->int32;
        need_update_2 = true;
      }
      break;
    case KEY_FORECAST_WEATHER_0:
    case KEY_FORECAST_WEATHER_1:
    case KEY_FORECAST_WEATHER_2:
    case KEY_FORECAST_WEATHER_3:
    case KEY_FORECAST_WEATHER_4:
    case KEY_FORECAST_WEATHER_5:
      ;
      offset = t->key - KEY_FORECAST_WEATHER_0;
      if (g_forecast_weather[offset] != (int)t->value->int32) {
        g_forecast_weather[offset] = (int)t->value->int32;
        need_update_3 = true;
      }
      break;
    case KEY_NIGHT_TIME:
      ;
      g_night_time = (int)t->value->int32;
      need_update_4 = true;
      break;
#endif
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }

  if (need_update_0) {
    update_weather(0);
  }
#ifdef PBL_COLOR
  if (need_update_1) {
    update_weather(1);
  }
  if (need_update_2) {
    update_weather(2);
  }
  if (need_update_3) {
    update_weather(3);
  }
  if (need_update_4) {
    update_weather(4);
  }
#endif
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed: %d", reason);
  update_weather_conditions_display(0);
  update_weather_temperature_display();
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static int weather_index_for_conditions(uint32_t conditions) {
  int weather_state = 0;
  if (conditions == 0) {
    weather_state = 1;
  } else if (conditions == 1) {
    weather_state = 2;
  } else if (conditions == 800) {
    weather_state = 3;
  } else if (conditions == 801) {
    weather_state = 4;
  } else if (conditions == 802) {
    weather_state = 5;
  } else if (conditions == 803 || conditions == 804) {
    weather_state = 6;
  } else if (conditions >= 200 && conditions < 300) {
    weather_state = 7;
  } else if (conditions >= 300 && conditions < 400) {
    weather_state = 8;
  } else if (conditions == 500 || conditions == 501) {
    weather_state = 8;
  } else if (conditions >= 502 && conditions < 600) {
    weather_state = 9;
  } else if (conditions >= 600 && conditions < 700) {
    weather_state = 10;
  } else if (conditions >= 700 && conditions < 800) {
    weather_state = 11;
  } else if (conditions >= 900 && conditions < 906) {
    weather_state = 12;
  } else if (conditions == 906) {
    weather_state = 10;
  } else if (conditions >= 951 && conditions < 955) {
    weather_state = 3;
  } else if (conditions >= 955) {
    weather_state = 12;
  }
  return weather_state;
}
