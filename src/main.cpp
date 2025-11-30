#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Preferences.h>
#include <DisplayManager.h>

// Display manager instance
DisplayManager display;

// Preferences for persistent storage
Preferences preferences;

// WiFi mode configuration
enum WiFiMode {
  MODE_NONE,
  MODE_CLIENT,
  MODE_SERVER
};

WiFiMode currentMode = MODE_NONE;

void saveWiFiMode(WiFiMode mode) {
  preferences.begin("wifi-config", false);
  preferences.putUInt("mode", (uint8_t)mode);
  preferences.end();
}

WiFiMode loadWiFiMode() {
  preferences.begin("wifi-config", true);
  uint8_t mode = preferences.getUInt("mode", MODE_NONE);
  preferences.end();
  return (WiFiMode)mode;
}

void setupWiFiClient() {
  display.showMessage("WiFi Client\nMode", ST77XX_GREEN);
  delay(1000);
  
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(180);
  
  display.showMessage("Connect to:\nMeshSense-AP", ST77XX_YELLOW);
  
  if (!wifiManager.autoConnect("MeshSense-AP", "password")) {
    display.showMessage("WiFi Failed!\nRestarting...", ST77XX_RED);
    delay(3000);
    ESP.restart();
  }
  
  display.showMessage("Connected!", ST77XX_GREEN);
  delay(1000);
  
  // Display connection info
  display.clear();
  delay(100);
  
  char buffer[64];
  display.showMessageAt("WiFi Client Mode", 10, 20, ST77XX_GREEN, 1);
  
  snprintf(buffer, sizeof(buffer), "SSID: %s", WiFi.SSID().c_str());
  display.showMessageAt(buffer, 10, 40, ST77XX_GREEN, 1);
  
  snprintf(buffer, sizeof(buffer), "IP: %s", WiFi.localIP().toString().c_str());
  display.showMessageAt(buffer, 10, 55, ST77XX_GREEN, 1);
  
  snprintf(buffer, sizeof(buffer), "Signal: %d dBm", WiFi.RSSI());
  display.showMessageAt(buffer, 10, 70, ST77XX_GREEN, 1);
  
  saveWiFiMode(MODE_CLIENT);
}

void setupWiFiServer() {
  display.showMessage("WiFi AP\nMode", ST77XX_BLUE);
  delay(1000);
  
  const char* ap_ssid = "MeshSense-Server";
  const char* ap_password = "mesh1234";
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  
  IPAddress IP = WiFi.softAPIP();
  
  // Display AP info
  display.clear();
  delay(100);
  
  char buffer[64];
  display.showMessageAt("WiFi AP Mode", 10, 20, ST77XX_BLUE, 1);
  
  snprintf(buffer, sizeof(buffer), "SSID: %s", ap_ssid);
  display.showMessageAt(buffer, 10, 40, ST77XX_BLUE, 1);
  
  snprintf(buffer, sizeof(buffer), "Pass: %s", ap_password);
  display.showMessageAt(buffer, 10, 55, ST77XX_BLUE, 1);
  
  snprintf(buffer, sizeof(buffer), "IP: %s", IP.toString().c_str());
  display.showMessageAt(buffer, 10, 70, ST77XX_BLUE, 1);
  
  display.showMessageAt("Waiting for", 10, 90, ST77XX_YELLOW, 1);
  display.showMessageAt("connections...", 10, 105, ST77XX_YELLOW, 1);
  
  saveWiFiMode(MODE_SERVER);
}

void provisionWiFi() {
  display.showMessage("WiFi Setup\nRequired", ST77XX_MAGENTA);
  delay(2000);
  
  display.clear();
  delay(100);
  
  display.showMessageAt("Press:", 10, 30, ST77XX_WHITE, 2);
  display.showMessageAt("BTN A:", 10, 60, ST77XX_GREEN, 2);
  display.showMessageAt("Client", 10, 80, ST77XX_WHITE, 2);
  display.showMessageAt("BTN B:", 10, 110, ST77XX_BLUE, 2);
  display.showMessageAt("Server", 10, 130, ST77XX_WHITE, 2);
  
  // Wait for button press (simplified - needs hardware button implementation)
  // For now, default to client mode after 10 seconds
  unsigned long startTime = millis();
  bool selected = false;
  
  while (!selected && (millis() - startTime < 10000)) {
    // TODO: Implement button reading
    // For demo, auto-select client mode
    delay(100);
  }
  
  // Default to client mode for initial setup
  setupWiFiClient();
}

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("ESP32-S3 TFT Feather - MeshSense");
  Serial.print("Setup running on core: ");
  Serial.println(xPortGetCoreID());
  
  // Initialize display manager (runs on Core 0)
  if (!display.begin()) {
    Serial.println("Failed to initialize display!");
    while (1) delay(1000);
  }
  
  display.showMessage("Initializing...", ST77XX_CYAN);
  delay(1000);
  
  // Check if WiFi is already provisioned
  currentMode = loadWiFiMode();
  
  if (currentMode == MODE_NONE) {
    Serial.println("No WiFi configuration found");
    provisionWiFi();
  } else if (currentMode == MODE_CLIENT) {
    Serial.println("Restoring Client mode");
    display.showMessage("Connecting...", ST77XX_GREEN);
    WiFi.begin();
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      setupWiFiClient();
    } else {
      display.showMessage("Connection\nFailed!", ST77XX_RED);
      delay(2000);
      provisionWiFi();
    }
  } else if (currentMode == MODE_SERVER) {
    Serial.println("Restoring Server mode");
    setupWiFiServer();
  }
  
  Serial.print("Main loop will run on core: ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  // Display status updates
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 5000) {
    lastUpdate = millis();
    
    if (currentMode == MODE_CLIENT && WiFi.status() == WL_CONNECTED) {
      Serial.print("Client - Signal: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    } else if (currentMode == MODE_SERVER) {
      Serial.print("Server - Clients: ");
      Serial.println(WiFi.softAPgetStationNum());
    }
  }
  
  delay(100);
}
