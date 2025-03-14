#ifndef WEBSOCKET_JS_H
#define WEBSOCKET_JS_H

const char websocket_js[] PROGMEM = R"rawliteral(
class WebSocketClient {
    constructor(url) {
        this.url = url;
        this.ws = null;
        this.listeners = {};
        this.reconnectInterval = 2000; // Waktu tunggu sebelum reconnect
        this.i = 0
    }

    connect() {
        this.ws = new WebSocket(this.url);

        this.ws.onopen = () => {
            console.log("WebSocket Connected");
            this.emit("open");
        };

        this.ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                console.log(`${JSON.stringify(data)}: ${this.i++}`);
                this.emit("message", data);
            } catch (error) {
                console.error("Error parsing WebSocket message:", error);
            }
        };

        this.ws.onclose = () => {
            console.warn("WebSocket Disconnected, retrying...");
            this.emit("close");
            setTimeout(() => this.connect(), this.reconnectInterval);
        };

        this.ws.onerror = (error) => {
            console.error("WebSocket Error:", error);
            this.ws.close(); // Tutup koneksi agar bisa reconnect
        };
    }

    on(event, callback) {
        this.listeners[event] = callback;
    }

    emit(event, data) {
        if (this.listeners[event]) {
            this.listeners[event](data);
        }
    }

    send(data) {
        if (this.ws && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(JSON.stringify(data));
        }
    }
}
 
 export default WebSocketClient;
 
)rawliteral";

#endif