#ifndef WEBSOCKET_JS_H
#define WEBSOCKET_JS_H

const char websocket_js[] PROGMEM = R"rawliteral(
class WebSocketClient {
    constructor(url) {
        this.url = url;
        this.ws = null;
        this.listeners = {}; // Menyimpan event listener aktif
        this.reconnectInterval = 2000;
        this.i = 0;
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
                console.log("Checking message type:", data.type);
                console.log("Available listeners: ", JSON.stringify(this.listeners));

                if (data.type && this.listeners[data.type]) {
                    this.emit(data.type, data);
                } else {
                    console.warn("Received message with unknown type:", data.type);
                }

            } catch (error) {
                console.log(`${JSON.stringify(event.data)}: ${this.i++}`);
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
            this.ws.close();
        };
    }

    on(event, callback) {
        if (!this.listeners[event]) {
            this.listeners[event] = [];
        }
        this.listeners[event].push(callback);
    }

    off(event) {
        delete this.listeners[event];
    }

    emit(event, data) {
        if (this.listeners[event]) {
            this.listeners[event].forEach(callback => callback(data));
        }
    }

    send(data) {
        if (this.ws && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(JSON.stringify(data));
        }
    }
}


// Singleton WebSocket Client
export const client = new WebSocketClient("ws://esp32-server.local/ws");
export const registerListener = (event, verifier, callback) => {
    client.on(event, (data) => {
        if (verifier.every(key => data[key] !== undefined)) {
            callback(data);
        }else{
            console.warn("Received unexpected data:", data);
        }
        client.send('ACK')
    })
}
client.connect();
)rawliteral";

#endif