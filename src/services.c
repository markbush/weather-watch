#include <pebble.h>

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_TEMP_UNIT 2
#define KEY_BATTERY 3
#define KEY_BLUETOOTH 4
#define KEY_DISCONNECT 5

extern void update_weather_conditions_display(uint32_t weather_state);
extern void update_weather_temperature_display();
extern void use_temperature_unit(uint32_t temperature_unit);
extern void update_battery();
extern void update_bluetooth();

static void inbox_received_callback(DictionaryIterator *iterator, void *context);
static void inbox_dropped_callback(AppMessageResult reason, void *context);
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
static void outbox_sent_callback(DictionaryIterator *iterator, void *context);
static uint32_t weather_state_for_conditions(uint32_t conditions);

extern int s_temperature;
extern int s_battery_showing;
extern int s_bluetooth_showing;
extern int s_bluetooth_disconnect;

void setup_weather_service() {
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Initialise the service and retrieve initial conditions
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());  
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while (t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_TEMPERATURE:
      ;
      s_temperature = (int)t->value->int32;
      update_weather_temperature_display();
      break;
    case KEY_CONDITIONS:
      ;
      uint32_t weather_state = weather_state_for_conditions((uint32_t)t->value->int32);
      update_weather_conditions_display(weather_state);
      break;
    case KEY_TEMP_UNIT:
      use_temperature_unit((uint32_t)t->value->int32);
      update_weather_temperature_display();
      break;
    case KEY_BATTERY:
      ;
      s_battery_showing = (int)t->value->int32;
      update_battery();
      break;
    case KEY_BLUETOOTH:
      ;
      s_bluetooth_showing = (int)t->value->int32;
      update_bluetooth();
      break;
    case KEY_DISCONNECT:
      ;
      s_bluetooth_disconnect = (int)t->value->int32;
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  //APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static uint32_t weather_state_for_conditions(uint32_t conditions) {
  uint32_t weather_state = RESOURCE_ID_IMAGE_ALERT;
  if (conditions == 800) {
    weather_state = RESOURCE_ID_IMAGE_CLEAR;
  } else if (conditions == 801) {
    weather_state = RESOURCE_ID_IMAGE_CLOUDS_FEW;
  } else if (conditions == 802) {
    weather_state = RESOURCE_ID_IMAGE_CLOUDS_SCATTERED;
  } else if (conditions == 803 || conditions == 804) {
    weather_state = RESOURCE_ID_IMAGE_CLOUDS_BROKEN;
  } else if (conditions >= 200 && conditions < 300) {
    weather_state = RESOURCE_ID_IMAGE_THUNDERSTORM;
  } else if (conditions >= 300 && conditions < 400) {
    weather_state = RESOURCE_ID_IMAGE_RAIN_LIGHT;
  } else if (conditions == 500 || conditions == 501) {
    weather_state = RESOURCE_ID_IMAGE_RAIN_LIGHT;
  } else if (conditions >= 502 && conditions < 600) {
    weather_state = RESOURCE_ID_IMAGE_RAIN_HEAVY;
  } else if (conditions >= 600 && conditions < 700) {
    weather_state = RESOURCE_ID_IMAGE_SNOW;
  } else if (conditions >= 700 && conditions < 800) {
    weather_state = RESOURCE_ID_IMAGE_MIST;
  } else if (conditions >= 900 && conditions < 906) {
    weather_state = RESOURCE_ID_IMAGE_WIND;
  } else if (conditions == 906) {
    weather_state = RESOURCE_ID_IMAGE_SNOW;
  } else if (conditions >= 951 && conditions < 955) {
    weather_state = RESOURCE_ID_IMAGE_CLEAR;
  } else if (conditions >= 955) {
    weather_state = RESOURCE_ID_IMAGE_WIND;
  }
  return weather_state;
}