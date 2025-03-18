#ifndef HTTP_H
#define HTTP_H

#include <ESPAsyncWebServer.h>

// Include Files
#include "../CSS/style_css.h"

#include "../index_html.h"
#include "../HTML/jump_html.h"
#include "../HTML/pushup_html.h"
#include "../HTML/punch_html.h"

#include "../JS/features/heart_js.h"
#include "../JS/features/jump_js.h"
#include "../JS/features/pushup_js.h"
#include "../JS/features/punch_js.h"
#include "../JS/websocket_js.h"
#include "../JS/script_js.h"

#include "../icon/jump_svg.h"
#include "../icon/pushup_svg.h"
#include "../icon/punch_svg.h"
#include "../icon/hr_svg.h"
#include "../icon/spo_svg.h"

AsyncWebServer server(80);

// Definisi struktur untuk menyimpan informasi file
struct Route {
    const char *path;
    const char *mimeType;
    const char *content;
  };

// Daftar file yang disajikan oleh server
Route routes[] = {
  // No content
  {"/favicon.ico", "image/x-icon", ""},

  // CSS Files
  {"/style.css", "text/css", style_css},

  // JavaScript Files
  {"/JS/features/heart.js", "application/javascript", heart_js},
  {"/JS/features/jump.js", "application/javascript", jump_js},
  {"/JS/features/punch.js", "application/javascript", punch_js},
  {"/JS/features/pushup.js", "application/javascript", pushup_js},
  {"/JS/websocket.js", "application/javascript", websocket_js},
  {"/JS/script.js", "application/javascript", script_js},

  // HTML Files
  {"/", "text/html", index_html},
  {"/HTML/jump.html", "text/html", jump_html},
  {"/HTML/punch.html", "text/html", punch_html},
  {"/HTML/pushup.html", "text/html", pushup_html},

  // Icon Files
  {"/icon/hr.svg", "image/svg+xml", hr_svg},
  {"/icon/spo.svg", "image/svg+xml", spo_svg},
  {"/icon/punch.svg", "image/svg+xml", punch_svg},
  {"/icon/pushup.svg", "image/svg+xml", pushup_svg},
  {"/icon/jump.svg", "image/svg+xml", jump_svg},
};

// Fungsi untuk mendaftarkan semua route
void registerRoutes(AsyncWebServer &server) {
    for (Route &route : routes) {
        server.on(route.path, HTTP_GET, [route](AsyncWebServerRequest *request) {
            request->send(200, route.mimeType, route.content);
        });
    }
  }

#endif