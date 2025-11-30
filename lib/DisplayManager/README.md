# DisplayManager Library

Thread-safe TFT display manager for Adafruit ESP32-S3 TFT Feather board using FreeRTOS.

## Overview

The DisplayManager library provides a thread-safe interface for controlling the built-in ST7789 TFT display on the Adafruit ESP32-S3 TFT Feather board. It runs the display operations on a separate FreeRTOS task (Core 0), allowing the main application (Core 1) to run without blocking on display updates.

## Features

- **Thread-safe**: Uses FreeRTOS queue-based message passing
- **Non-blocking**: Display updates don't block main application
- **Dual-core**: Runs on Core 0, leaving Core 1 for WiFi and main logic
- **Simple API**: Easy-to-use methods for common display operations

## Architecture

```text
Main Application (Core 1)          Display Manager (Core 0)
      |                                    |
      |---> showMessage() -------------> Queue
      |---> showMessageAt() -----------> Queue
      |---> clear() --------------------> Queue
      |                                    |
      |                              processMessages()
      |                                    |
      |                              Update Display
```

## API Reference

### Initialization

```cpp
DisplayManager display;

void setup() {
  if (!display.begin()) {
    Serial.println("Display init failed!");
  }
}
```

### Display Methods

#### showMessage()

Display a centered message (clears screen first)

```cpp
display.showMessage("Hello World", ST77XX_GREEN, 2);
```

Parameters:

- `msg`: Text to display
- `color`: Text color (default: WHITE)
- `textSize`: Text size (default: 2)

#### showMessageAt()

Display text at specific position (no clear)

```cpp
display.showMessageAt("Status: OK", 10, 20, ST77XX_BLUE, 1);
```

Parameters:

- `msg`: Text to display
- `x`: X position
- `y`: Y position
- `color`: Text color (default: WHITE)
- `textSize`: Text size (default: 1)

#### clear()

Clear the display

```cpp
display.clear(ST77XX_BLACK);
```

#### setBrightness()

Control backlight

```cpp
display.setBrightness(true);  // On
display.setBrightness(false); // Off
```

## Hardware Configuration

Display pins are configured for Adafruit ESP32-S3 TFT Feather:

- TFT_CS: GPIO 7
- TFT_DC: GPIO 39
- TFT_RST: GPIO 40
- TFT_BACKLIGHT: GPIO 45

## Thread Safety

All public methods are thread-safe and can be called from any task. The library uses FreeRTOS queues to safely pass display commands to the display task.

## Memory Usage

- Task stack: 4096 bytes
- Message queue: 10 messages × 136 bytes = 1360 bytes
- Total: ~5.5 KB

## Example

```cpp
#include <DisplayManager.h>

DisplayManager display;

void setup() {
  Serial.begin(115200);

  if (!display.begin()) {
    Serial.println("Display failed!");
    while(1);
  }

  display.showMessage("Starting...", ST77XX_CYAN);
  delay(2000);

  display.clear();
  display.showMessageAt("Line 1", 10, 20, ST77XX_GREEN, 1);
  display.showMessageAt("Line 2", 10, 35, ST77XX_YELLOW, 1);
}

void loop() {
  // Main loop continues without blocking on display
  // Do work here
}
```

## Dependencies

- Adafruit GFX Library
- Adafruit ST7735 and ST7789 Library
- Arduino ESP32 (FreeRTOS included)
