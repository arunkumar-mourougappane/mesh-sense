#include "DisplayManager.h"

DisplayManager::DisplayManager() 
  : tft(TFT_CS, TFT_DC, TFT_RST), 
    displayTaskHandle(NULL), 
    messageQueue(NULL) {
}

DisplayManager::~DisplayManager() {
  if (displayTaskHandle != NULL) {
    vTaskDelete(displayTaskHandle);
  }
  if (messageQueue != NULL) {
    vQueueDelete(messageQueue);
  }
}

bool DisplayManager::begin() {
  // Initialize backlight
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);
  
  // Initialize TFT display
  tft.init(135, 240);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  
  // Create message queue
  messageQueue = xQueueCreate(10, sizeof(DisplayMessage));
  if (messageQueue == NULL) {
    Serial.println("Failed to create display message queue");
    return false;
  }
  
  // Create display task on Core 0
  BaseType_t result = xTaskCreatePinnedToCore(
    displayTask,           // Task function
    "DisplayTask",         // Task name
    4096,                  // Stack size
    this,                  // Task parameter
    1,                     // Priority
    &displayTaskHandle,    // Task handle
    0                      // Core 0
  );
  
  if (result != pdPASS) {
    Serial.println("Failed to create display task");
    return false;
  }
  
  Serial.println("Display manager initialized on Core 0");
  return true;
}

void DisplayManager::showMessage(const char* msg, uint16_t color, uint8_t textSize) {
  if (messageQueue == NULL) return;
  
  DisplayMessage message;
  strncpy(message.text, msg, sizeof(message.text) - 1);
  message.text[sizeof(message.text) - 1] = '\0';
  message.color = color;
  message.textSize = textSize;
  message.clearScreen = true;
  message.x = 10;
  message.y = 50;
  
  xQueueSend(messageQueue, &message, portMAX_DELAY);
}

void DisplayManager::showMessageAt(const char* msg, int16_t x, int16_t y, uint16_t color, uint8_t textSize) {
  if (messageQueue == NULL) return;
  
  DisplayMessage message;
  strncpy(message.text, msg, sizeof(message.text) - 1);
  message.text[sizeof(message.text) - 1] = '\0';
  message.color = color;
  message.textSize = textSize;
  message.clearScreen = false;
  message.x = x;
  message.y = y;
  
  xQueueSend(messageQueue, &message, portMAX_DELAY);
}

void DisplayManager::clear(uint16_t color) {
  if (messageQueue == NULL) return;
  
  DisplayMessage message;
  message.text[0] = '\0';
  message.color = color;
  message.clearScreen = true;
  message.x = 0;
  message.y = 0;
  
  xQueueSend(messageQueue, &message, portMAX_DELAY);
}

void DisplayManager::setBrightness(bool on) {
  digitalWrite(TFT_BACKLIGHT, on ? HIGH : LOW);
}

void DisplayManager::displayTask(void* parameter) {
  DisplayManager* manager = static_cast<DisplayManager*>(parameter);
  
  Serial.println("Display task started");
  
  while (true) {
    manager->processMessages();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void DisplayManager::processMessages() {
  DisplayMessage message;
  
  while (xQueueReceive(messageQueue, &message, 0) == pdTRUE) {
    if (message.clearScreen) {
      tft.fillScreen(ST77XX_BLACK);
    }
    
    if (message.text[0] != '\0') {
      tft.setCursor(message.x, message.y);
      tft.setTextColor(message.color);
      tft.setTextSize(message.textSize);
      tft.println(message.text);
    }
  }
}
