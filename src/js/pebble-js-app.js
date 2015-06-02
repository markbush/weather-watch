function xhrRequest(url, type, callback) {
  //log("URL: " + url);
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
}

function log(message) {
  console.log("[" + (new Date()) + "] " + message);
}

var config_url = 'http://markbush.github.io/weather-watch/';
var forecastType = 1;
var config = {};
var sunrise = 0;
var sunset = 0;
var DAY = 24 * 60 * 60;

function weatherCallback(responseText) {
  var json = {};
  try {
    json = JSON.parse(responseText);
  } catch (e) { }
  var dictionary = {
    "conditions": 0
  };
  if (typeof json.main !== 'undefined') {
    dictionary.temperature = Math.round(json.main.temp - 273.15);
    dictionary.conditions = json.weather[0].id;
    var nightTime = 1;
    var now = Math.round((new Date()).getTime() / 1000);
    if (sunrise === 0) {
      getForecast();
    }
    sunrise = json.sys.sunrise;
    sunset = json.sys.sunset;
    if (now >= json.sys.sunrise && now <= json.sys.sunset) {
      nightTime = 0;
    }
    dictionary.nightTime = nightTime;
  } else if (json.message) {
    log("No weather available: " + json.message);
  } else {
    log("Weather not available: " + responseText);
  }
  Pebble.sendAppMessage(dictionary,
    function(e) {
    },
    function(e) {
      log("Error sending weather info to Pebble!");
    }
  );
}

function hourlyForecastCallback(responseText) {
  var json = {};
  try {
    json = JSON.parse(responseText);
  } catch (e) { }
  var dictionary = { };
  if (typeof json.list !== 'undefined') {
    for (var i = 0; i < 6; i++) {
      var item = json.list[i];
      var date = new Date(item.dt * 1000);
      var temp = Math.round(item.main.temp - 273.15);
      var weather = item.weather[0].id;
      dictionary['forecastTitle'+i] = date.getHours();
      dictionary['forecastTempMin'+i] = temp;
      dictionary['forecastTempMax'+i] = temp;
      if (item.dt <= sunrise ||
          (item.dt >= sunset && item.dt <= (sunrise + DAY))) {
        weather += 1000;
      }
      dictionary['forecastWeather'+i] = weather;
    }
  } else if (json.message) {
    log("No weather available: " + json.message);
  } else {
    log("Weather not available: " + responseText);
  }
  Pebble.sendAppMessage(dictionary,
    function(e) {
    },
    function(e) {
      log("Error sending weather info to Pebble!");
    }
  );
}

function dailyForecastCallback(responseText) {
  var json = {};
  try {
    json = JSON.parse(responseText);
  } catch (e) { }
  var dictionary = { };
  if (typeof json.list !== 'undefined') {
    for (var i = 0; i < 6; i++) {
      var item = json.list[i];
      var date = new Date(item.dt * 1000);
      var tempMin = Math.round(item.temp.min - 273.15);
      var tempMax = Math.round(item.temp.max - 273.15);
      var weather = item.weather[0].id;
      dictionary['forecastTitle'+i] = date.getDate();
      dictionary['forecastTempMin'+i] = tempMin;
      dictionary['forecastTempMax'+i] = tempMax;
      dictionary['forecastWeather'+i] = weather;
    }
  } else if (json.message) {
    log("No weather available: " + json.message);
  } else {
    log("Weather not available: " + responseText);
  }
  Pebble.sendAppMessage(dictionary,
    function(e) {
    },
    function(e) {
      log("Error sending weather info to Pebble!");
    }
  );
}

function locationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  //log("URL: " + url);
  xhrRequest(url, 'GET', weatherCallback);
}

function hourlyForecastLocationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/forecast?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  //log("URL: " + url);
  xhrRequest(url, 'GET', hourlyForecastCallback);
}

function dailyForecastLocationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/forecast/daily?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  //log("URL: " + url);
  xhrRequest(url, 'GET', dailyForecastCallback);
}

function locationError(err) {
  var errMsg = "Unknown Error";
  if (typeof err !== "undefined") {
    errMsg = err.message;
    if (err.code == err.PERMISSION_DENIED) {
      errMsg = 'Location services denied';
    } else if (err.code == err.POSITION_UNAVAILABLE) {
      errMsg = 'Location not available';
    }
  }
  log("Error requesting location: " + errMsg);
  var dictionary = {
    "conditions": 1
  };
  Pebble.sendAppMessage(dictionary,
    function(e) {
    },
    function(e) {
      log("Error sending location error to Pebble!");
    }
  );
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

function getForecast() {
  var successCallback = hourlyForecastLocationSuccess;
  if (forecastType == 2) {
    successCallback = dailyForecastLocationSuccess;
  }
  //log("Forecast type: " + forecastType);
  navigator.geolocation.getCurrentPosition(
    successCallback,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    //log("AppMessage received: " + JSON.stringify(e));
    var requestType = e.payload['0'];
    if (typeof requestType === "undefined") {
      requestType = e.payload.temperature;
    }
    if (requestType === 0) {
      //log('Getting weather');
      getWeather();
    } else {
      //log('Getting forecast');
      forecastType = requestType;
      getForecast();
    }
  }
);

Pebble.addEventListener("showConfiguration", function(e) {
    Pebble.openURL(config_url);
});

Pebble.addEventListener('webviewclosed', function (e) {
  //log("Config result: " + e.response);

    if ((typeof e.response === 'string') && (e.response.length > 0)) {
        config = JSON.parse(e.response);
        if (typeof config.forecastType !== "undefined") {
          forecastType = config.forecastType;
        }

        // Store it locally
        window.localStorage.setItem('weatherWatch', e.response);

        // And send it to Pebble
        Pebble.sendAppMessage(config);
    }
});
