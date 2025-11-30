# Wokwi Simulation for MeshSense

This project includes Wokwi simulator support for testing without physical hardware.

## What is Wokwi?

Wokwi is an online electronics simulator that supports ESP32 boards and various peripherals. It allows you to test your firmware in a virtual environment.

Website: https://wokwi.com/

## Hardware Simulation

The Wokwi configuration simulates:
- **ESP32-S3 DevKit** (closest match to Feather)
- **ST7789 TFT Display** (240x135 pixels)
- **Serial Monitor** for debugging

### Pin Mapping

| Component | ESP32 Pin | Function |
|-----------|-----------|----------|
| TFT CS | GPIO 7 | Chip Select |
| TFT DC | GPIO 39 | Data/Command |
| TFT RST | GPIO 40 | Reset |
| TFT BLK | GPIO 45 | Backlight |
| TFT SCL | GPIO 12 | SPI Clock |
| TFT SDA | GPIO 11 | SPI MOSI |

## Running the Simulation

### Method 1: Wokwi for VS Code (Recommended)

1. **Install Extension:**
   ```
   VS Code → Extensions → Search "Wokwi Simulator"
   ```

2. **Build Firmware:**
   ```bash
   pio run
   ```

3. **Start Simulation:**
   - Press `F1` → "Wokwi: Start Simulator"
   - Or click the Wokwi icon in VS Code status bar

4. **Monitor Output:**
   - Serial output appears in terminal
   - TFT display updates in simulator window

### Method 2: Wokwi CLI

1. **Install Wokwi CLI:**
   ```bash
   curl -L https://wokwi.com/ci/install.sh | sh
   ```

2. **Build Firmware:**
   ```bash
   pio run
   ```

3. **Run Simulation:**
   ```bash
   wokwi-cli --timeout 0 .
   ```

### Method 3: Wokwi Website

1. **Build Firmware:**
   ```bash
   pio run
   ```

2. **Upload Files:**
   - Go to https://wokwi.com/
   - Create new ESP32-S3 project
   - Upload `diagram.json` and `firmware.bin`

3. **Run:**
   - Click "Play" button in Wokwi interface

## Configuration Files

### wokwi.toml
Main configuration file that specifies:
- Firmware paths
- Port forwarding settings
- Simulator version

### diagram.json
Hardware diagram that defines:
- Board type (ESP32-S3)
- Components (TFT display)
- Wiring connections
- Component attributes

## Limitations

### Display Simulation
- Full color TFT display may have limited fidelity
- Some graphics effects may not render perfectly
- Refresh rate may differ from hardware

### WiFi Simulation
- WiFi functionality is simulated
- Network speed may not match real hardware
- Some WiFi operations may timeout differently

### Performance
- Simulation runs slower than real hardware
- FreeRTOS timing may differ slightly
- Use for functional testing, not performance testing

## Testing Strategies

### What to Test in Wokwi

✅ **Good for:**
- Display initialization
- Basic graphics rendering
- Text output on TFT
- Serial communication
- Logic flow testing
- WiFi state machine testing

❌ **Not ideal for:**
- Performance benchmarking
- Real WiFi connectivity
- Precise timing measurements
- Power consumption testing

### Testing the Display

```cpp
// Test display initialization
display.begin();
display.showMessage("Wokwi Test", ST77XX_GREEN);
```

### Testing WiFi State Machine

```cpp
// Test WiFi provisioning logic without real network
// State transitions will work in simulation
setupWiFiClient();
```

## Troubleshooting

### Simulation Won't Start

1. **Check Firmware Build:**
   ```bash
   pio run
   ls -la .pio/build/adafruit_feather_esp32s3_tft/firmware.elf
   ```

2. **Verify wokwi.toml Path:**
   - Ensure paths match your build output
   - Check file permissions

3. **VS Code Extension Issues:**
   - Reload VS Code window
   - Check extension logs
   - Update extension to latest version

### Display Not Working

1. **Check Pin Connections:**
   - Review `diagram.json`
   - Verify pin numbers match code

2. **Serial Output:**
   - Check for initialization errors
   - Look for "Display task started" message

### Slow Simulation

1. **Reduce Complexity:**
   - Comment out WiFi scanning temporarily
   - Simplify display updates

2. **Use CLI Mode:**
   - CLI is faster than browser
   - Run headless for testing

## Best Practices

1. **Build First:**
   Always build firmware before starting simulation
   ```bash
   pio run && wokwi-cli .
   ```

2. **Use Serial Debug:**
   Add debug messages to track execution
   ```cpp
   Serial.println("Display initialized");
   ```

3. **Incremental Testing:**
   Test components individually before integration

4. **Version Control:**
   Keep `wokwi.toml` and `diagram.json` in git

## Integration with CI/CD

Currently, Wokwi simulation is for local development only. Future enhancements could include:
- Automated Wokwi testing in CI pipeline
- Screenshot capture for visual regression testing
- Headless simulation for unit tests

## Resources

- **Wokwi Documentation:** https://docs.wokwi.com/
- **ESP32-S3 Reference:** https://docs.wokwi.com/parts/wokwi-esp32-s3-devkitc-1
- **ST7789 Display:** https://docs.wokwi.com/parts/wokwi-st7789
- **Wokwi for VS Code:** https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode

## Example Workflow

```bash
# 1. Make code changes
vim src/main.cpp

# 2. Build firmware
pio run

# 3. Test in Wokwi
wokwi-cli .

# 4. Observe serial output and display
# Press Ctrl+C to stop

# 5. If tests pass, commit
git add . && git commit -m "Feature update"
```

## Support

If you encounter issues with Wokwi simulation:
1. Check Wokwi documentation
2. Verify firmware builds correctly with PlatformIO
3. Review pin mappings in diagram.json
4. Test on actual hardware for validation

---

**Note:** Wokwi simulation is a development tool. Always validate on actual hardware before deployment!
