#include <Arduino.h>

const char landing_page_html[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://code.highcharts.com/highcharts.js"></script>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/gh/WyattMarks/ESP32_HeatController/ESP32_HeatController_Firmware/src/style.css">
</head>
<body>
    <h2>ESP32 Reflow Plate</h2>
    <div id="chart-temperature" class="container"></div>
    <div class="slide-container">
        <input type="range" min="1" max="300" value="100" class="slider" id="setTemperature">
        <h4>Set Temperature: <span id="slideOutput"></span>&deg;C</h4>
    </div>
    <div class="button-container">
        <a href="#" class="button" id="turnonbutton"></a>
    </div>
</body>
<script src="https://cdn.jsdelivr.net/gh/WyattMarks/ESP32_HeatController/ESP32_HeatController_Firmware/src/reflow.js"></script>
</html>
)=====";