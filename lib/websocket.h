#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <unordered_map>
#include <ArduinoJson.h>
#include <AsyncTCP.h>

#define MAX_QUEUE_SIZE 20

AsyncWebSocket ws("/ws");

struct ClientData {
    AsyncWebSocketClient* client;
    size_t queueSize;
};

std::unordered_map<uint32_t, ClientData> clients;

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
    uint32_t clientID = client->id();

    if (type == WS_EVT_CONNECT) {
        Serial.printf("Client %u connected\n", clientID);
        clients[clientID] = {client, 0};  // Tambahkan client ke daftar
    } 
    else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("Client %u disconnected\n", clientID);
        client->close();
        clients.erase(clientID);  // Hapus dari daftar
    } 
    else if (type == WS_EVT_DATA) {
        // aku bingung caranya menampilkan data saat ada ACK gimana
        String message = String((char*)data, len);  // Pastikan baca sesuai panjang data
        message.trim();  // Hapus karakter tambahan seperti \r atau \n
        
        // Tampilkan pesan ACK dan antrean client dengan benar
        Serial.printf("Received ACK from Client %u, message: %s, client queue: %u\n", clientID, message.c_str(), clients[clientID].queueSize);
        if (message == "ACK") {
            clients[clientID].queueSize = 0;  // Reset antrean client yang mengirim ACK
            Serial.printf("Resetting Client %u, get ACKed queue size: %u\n", clientID, clients[clientID].queueSize);
        }
    }
}

// void resetQueueTask(void *parameter) {
//     while (true) {
//         for (auto& entry : clients) {
//             Serial.printf("Resetting Client %u queue size: %u\n", entry.first, entry.second.queueSize);
//             ClientData& clientData = entry.second;
//             if (clientData.queueSize > 0) {
//                 clientData.queueSize--; // Kurangi buffer jika tidak ada ACK
//             }
//         }
//         vTaskDelay(pdMS_TO_TICKS(250)); // Reset setiap 2 detik
//     }
// }

void sendJsonToClient(ClientData &clientData, uint32_t clientId, const String &jsonData, const char *featureName) {
    if (clientData.queueSize < MAX_QUEUE_SIZE) {
        clientData.client->text(jsonData);
        clientData.queueSize++;
        Serial.printf("Sent %s data to Client %u\n", featureName, clientId);
    } else {
        Serial.printf("Client %u queue full (%s), skipping...\n", clientId, featureName);
    }
}

// void sendDataTask(void *parameter) {
//     while (true) {
//         String punchJson = "{\"type\": \"punch\", \"punchPower\": " + String(punchPower) + ", \"retractionTime\": " + String(retractionTime) + "}";
//         String jumpJson = "{\"type\": \"jump\", \"jumpHeight\": " + String(jumpHeight) + "}";
//         String pushUpJson = "{\"type\": \"pushUp\"}";

//         for (auto &entry : clients) {
//             ClientData &clientData = entry.second;

//             // Gunakan fungsi sendJsonToClient agar lebih modular
//             sendJsonToClient(clientData, entry.first, punchJson, "punch");
//             sendJsonToClient(clientData, entry.first, jumpJson, "jump");
//             sendJsonToClient(clientData, entry.first, pushUpJson, "pushUp");
//         }

//         vTaskDelay(pdMS_TO_TICKS(100)); 
//     }
// }

#endif