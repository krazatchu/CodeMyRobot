/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "prototypes.h"

extern bool wifiEnabled;
extern bool apMode;
extern char* ssid;
extern char* password;

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Set LED GPIO
const int ledPin = 33;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t * data, size_t len){
 
  if(type == WS_EVT_CONNECT){
 
    Serial.println("Websocket client connection received");
    client->text("Hello from ESP32 Server");
 
  } else if(type == WS_EVT_DATA){
    String dataReceived;
    Serial.println("Data received");
    
    for(int i=0; i < len; i++) {
          dataReceived += (char) data[i];
    }
    Serial.println(dataReceived);

    if (dataReceived == "hello") {
      client->text("Hello from ESP32 Server");
    }
  
  } else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
  }
}

void setupWifi() {
  if (wifiEnabled) {
    if (apMode) {
      // Connect to Wi-Fi network with SSID and password
      Serial.print("Setting AP (Access Point)…");
      // Remove the password parameter, if you want the AP (Access Point) to be open
      WiFi.softAP(ssid, password);
    
      IPAddress IP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(IP);
    }
  
    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  
    // Initialize WebSocket
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
  
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
  
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    
    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/style.css", "text/css");
    });
  
    // Go forward
    server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request){    
      moveRobot(5, 40, 1);
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    
    // Turn left
    server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request){
      turnRobot(3, 40, -1);
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    // Turn right
    server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request){
      turnRobot(3, 40, 1);
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    // Go backward
    server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request){
      moveRobot(5, 40, -1);
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });

    // WebSocket test page
    server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/test.html", String(), false, processor);
    });
  
    // Start server
    server.begin();
  }
}
