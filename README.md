# Weather Watch

A simple watchface for Pebble with weather.

I prefer analogue watchfaces, however pretty much all of the ones available use full hands which then often obscure the extra information being displayed on the watch.  I created this one with hand indicators that only show near the outside.  This keeps everything nice and clean.

A typical view of the watch is as follows:

![Weather Watch](https://raw.githubusercontent.com/markbush/weather-watch/master/resources/images/watch-1.png "Weather Watch")

## Configuration

There are a number of configuration items available:

* Temperature unit (&deg;C or &deg;F)
* Battery status (you can choose to show the battery status always, never, only when charging or while it is less than a specified threshold)
* Bluetooth indicator (showing while connected or never showing)
* Bluetooth disconnect alert (choose a type of vibration alert when the watch loses bluetooth connectivity with your phone)

I am going to be adding configuration for whether the temperature and weather conditions are shown and aim to provide support for displaying the date in non-English locales.
