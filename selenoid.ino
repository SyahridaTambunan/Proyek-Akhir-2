#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Konfigurasi WiFi
const char* ssid = "putra123";
const char* password = "123putra";

// Konfigurasi pin
const int buttonPin1 = 5;     // Push button untuk pintu 1 (D1)
const int buttonPin2 = 4;     // Push button untuk pintu 2 (D2)
const int relayPin1 = 0;      // Pin relay untuk pintu 1 (D3)
const int relayPin2 = 2;      // Pin relay untuk pintu 2 (D4)

// Status pintu
bool door1Open = false;
bool door2Open = false;

ESP8266WebServer server(80);

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  Serial.begin(115200);

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup server routes
  server.on("/", handleRoot);
  server.on("/door1", handleDoor1);
  server.on("/door2", handleDoor2);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();

  if (digitalRead(buttonPin1) == LOW) {
    delay(100); 
    if (digitalRead(buttonPin1) == LOW) {
      toggleDoor(1);
      delay(100); 
    }
  }

  if (digitalRead(buttonPin2) == LOW) {
    delay(100); 
    if (digitalRead(buttonPin2) == LOW) {
      toggleDoor(2);
      delay(100); 
    }
  }
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Control Doorlock By Webpage</h1>";
  html += "<h2>Door 1</h2>";
  html += "<p>Status: " + String(door1Open ? "Open" : "Closed") + "</p>";
  html += "<p><a href='/door1'><button>Buka Pintu 1</button></a></p>";
  html += "<p><a href='/door1'><button>Tutup Pintu 1</button></a></p>";
  html += "<h2>Door 2</h2>";
  html += "<p>Status: " + String(door2Open ? "Open" : "Closed") + "</p>";
  html += "<p><a href='/door2'><button>Buka Pintu 2</button></a></p>";
  html += "<p><a href='/door2'><button>Tutup Pintu 2</button></a></p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleDoor1() {
  toggleDoor(1);

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDoor2() {
  toggleDoor(2);

  server.sendHeader("Location", "/");
  server.send(303);
}

void toggleDoor(int doorNumber) {
  if (doorNumber == 1) {
    if (door1Open) {
      closeDoor(1);
    } else {
      openDoor(1);
    }
  } else if (doorNumber == 2) {
    if (door2Open) {
      closeDoor(2);
    } else {
      openDoor(2);
    }
  }
}

void openDoor(int doorNumber) {
  if (doorNumber == 1) {
    digitalWrite(relayPin1, HIGH);
    door1Open = true;
    Serial.println("Door 1 opened");
  } else if (doorNumber == 2) {
    digitalWrite(relayPin2, HIGH);
    door2Open = true;
    Serial.println("Door 2 opened");
  }
}

void closeDoor(int doorNumber) {
  if (doorNumber == 1) {
    digitalWrite(relayPin1, LOW);
    door1Open = false;
    Serial.println("Door 1 closed");
  } else if (doorNumber == 2) {
    digitalWrite(relayPin2, LOW);
    door2Open = false;
    Serial.println("Door 2 closed");
  }
}