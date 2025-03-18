#ifndef WEBSOCKET_JS_H
#define WEBSOCKET_JS_H

const char websocket_js[] PROGMEM = R"rawliteral(
class WebSocketClient {
    constructor(url) {
        this.url = url;
        this.ws = null;
        this.reconnectInterval = 2000;
        this.maxRetries = 10;
        this.retryCount = 0;
        this.i = 0;
        this.handlers = {};
    }

    connect() {
        if (this.retryCount >= this.maxRetries) {
            console.error("Max retries reached. Stopping reconnect attempts.");
            return;
        }

        this.ws = new WebSocket(this.url);

        this.ws.onopen = () => {
            console.log("WebSocket Connected");
            this.retryCount = 0;
        };

        this.ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                console.log(`Message received: ${JSON.stringify(data)}: ${this.i++}`);
                console.log(`Ongoing handlers: ${JSON.stringify(this.handlers)}`);

                if (data.type && this.handlers[data.type]) {
                    this.handlers[data.type](data);
                    this.ws.send("ACK");
                } else {
                    console.warn("Unhandled message type:", data.type);
                }
            } catch (error) {
                console.error("Error parsing WebSocket message:", error);
            }
        };

        this.ws.onclose = () => {
            console.warn("WebSocket Disconnected, retrying...");
            this.retryCount++;
            setTimeout(() => this.connect(), this.reconnectInterval);
        };

        this.ws.onerror = (error) => {
            console.error("WebSocket Error:", error);
            this.ws.close();
        };
    }

    send(data) {
        if (this.ws && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(JSON.stringify(data));
        }
    }

    addHandler(type, callback) {
        this.handlers[type] = callback;
    }

    removeHandler(type) {
        delete this.handlers[type];
    }
}

// Singleton WebSocket Client
export const client = new WebSocketClient("ws://sicermat.local/ws");
// client.connect();

)rawliteral";

#endif