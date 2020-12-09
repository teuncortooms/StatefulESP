
window.addEventListener('load', onLoad);
function onLoad(event) {
    initWebSocket();
    initButton();
}

// WEBSOCKET
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}
function onOpen(event) {
    console.log('Connection opened');
}
function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

// MESSAGE HANDLER
function onMessage(event) {
    let msg = JSON.parse(event.data);
    if (msg.type == "LEDSTATE") {
        updateLedState(msg.value);
    }
    if (msg.type == "LDRVALUE") {
        updateLdrChart(msg.value);
    }
}

// LED CONTROL
function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
}
function toggle() {
    websocket.send('TOGGLE_LED');
}
function updateLedState(value) {
    var state;
    if (value == "1") {
        state = "ON";
    }
    else {
        state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
}

// LDR CHART
var chartT = new Highcharts.Chart({
    chart: { 
        renderTo: 'chart-ldr'
    },
    title: { text: 'LDR' },
    series: [{
        showInLegend: false,
        data: []
    }],
    plotOptions: {
        line: {
            animation: false,
            dataLabels: { enabled: false }
        },
        series: { color: '#059e8a' }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: { text: 'Brightness (max 4095)' }
    },
    credits: { enabled: false }
});

function updateLdrChart(value) {
    var x = (new Date()).getTime();
    var y = parseFloat(value);
    if (chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
    } else {
        chartT.series[0].addPoint([x, y], true, false, true);
    }
}