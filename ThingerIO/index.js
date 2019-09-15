slider = document.getElementById('slider');
ledOn = document.getElementById('LEDon');
ledOff = document.getElementById('LEDoff');
var request = new XMLHttpRequest();
var weatherData;

document.getElementById('slider').addEventListener('change', callData);

initialize();
setInterval(getData, 60000);


function initialize() {

    ledOn.hidden = true;
    ledOff.hidden = false;
    getData();
    getWeather();

}

function callData() {

    var switchValue = !slider.checked;
    var switchLED = {
    "async": true,
    "crossDomain": true,
    "url": "API_URL",
    "method": "POST",
    "headers": {
        "Content-Type": "application/json",
        "Authorization": "YOUR_TOKEN"
    },
    "processData": false,
    "data": `{\n    \"in\": ${switchValue}\n}`
    }   

    $.ajax(switchLED).done((res) => {

    });
    getData();


}

function getData() {

    var ledState = {
    "async": true,
    "crossDomain": true,
    "url": "API_URL",
    "method": "GET",
    "headers": {
        "Content-Type": "application/json",
        "Authorization": "YOUR_TOKEN"
    },
    "processData": false,
    "data": ""
    }

    $.ajax(ledState).done((res) => {
        if (slider.checked != !res.out) {
            
            slider.checked = !res.out;

        } 
        if (!res.out) {
            ledOn.hidden = false;
            ledOff.hidden = true;

        } else {

            ledOn.hidden = true;
            ledOff.hidden = false;

        }
    });
}

function callWeather(temperature) {

    var weather = {
    "async": true,
    "crossDomain": true,
    "url": "API_URL",
    "method": "POST",
    "headers": {
        "Content-Type": "application/json",
        "Authorization": "YOUR_TOKEN"
    },
    "processData": false,
    "data": `{\n    \"in\": ${temperature}\n}`
    }   

    $.ajax(weather).done((res) => {

    });



}

function getWeather() {

    request.open('GET', 'http://api.openweathermap.org/data/2.5/forecast?id=6322598&APPID=YOUR_KEY&units=metric', true)
    var temperature = request.onload = function () {
        
        weatherData = JSON.parse(this.response);
        document.getElementById("cityName").innerText = `${weatherData.city.name}, ${weatherData.city.country}`;
        document.getElementById("temperature").innerText = `${Math.round(weatherData.list[0].main.temp)}°C`;
        return Math.round(weatherData.list[0].main.temp);
    }
    request.send()
    callWeather(temperature);
    
}