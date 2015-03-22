var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

var config_url = 'http://markbush.github.io/weather-watch/';
var config = {};

function locationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude;

  // Send request to OpenWeatherMap
  //console.log("URL: " + url);
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      //console.log("Weather: " + responseText);
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      //console.log("Temperature is " + temperature);

      // Conditions
      var conditions = json.weather[0].id;      
      //console.log("Conditions are " + conditions);

      // Assemble dictionary using our keys
      var dictionary = {
        "temperature": temperature,
        "conditions": conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          //console.log("Weather info: " + json.main.temp + "(" + temperature + ")" + " " + conditions);
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}

function locationError(err) {
  console.log("Error requesting location!");
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