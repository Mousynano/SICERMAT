#ifndef SCRIPT_JS_H
#define SCRIPT_JS_H

const char script_js[] PROGMEM = R"rawliteral(
import { updateDataHeart } from "./features/heart.js";
import { client, registerListener } from "./websocket.js";

//NAV BURGER//
const hamburger = document.getElementById('hamburger')
hamburger.addEventListener('click', () => {
   const nav = document.getElementById('nav')
   nav.classList.toggle('openNav')
   hamburger.classList.toggle('isX');
});

//NAV CONTROL//
const contentFrame = document.getElementById('contentFrame');
const punchNav = document.getElementById('punch-nav');
const jumpNav = document.getElementById('jump-nav');
const pushupNav = document.getElementById('pushup-nav');

// FEATURE CONTROL
const featureScripts = {
   "punch": "./features/punch.js",
   "jump": "./features/jump.js",
   "pushup": "./features/pushup.js"
};

let currentFeature = null;
let currentFetchData = null; 

async function loadFeature(feature) {
   try {
      contentFrame.src = `./HTML/${feature}.html`;

      if (currentFeature && currentFetchData) {
         client.off(currentFeature, currentFetchData);
      }

      if (featureScripts[feature]) {
         try {
            const module = await import(featureScripts[feature]).then(module => console.log(JSON.stringify(module))); 
            const { verifier, fetchData } = module;

            if (typeof fetchData === "function") {
               currentFeature = feature; 
               currentFetchData = fetchData; 

               // client.on(feature, fetchData); 
               registerListener(feature, verifier, fetchData);
               console.log(`Loaded script for ${feature}`);
            } else {
               console.error(`fetchData function not found in ${featureScripts[feature]}`);
            }
         } catch (error) {
            console.error(`Failed to load script for ${feature}.js`, error);
         }
      }
   } catch (error) {
      console.error(`Failed to load ${feature}.html`, error);
   }
}

loadFeature("punch");
registerListener("heart", ["heartRate", "spo"], updateDataHeart);

// BUTTON EVENT LISTENERS
punchNav.addEventListener('click', function (e) {
   e.preventDefault();
   loadFeature('punch');
});
jumpNav.addEventListener('click', function (e) {
   e.preventDefault();
   loadFeature('jump');
});
pushupNav.addEventListener('click', function (e) {
   e.preventDefault();
   loadFeature('pushup');
});
)rawliteral";

#endif