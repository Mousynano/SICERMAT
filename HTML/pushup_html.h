#ifndef PUSHUP_HTML_H
#define PUSHUP_HTML_H

const char pushup_html[] PROGMEM = R"rawliteral(
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
   <section id="pushup-content">
      <div class="container dashboard-container" id="pushup-training">
         <h1 class=" dashboard-title boxing-title">Push Up Exercise Dashboard</h1>
         <h4 class="dashboard-subtitle text-center text-white mb-10">(Place device on the Upper Arm)</h4>
         <section id="control-section">
            <div class="text-white">
               <button id="str-pushup" class="str-btn">Start</button>
               <button id="rst-pushup" class="rst-btn">Reset </button>
            </div>
         </section>

         <div class="row cards-container">
            <div class="col-md-3 ">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-white">Push Up Count</h5>
                     <p id="pushupCount" class="card-text">0</p>
                     <img src="../icon/pushup.svg" alt="pushupicon" class="w-[130px] mt-[40px]" />
                  </div>
               </div>
            </div>

            <div class="col-md-3">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-yellow-300">Push Up Speed</h5>
                     <div class="pushupSpeedItem">
                        <h5 id="pushupSpeed" class="card-text text-center mt-[50px]">0</h5>
                        <p class="text-white text-center text-[0.8rem] mt-[40px]">pushups / 3 sec</p>
                        <canvas id="pushupGauge" class="mt-[-90px]"></canvas>
                     </div>
                  </div>
               </div>
            </div>

            <div class="col-md-3">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-red-300 ">Max Pushup</h5>
                     <h5 id="maxPushup" class="card-text text-[3rem] self-center mt-[3rem] text-center">0</h5>
                  </div>
               </div>
            </div>

            <div class="col-md-3">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-green-300">Timer</h5>
                     <div class="flex flex-col h-[100%] gap-5 justify-evenly">
                        <h4 id="timer" class="card-text">00:00</h4>
                     </div>
                  </div>
               </div>
            </div>

         </div>

         <div class="row mt-5 graph-container">
            <div class="col-md-12">
               <div class="card">
                  <div class="card-body">
                     <h5 class="text-white graph-title">Push Up Graph</h5>
                     <div class="chart-container">
                        <canvas id="pushupChart"></canvas>
                     </div>
                  </div>
               </div>
            </div>
         </div>
      </div>

      <div>
         <button id="export-pushup">Export Push Up Graph</button>
      </div>
   </section>


   <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
   <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
   <script src="https://code.jscharting.com/latest/jscharting.js"></script>
   <script type="text/javascript" src="https://code.jscharting.com/latest/modules/types.js"></script>
   <!-- <script src="../JS/websocket.js"></script> -->
   <!-- <script type="module" src="../JS/features/pushup.js"></script> -->
</body>

</html>

)rawliteral";

#endif