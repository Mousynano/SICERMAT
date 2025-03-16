#ifndef PUNCH_HTML_H
#define PUNCH_HTML_H

const char punch_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">


<head>
   <meta charset="UTF-8">
   <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <title>SICERMAT Dashboard</title>
   <script src="https://cdn.tailwindcss.com"></script>
   <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/css/bootstrap.min.css" rel="stylesheet" />
   <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css" rel="stylesheet" />
   <script src="https://bernii.github.io/gauge.js/dist/gauge.min.js"></script>
   <link rel="stylesheet" href="../CSS/style.css">
</head>

<body>
   <section id="punch-content">
      <div class="container dashboard-container" id="punch-training">
         <h1 class=" dashboard-title boxing-title">
            Punching Exercise Dashboard</h1>
         <h4 class="text-center text-white mb-10 dashboard-subtitle">(Place device on the wrist)</h4>
         <section id="control-section">
            <div class="text-white">
               <button id="str-punch" class="str-btn">Start</button>
               <button id="rst-punch" class="rst-btn">Reset </button>
            </div>
         </section>

         <div class="row cards-container">
            <div class="col-md-3">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-white">Punch Count</h5>
                     <p id="punchCount" class="card-text">0</p>
                     <img src="../icon/punch.svg" alt="boxingicon" class="w-[150px] mt-[30px]" />
                  </div>
               </div>
            </div>

            <div class="col-md-3">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title">Punch Power</h5>
                     <h5 id="punchPower" class="card-text">0</h5>
                     <canvas id="punchPowerGauge" class="mt-4" height="165" width="365"></canvas>
                  </div>
               </div>
            </div>

            <div class="col-md-3">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-red-300">Retraction Power</h5>
                     <h5 id="retractionTime" class="card-text">0</h5>
                     <canvas id="retractionTimeGauge" class="mt-4" height="165" width="365"></canvas>
                  </div>
               </div>
            </div>

            <div class="col-md-3">
               <div class="card carddata">
                  <div class="card-body justify-center gap-[5px]">
                     <h5 class="card-title text-green-300">Max Power</h5>
                     <p id="maxPower" class="card-text mb-[5px]">0</p>
                     <h5 class="card-title text-yellow-200">Avg Power</h5>
                     <p id="avgPower" class="card-text">0</p>
                  </div>
               </div>
            </div>
         </div>

         <div class="row mt-5">
            <div class="col-md-12 graph-container">
               <div class="card">
                  <div class="card-body">
                     <h5 class="graph-title text-white">Power Graph</h5>
                     <div class="chart-container">
                        <canvas id="powerChart"></canvas>
                     </div>
                  </div>
               </div>
            </div>
         </div>
      </div>

      <div>
         <button id="export-punch">Export Punching Graph</button>
      </div>

   </section>

   <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
   <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
   <script src="https://code.jscharting.com/latest/jscharting.js"></script>
   <script type="text/javascript" src="https://code.jscharting.com/latest/modules/types.js"></script>
   <!-- <script src="../JS/websocket.js"></script> -->
   <!-- <script type="module" src="../JS/features/punch.js"></script> -->
</body>

</html>

)rawliteral";

#endif