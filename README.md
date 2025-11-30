# MeshSense - ESP32-S3 TFT Feather WiFi Provisioning

[![PlatformIO CI](https://github.com/yourusername/mesh-sense/actions/workflows/build.yml/badge.svg)](https://github.com/yourusername/mesh-sense/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A PlatformIO project for the Adafruit ESP32-S3 TFT Feather board with WiFi provisioning capabilities and multi-threaded display management.

## Hardware

- **Board**: Adafruit ESP32-S3 TFT Feather
- **Display**: Built-in 1.14" 240x135 Color TFT (ST7789)
- **WiFi**: ESP32-S3 built-in
- **MCU**: Dual-core ESP32-S3 @ 240MHz

## Features

- **WiFi Provisioning**: Configure device as either WiFi Client or Access Point Server
- **Persistent Configuration**: Saves WiFi mode to flash memory
- **Visual Feedback**: TFT display shows connection status and configuration
- **Multi-threaded Architecture**: Display runs on separate FreeRTOS task (Core 0)
- **Thread-safe Display**: Non-blocking display updates via message queue
- **Dual Mode Support**:
  - **Client Mode**: Connects to existing WiFi network via WiFiManager captive portal
  - **Server Mode**: Creates its own Access Point for other devices to connect

## Architecture

The project uses a dual-core architecture:

- **Core 0**: Display Manager task - handles all TFT display operations
- **Core 1**: Main application - WiFi management, provisioning logic

This separation ensures display updates never block WiFi operations or main application logic.

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- Adafruit ESP32-S3 TFT Feather board
- USB-C cable

### Building

```bash
pio run
```

**Memory Usage** (as of latest build):

- RAM: 17.6% (57,552 / 327,680 bytes)
- Flash: 59.9% (864,205 / 1,441,792 bytes)

### Uploading

```bash
pio run --target upload
```

### Monitoring Serial Output

```bash
pio device monitor
```

## WiFi Provisioning

### First Boot

On first boot, the device will prompt you to select a WiFi mode:

1. **Client Mode** (default after 10 seconds):

   - Device creates a captive portal "MeshSense-AP" (password: "password")
   - Connect to this AP with your phone/computer
   - Configure your home WiFi credentials through the web interface
   - Device saves credentials and connects automatically on future boots

2. **Server Mode**:
   - Device creates an Access Point "MeshSense-Server" (password: "mesh1234")
   - Other devices can connect to this AP
   - IP address: 192.168.4.1

### Reconfiguration

To reset WiFi configuration, you can:

- Erase flash: `pio run --target erase`
- Or modify code to add a physical button to reset configuration

## Configuration

Edit `include/wifi_config.h` to customize:

- AP SSID and passwords
- Connection timeouts
- Portal credentials

## Libraries Used

- **DisplayManager**: Custom thread-safe TFT display manager (see `lib/DisplayManager/`)
- **Adafruit GFX Library**: Graphics core
- **Adafruit ST7789**: TFT display driver
- **WiFiManager**: WiFi provisioning with captive portal
- **ArduinoJson**: JSON parsing (for future expansion)
- **Preferences**: ESP32 NVS storage

## Project Structure

```text
mesh-sense/
├── src/
│   └── main.cpp              # Main application (Core 1)
├── lib/
│   └── DisplayManager/       # Thread-safe display library (Core 0)
│       ├── DisplayManager.h
│       ├── DisplayManager.cpp
│       └── README.md
├── include/
│   └── wifi_config.h         # WiFi configuration constants
└── platformio.ini            # Project configuration
```

## Pin Configuration

Display pins (Adafruit ESP32-S3 TFT Feather):

- TFT_CS: GPIO 7
- TFT_DC: GPIO 39
- TFT_RST: GPIO 40
- TFT_BACKLIGHT: GPIO 45

## Contributing

See [CONTRIBUTING.md](.github/CONTRIBUTING.md) for development workflow and guidelines.

All pull requests are automatically checked for:

- Successful build
- Memory usage limits (RAM < 80%, Flash < 90%)
- Size comparison against base branch

## CI/CD

This project uses GitHub Actions for automated builds and checks:

- **Build Workflow**: Runs on every push/PR to verify compilation and memory usage
- **Size Check**: Compares memory usage on PRs against the base branch

See [MEMORY_LIMITS.md](.github/MEMORY_LIMITS.md) for details on memory thresholds.

## License

See LICENSE file for details.
