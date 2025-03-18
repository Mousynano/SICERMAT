#ifndef HEART_JS_H
#define HEART_JS_H

const char heart_js[] PROGMEM = R"rawliteral(

//HR AND SPO//
var heartValue = 0
var spoValue = 0
var fetchHrSpo = true

var heartRateGauge = new Gauge(document.getElementById('heartRateGauge')).setOptions({
   angle: -0.5,
   lineWidth: 0.1,
   radiusScale: 1.1,
   pointer: {
      length: 0,
      strokeWidth: 0,
      color: 'red'
   },
   limitMax: 'true',
   highDpiSupport: true,
   colorStart: '#ff0000',
   colorStop: '#ff6347',
   strokeColor: '#E0E0E0',
   generateGradient: true
});
heartRateGauge.maxValue = 200;
heartRateGauge.setMinValue(0);
heartRateGauge.set(0);
var spo2Gauge = new Gauge(document.getElementById('spo2Gauge')).setOptions({
   angle: -0.5,
   lineWidth: 0.1,
   radiusScale: 1.1,
   pointer: {
      length: 0,
      strokeWidth: 0,
      color: '#32cd32'
   },
   limitMax: 'true',
   highDpiSupport: true,
   colorStart: '#0000ff',
   colorStop: '#1e90ff',
   strokeColor: '#E0E0E0',
   generateGradient: true
});
spo2Gauge.maxValue = 100;
spo2Gauge.setMinValue(0);
spo2Gauge.set(0);


// UPDATE DATA
export const updateData = (data) => {
   const {heartRate, spo} = data;
   heartRateGauge.set(heartRate);
   spo2Gauge.set(spo);

   $('#heartRateText').text(`${heartRate} BPM`);
   $('#spo2Text').text(`${spo}%`);
}
)rawliteral";

#endif