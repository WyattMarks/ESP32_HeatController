var chart = new Highcharts.Chart({
    chart:{ renderTo : 'chart-temperature' },
    title: { text: 'Plate Temperature' },
    series: [{
        name: 'Real Temperature',
        showInLegend: true,
        data: [],
        color: '#059e8a'
    }, {
        name: 'Goal Temperature',
        showInLegend: true,
        data: [],
        color: '#3477eb'
    }, {
        name: 'Relay On',
        showInLegend: true,
        data: [],
        color: '#c23b22'
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
    },
    credits: {
        enabled: false
    }});

var slider = document.getElementById("setTemperature");
var output = document.getElementById("slideOutput");
output.innerHTML = slider.value;

slider.oninput = function() {
    output.innerHTML = this.value;

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            //console.log("success")
        }
    };
    xhttp.open("POST", "/set", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    var params = "temperature=" + this.value;
    xhttp.send(params);
} 

setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var d = (new Date()).getTime();
            var data = JSON.parse(this.responseText);
            var s = chart.series[0]

            s.addPoint({x: d, y: data.temperature}, true, s.data.length > 80, true);

            s = chart.series[1]
            s.addPoint({x: d, y: data.relay ? parseInt(slider.value) : 0}, true, s.data.length > 80, true);

            s = chart.series[2]
            s.addPoint({x: d, y: data.relay ? 75 : 0}, true, s.data.length > 80, true);
        }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
}, 1500 ) ;




var button = document.getElementById("turnonbutton");
button.addEventListener('click', (e => {
    e.preventDefault();

    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText == "on") {
                button.style = 'background: #c23b22;';
                button.innerHTML = "Turn off";
            } else {
                button.style = 'background: #059e8a;';
                button.innerHTML = "Turn on";
            }
        }
    };
    xhttp.open("GET", "/power", true);
    xhttp.send();
}));

var loadInitialState = new XMLHttpRequest();
loadInitialState.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        var d = (new Date()).getTime();
        var data = JSON.parse(this.responseText);
        if (data.running) {
            button.style = 'background: #c23b22;';
            button.innerHTML = "Turn off";
        } else {
            button.innerHTML = "Turn on";
            button.style = 'background: #059e8a;';
        }
    }
};
loadInitialState.open("GET", "/temperature", true);
loadInitialState.send();
