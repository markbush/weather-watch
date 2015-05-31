# Weather Watch

A simple watchface for Pebble with weather.

I prefer analogue watchfaces, however pretty much all of the ones available use full hands which then often obscure the extra information being displayed on the watch.  I created this one with hand indicators that only show near the outside.  This keeps everything nice and clean.

This has been extensively updated for Pebble Time.  There are now large colour backgrounds showing the current weather conditions.  The temperature is displayed as both a value and as a colour gradient chart.  These colours are used in the forecast that can now be seen at the bottom which is easier to see than text would be.  For hourly forecasts, the hour of the forecast is shown, for daily forecasts, the date is shown.

Typical views of the watch are as follows (Original Pebble and Pebble Time respectively):

![Weather Watch](https://raw.githubusercontent.com/markbush/weather-watch/master/resources/images/watch-1.png "Weather Watch") ![Weather Watch](https://raw.githubusercontent.com/markbush/weather-watch/master/resources/images/watch-time-1.png "Weather Watch")

### Communication Errors

![Phone Error](https://raw.githubusercontent.com/markbush/weather-watch/master/resources/images/query.png "Phone Error") Waiting for connection to the phone to obtain weather info.

![Location Error](https://raw.githubusercontent.com/markbush/weather-watch/master/resources/images/location.png "Location Error") Cannot obtain location.  Check phone's location services are turned on.  If they are, then the phone is currently unable to determine its location.

![Weather Error](https://raw.githubusercontent.com/markbush/weather-watch/master/resources/images/alert.png "Weather Error") The weather service is unavailable.  This either means the service is temporarily down or your phone has no network coverage.

If a communication error occurs, you can force the watchface to try again by switching to another app and back on the Pebble.  If the error persists, then either Pebble cannot connect to your phone (check your bluetooth settings) or your phone is not connecting to the internet (try again when you have better network coverage).

## Configuration

There are a number of configuration items available:

* Temperature unit (&deg;C or &deg;F)
* Battery status (you can choose to show the battery status always, never, only when charging or while it is less than a specified threshold)
* Hide/show temperature
* Hide/show weather conditions
* Bluetooth indicator (showing while connected or never showing)
* Bluetooth disconnect alert (choose a type of vibration alert when the watch loses bluetooth connectivity with your phone)
* Language
* Hourly or daily forecast

You can select to view the date in one of the following languages: Bulgarian, Byelorussian, Chinese, Croatian, Czech, Danish, Dutch, English, Estonian, Finnish, French, German, Greek, Hungarian, Icelandic, Italian, Japanese, Lithuanian, Norwegian, Polish, Portuguese, Romanian, Russian, Slovak, Slovenian, Spanish, Swedish, Turkish, Ukranian
