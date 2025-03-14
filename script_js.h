#ifndef SCRIPT_JS_H
#define SCRIPT_JS_H

const char script_js[] PROGMEM = R"rawliteral(
//NAV BURGER//
var hamburger = document.getElementById('hamburger')
hamburger.addEventListener('click', () => {
   const nav = document.getElementById('nav')
   nav.classList.toggle('openNav')
   hamburger.classList.toggle('isX');
})



//NAV CONTROL//
const contentFrame = document.getElementById('contentFrame');
const punchNav = document.getElementById('punch-nav');
const jumpNav = document.getElementById('jump-nav');
const pushupNav = document.getElementById('pushup-nav');


punchNav.addEventListener('click', function (e) {
   e.preventDefault();
   contentFrame.src = './HTML/punch.html';
});
jumpNav.addEventListener('click', function (e) {
   e.preventDefault();
   contentFrame.src = './HTML/jump.html';
});
pushupNav.addEventListener('click', function (e) {
   e.preventDefault();
   contentFrame.src = './HTML/pushup.html';
});



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

function fetchHrSpoData() {
   if (fetchHrSpo) {

      heartValue = Math.floor(Math.random() * 71) + 60;
      spoValue = Math.floor(Math.random() * 21) + 80;

      heartRateGauge.set(heartValue);
      spo2Gauge.set(spoValue);

      $('#heartRateText').text(`${heartValue} BPM`);
      $('#spo2Text').text(`${spoValue}%`);

   }
}
setInterval(fetchHrSpoData, 1000);
///////////////////////////////

)rawliteral";

#endif