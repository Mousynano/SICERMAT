#ifndef PUSHUP_JS_H
#define PUSHUP_JS_H

const char pushup_js[] PROGMEM = R"rawliteral(
//1) VARIABLES//
var pushUpCount = 0
var pushUpSet = 0
var maxPushup = 0;
var timeElapsedPushup = 0
var allPushupData = []
var isRunning = false;
var time = 0;
var fetchPushup = false

//2) GAUGES//
var pushupGauge = new Gauge(document.getElementById('pushupGauge')).setOptions({
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
   colorStart: '#ffff00',
   colorStop: '#ffffe0',
   strokeColor: '#E0E0E0',
   generateGradient: true
});
pushupGauge.maxValue = 100;
pushupGauge.setMinValue(0);
pushupGauge.set(0);

//3) CHARTS//
const ctx_pushup = document.getElementById('pushupChart').getContext('2d');
const pushupChart = new Chart(ctx_pushup, {
   type: 'line',
   data: {
      labels: [],
      datasets: [{
         label: 'Push Up Speed',
         data: [],
         borderColor: '#ffff00',
         borderWidth: 1,
         fill: false,
      }],
   },
   options: {
      responsive: true,
      scales: {
         y: {
            beginAtZero: true,
            ticks: {
               color: 'white'
            },
            grid: {
               color: 'rgba(255, 255, 255, 0.2)'
            }
         },
         x: {
            ticks: {
               color: 'white'
            },
            grid: {
               color: 'rgba(255, 255, 255, 0.2)'
            }
         }
      },
      plugins: {
         legend: {
            labels: {
               color: 'white'
            }
         }
      }
   }
});
document.getElementById("export-pushup").addEventListener("click", function () {
   let csvContent = "Time, Total(push ups/ 3sec)\n";
   allPushupData.forEach(entry => {
      csvContent += `${entry.time},${entry.speed}\n`;
   });

   const blob = new Blob([csvContent], { type: "text/csv" });
   const url = URL.createObjectURL(blob);

   const link = document.createElement("a");
   link.href = url;
   link.download = "all_pushup_data.csv";
   link.click();

   URL.revokeObjectURL(url);
});

//4) TIMER FUNCTIONALITY//
let timerElement = document.getElementById("timer");
let timerInterval;

function formatTime(seconds) {
   let minutes = Math.floor(seconds / 60).toString().padStart(2, '0');
   let secs = (seconds % 60).toString().padStart(2, '0');
   return `${minutes}:${secs}`;
}
function updateTimer() {
   timerElement.textContent = formatTime(time);
}


//5) FETCHING DATA //
export const updateData = () => {
   if (fetchPushup) {
      let pushUpOneSec = Math.floor(Math.random() * 3) + 1;
      pushUpCount += pushUpOneSec
      pushUpSet += pushUpOneSec

      $('#pushupCount').text(`${pushUpCount}`);

      timeElapsedPushup++;
      if (timeElapsedPushup > 30) {
         pushupChart.data.labels.shift();
         pushupChart.data.datasets.forEach(dataset => {
            dataset.data.shift();
         });
      }

      if (timeElapsedPushup % 3 === 0) {
         pushupGauge.set(pushUpSet);
         $('#pushupSpeed').text(`${pushUpSet}`);

         maxPushup = Math.max(maxPushup, pushUpSet);
         $('#maxPushup').text(`${maxPushup}`);

         pushupChart.data.labels.push(`${timeElapsedPushup} Sec`);
         pushupChart.data.datasets[0].data.push(pushUpSet);
         pushupChart.update();

         allPushupData.push({ time: `Time ${timeElapsedPushup}`, speed: pushUpSet });
         pushUpSet = 0
      }

   }
}

//6) START AND RESET BUTTONS//
var strPushup = document.getElementById('str-pushup')
var rstPushup = document.getElementById('rst-pushup')
strPushup.addEventListener('click', () => startExercise())
rstPushup.addEventListener('click', () => resetExercise())
function startPauseTime() {
   if (isRunning) {
      clearInterval(timerInterval);
   } else {
      timerInterval = setInterval(() => {
         time++;
         updateTimer();
      }, 1000);
   }
   isRunning = !isRunning;
}
function resetTime() {
   clearInterval(timerInterval);
   time = 0;
   isRunning = false;
   updateTimer();
}
function startExercise() {
   fetchPushup = !fetchPushup
   strPushup.textContent = fetchPushup ? "Pause" : "Start"
   startPauseTime()
}
function resetExercise() {
   pushUpCount = 0
   pushUpSet = 0
   maxPushup = 0;
   timeElapsedPushup = 0
   fetchPushup = false

   $('#pushupCount').text('0');
   $('#maxPushup').text('0');
   $('#pushupSpeed').text('0');

   pushupGauge.set(0);
   pushupChart.data.labels = [];
   pushupChart.data.datasets.forEach(dataset => {
      dataset.data = [];
   });
   pushupChart.update();
   strPushup.textContent = "Start"

   resetTime()

}
)rawliteral";

#endif