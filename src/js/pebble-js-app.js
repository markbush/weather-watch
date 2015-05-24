function xhrRequest(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
}

var config_url = 'http://markbush.github.io/weather-watch/';
var config = {};

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
  } else if (json.message) {
    console.log("No weather available: " + json.message);
  } else {
    console.log("Weather not available: " + responseText);
  }
  Pebble.sendAppMessage(dictionary,
    function(e) {
    },
    function(e) {
      console.log("Error sending weather info to Pebble!");
    }
  );
}

function locationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  //console.log("URL: " + url);
  xhrRequest(url, 'GET', weatherCallback);
}

function locationError(err) {
  var errMsg = err.message;
  if (err.code == err.PERMISSION_DENIED) {
    errMsg = 'Location services denied';
  } else if (err.code == err.POSITION_UNAVAILABLE) {
    errMsg = 'Location not available';
  }
  console.log("Error requesting location: " + errMsg);
  var dictionary = {
    "conditions": 1
  };
  Pebble.sendAppMessage(dictionary,
    function(e) {
    },
    function(e) {
      console.log("Error sending location error to Pebble!");
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

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    //console.log("AppMessage received!");
    getWeather();
  }
);

Pebble.addEventListener("showConfiguration", function(e) {
    Pebble.openURL(config_url);
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log("Config result: " + e.response);

    if ((typeof e.response === 'string') && (e.response.length > 0)) {
        config = JSON.parse(e.response);

        // Store it locally
        window.localStorage.setItem('weatherWatch', e.response);

        // And send it to Pebble
        Pebble.sendAppMessage(config);
    }
});
