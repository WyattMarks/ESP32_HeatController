#include <Arduino.h>

const char landing_page_html[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://code.highcharts.com/highcharts.js"></script>
    <style>
        body {
        min-width: 310px;
            max-width: 800px;
            height: 400px;
        margin: 0 auto;
        }
        h2 {
        font-family: Arial;
        font-size: 2.5rem;
        text-align: center;
        }
    </style>
</head>
<body>
    <h2>ESP32 Refloww Plate</h2>
    <div id="chart-temperature" class="container"></div>
</body>
<script>
    var chart = new Highcharts.Chart({
        chart:{ renderTo : 'chart-temperature' },
        title: { text: 'Plate Temperature' },
        series: [{
            name: 'Temperature',
            showInLegend: true,
            data: [],
            color: '#059e8a'
        }, {
            name: 'Relay On',
            showInLegend: true,
            data: [],
            color: '#c21919'
        }],
        plotOptions: {
            line: { animation: true,
                dataLabels: { enabled: false }
            }
        },
        xAxis: { type: 'datetime',
            dateTimeLabelFormats: { second: '%M:%S' }
        },
        yAxis: {
            title: { text: 'Temperature (Celsius)' },
            max: 300
        }
    });

    setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                var d = (new Date()).getTime();
                var data = JSON.parse(this.responseText);
                var s = chart.series[0]

                s.addPoint({x: d, y: data.temperature}, true, s.data.length > 80, true);

                s = chart.series[1]
                s.addPoint({x: d, y: data.relay ? 75 : 0}, true, s.data.length > 80, true);
            }
        };
        xhttp.open("GET", "/temperature", true);
        xhttp.send();
    }, 1500 ) ;
</script>
</html>
)=====";