#ifndef JUMP_HTML_H
#define JUMP_HTML_H

const char jump_html[] PROGMEM = R"rawliteral(
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
   <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
   <script src="https://cdn.jsdelivr.net/npm/chartjs-plugin-zoom@1.2.0/chartjs-plugin-zoom.min.js"></script>
   <link rel=" stylesheet" href="/style.css">
</head>

<body>
   <section id="jump-content">
      <div class="container dashboard-container" id="jump-training">
         <h1 class="dashboard-title ">
            Vertical Jump Exercise Dashboard</h1>
         <h4 class="dashboard-subtitle text-center text-white mb-10">(Place device on the ankles)</h4>
         <section id="control-section">
            <div class="text-white">
               <button id="str-jump" class="str-btn">Start</button>
               <button id="rst-jump" class="rst-btn">Reset </button>
            </div>
         </section>

         <div class="row cards-container">
            <div class="col-md-4">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-white">Jump Count</h5>
                     <p id="jumpCount" class="card-text">0</p>
                     <img src="/icon/jump.svg" alt="boxingicon" class="w-[150px] mt-[25px]" />
                  </div>
               </div>
            </div>

            <div class="col-md-4">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-cyan-300">Jump Height</h5>
                     <div class="flex max-w-[200px] w-[100%] mt-[-5px] justify-around items-center">
                        <div id="jumpHeightGauge" class="my-0 items-center justify-center flex w-[50%]"></div>
                        <div class="card-text my-0 text-[1.5rem] flex flex-col gap-2">
                           <p id="jumpHeight">0 m</p>
                           <p id="jumpCm">0 cm</p>
                        </div>
                     </div>
                  </div>
               </div>
            </div>

            <div class="col-md-4 centered-grid">
               <div class="card carddata">
                  <div class="card-body">
                     <h5 class="card-title text-green-300 ">Max Height</h5>
                     <h5 id="maxHeight" class="card-text text-[3rem] self-center mt-[3rem] text-center">0</h5>
                  </div>
               </div>
            </div>


         </div>

         <div class="row mt-5">
            <div class="col-md-12 graph-container">
               <div class="card">
                  <div class="card-body">
                     <h5 class="card-title text-white graph-title">Height Graph</h5>
                     <div class="chart-container">
                        <canvas id="heightChart"></canvas>
                     </div>
                  </div>
               </div>
            </div>
         </div>
      </div>

      <div>
         <button id="export-jump">Export Jump Graph</button>
      </div>

   </section>



   <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
   <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
   <script src="https://code.jscharting.com/latest/jscharting.js"></script>
   <script type="text/javascript" src="https://code.jscharting.com/latest/modules/types.js"></script>
   <!-- <script src="../JS/websocket.js"></script> -->
   <script type="module" src="../JS/features/jump.js"></script>
</body>

</html>
)rawliteral";

#endif