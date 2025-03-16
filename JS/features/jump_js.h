#ifndef JUMP_JS_H
#define JUMP_JS_H

const char jump_js[] PROGMEM = R"rawliteral(
//1) letIABLES//
let jumpCount = 0
let maxHeight = 0;
let timeElapsedJump = 0
let allJumpData = [];
let fetchJump = false

//2) GAUGES//
let jumpGauge = JSC.chart('jumpHeightGauge', {
   debug: true,
   box: {
      fill: 'transparent'
   },

   defaultSeries_type: 'gauge linear vertical',
   height: 190,
   width: 100,
   yAxis_defaultTick_label_color: 'white',

   yAxis: {
      defaultTick_enabled: false,
      customTicks: [1, 3, 5, 7, 10],
      scale: { range: [1, 10] },
      line: {
         width: 5,
         color: 'smartPalette',
         breaks_gap: 0.03
      },
   },
   legend_visible: false,
   palette: {
      pointValue: '%yValue',
      ranges: [
         { value: 2, color: '#FF5353' },
         { value: 3, color: '#FFD221' },
         { value: 6, color: '#77E6B4' },
         { value: [9, 10], color: '#21D683' }
      ]
   },
   defaultSeries: {
      defaultPoint_tooltip: '%yValue',
      shape_label: {
         text: '%name',
         verticalAlign: 'bottom',
         style_fontSize: 15,
      }
   },
   series: [
      {
         points: [['score', [1, 7]]],
      }
   ]
});

//3) CHARTS//
const ctx_height = document.getElementById('heightChart').getContext('2d');
const jumpChart = new Chart(ctx_height, {
   type: 'line', // Jenis chart
   data: {
      labels: [], // Label untuk X-Axis
      datasets: [{
         label: 'Jump Height', // Label chart
         data: [], // Data untuk chart
         borderColor: 'rgba(75, 192, 192, 1)',
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

document.getElementById("export-jump").addEventListener("click", function () {
   let csvContent = "Time,Jump Height (m)\n";
   allJumpData.forEach(entry => {
      csvContent += `${entry.time},${entry.height}\n`;
   });

   const blob = new Blob([csvContent], { type: "text/csv" });
   const url = URL.createObjectURL(blob);

   const link = document.createElement("a");
   link.href = url;
   link.download = "all_jump_data.csv";
   link.click();

   URL.revokeObjectURL(url);
});

//4) FETCH DATA//
export const updateData = (data) => {
   const { jumpHeight } = data
   jumpCount++

   allJumpData.push({ time: `Time ${timeElapsedJump + 1}`, height: jumpHeight });

   $('#jumpCount').text(`${jumpCount}`);
   $('#jumpHeight').text(`${jumpHeight}m`);
   $('#jumpCm').text(`${jumpHeight * 10} cm`);
   maxHeight = Math.max(maxHeight, jumpHeight);
   $('#maxHeight').text(`${maxHeight} m`);

   jumpGauge.options({
      series: [{
         points: [['score', [1, jumpHeight]]],
      }]
   })

   timeElapsedJump++;
   if (timeElapsedJump > 10) {
      jumpChart.data.labels.shift();
      jumpChart.data.datasets.forEach(dataset => {
         dataset.data.shift();
      });
   }

   jumpChart.data.labels.push(`Time ${timeElapsedJump}`);
   jumpChart.data.datasets[0].data.push(jumpHeight);
   jumpChart.update();
}

export const verifier = ['jumpHeight']

//5) START AND RESET BUTTONS//
let strJump = document.getElementById('str-jump')
let rstJump = document.getElementById('rst-jump')
strJump.addEventListener('click', () => startExercise())
rstJump.addEventListener('click', () => resetExercise())

function startExercise() {
   fetchJump = !fetchJump
   strJump.textContent = fetchJump ? "Pause" : "Start"
}
function resetExercise() {
   jumpCount = 0
   maxHeight = 0;
   timeElapsedJump = 0
   fetchJump = false

   $('#jumpCount').text('0');
   $('#jumpHeight').text(`0 m`);
   $('#jumpCm').text(`0 cm`);
   $('#maxHeight').text(`0 m`);

   jumpGauge.options({
      series: [{
         points: [['score', [1, 1]]],
      }]
   })
   jumpChart.data.labels = [];
   jumpChart.data.datasets.forEach(dataset => {
      dataset.data = [];
   });
   jumpChart.update();
   strJump.textContent = "Start"
}
)rawliteral";

#endif