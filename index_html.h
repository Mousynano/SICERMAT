#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char index_html[] PROGMEM = R"rawliteral(
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
   <link rel="stylesheet" href="/style.css">
</head>

<body>

   <nav id="nav" class="flex flex-col nav h-[100vh] top-0 z-10 text-white w-[200px] bg-black fixed">
      <h1 class=" mt-[100px] text-center text-[1rem] font-light bg-slate-500 py-2">Sistem Cerdas Monitoring Atlet
         Terpadu</h1>
      <div class="menu flex flex-col justify-center items-center font-light mt-[8rem] gap-4">
         <a id="punch-nav">Punching</a>
         <a id="jump-nav">Vertical Jump</a>
         <a id="pushup-nav">Push Up</a>
         <a href="https://youtube.com/shorts/AFWKHaOkh20?feature=shared" target="_blank">About</a>
      </div>
   </nav>

   <div id="hamburger" class="text-[1.5rem] text-white left-1 bg-blue top-2">
      <span></span>
      <span></span>
      <span></span>
   </div>



   <iframe id="contentFrame" src="/HTML/punch.html" frameborder="0"
      class="h-[150vh] overflow-x-hidden w-full"></iframe>


   <section id="fuzzy-heart">
      <h1 id="fuzzy" class="text-center mb-10 mt-20 text-[1.6rem] font-bold text-white">Athlete Status: Normal</h1>
      <div id="hr-spo" class="row mt-5 justify-center mb-10">

         <div class="col-md-3">
            <div class="card carddata">
               <div class="card-body">
                  <div class="flex justify-center items-center gap-2">
                     <img src="./icon/hr.svg" alt="hrIcon" class="w-[30px] pb-[10px]" />
                     <h5 class="card-title text-red-200">Heart Rate</h5>
                  </div>
                  <canvas id="heartRateGauge"></canvas>
                  <h5 id="heartRateText" class="card-text mt-[-88px] text-[1.1rem]">0</h5>
               </div>
            </div>
         </div>

         <div class="col-md-3">
            <div class="card carddata">
               <div class="card-body">
                  <div class="flex justify-center items-center gap-1">
                     <img src="./icon/spo.svg" alt="spoIcon" class="w-[30px] pb-[10px]" />
                     <h5 class="card-title">Spo2</h5>
                  </div>
                  <canvas id="spo2Gauge">
                  </canvas>
                  <h5 id="spo2Text" class="card-text mt-[-92px] text-[1.5rem]">0%</h5>
               </div>
            </div>
         </div>

      </div>
   </section>


   <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
   <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
   <script src="https://code.jscharting.com/latest/jscharting.js"></script>
   <script type="text/javascript" src="https://code.jscharting.com/latest/modules/types.js"></script>


   <script src="/main.js"></script>
</body>

</html>
)rawliteral";

#endif