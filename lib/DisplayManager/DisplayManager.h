#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// Display pins for Adafruit ESP32-S3 TFT Feather
#define TFT_CS         7
#define TFT_RST        40
#define TFT_DC         39
#define TFT_BACKLIGHT  45

// Display message structure
struct DisplayMessage {
  char text[128];
  uint16_t color;
  uint8_t textSize;
  bool clearScreen;
  int16_t x;
  int16_t y;
};

class DisplayManager {
public:
  DisplayManager();
  ~DisplayManager();
  
  // Initialize display and start thread
  bool begin();
  
  // Display control methods (thread-safe)
  void showMessage(const char* msg, uint16_t color = ST77XX_WHITE, uint8_t textSize = 2);
  void showMessageAt(const char* msg, int16_t x, int16_t y, uint16_t color = ST77XX_WHITE, uint8_t textSize = 1);
  void clear(uint16_t color = ST77XX_BLACK);
  void setBrightness(bool on);
  
  // Get display instance (use with caution, not thread-safe)
  Adafruit_ST7789* getDisplay() { return &tft; }
  
private:
  Adafruit_ST7789 tft;
  TaskHandle_t displayTaskHandle;
  QueueHandle_t messageQueue;
  
  // Display task function
  static void displayTask(void* parameter);
  void processMessages();
};

#endif // DISPLAY_MANAGER_H
