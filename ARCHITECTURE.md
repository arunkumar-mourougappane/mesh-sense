# Architecture Overview

## Multi-threaded Design

This project uses FreeRTOS on the ESP32-S3 dual-core processor to separate concerns:

```text
┌────────────────────────────────────────────────────────────┐
│                      ESP32-S3 System                       │
├───────────────────────────────┬────────────────────────────┤
│         Core 1                │          Core 0            │
│     (Main Application)        │     (Display Manager)      │
├───────────────────────────────┼────────────────────────────┤
│                               │                            │
│  ┌─────────────────────┐      │  ┌───────────────────┐     │
│  │   Main Loop Task    │      │  │  Display Task     │     │
│  │                     │      │  │                   │     │
│  │  • WiFi Management  │      │  │  • Process Queue  │     │
│  │  • Provisioning     │────────▶│  • Update TFT     │     │
│  │  • Logic/Control    │ Queue│  │  • Handle SPI     │     │
│  │                     │      │  │                   │     │
│  └─────────────────────┘      │  └───────────────────┘     │
│                               │                            │
└───────────────────────────────┴────────────────────────────┘
```

## Benefits

1. **Non-blocking Display**: Display updates don't delay WiFi operations
2. **Thread Safety**: Queue-based communication prevents race conditions
3. **Performance**: WiFi runs on dedicated core without display overhead
4. **Maintainability**: Clean separation of display and application logic

## Communication Flow

```text
     Main
Application Code                 DisplayManager
     |                                |
     |-- showMessage("Text") -------->|
     |                           [Queue Entry]
     |                                |
     |  (continues immediately)  processMessages()
     |                                |
     |                           [Update Display]
     |                                |
     |<-------- (async) --------------|
```

## Task Priorities

- **Display Task**: Priority 1 (Low)
- **WiFi Tasks**: Priority 19-23 (System managed)
- **Main Loop**: Priority 1 (Default)

The display task runs at low priority to ensure WiFi and critical operations take precedence.

## Memory Usage

| Component          | Size    |
| ------------------ | ------- |
| Display Task Stack | 4096 B  |
| Message Queue      | 1360 B  |
| Total Overhead     | ~5.5 KB |

## Thread Safety

All DisplayManager public methods are thread-safe:

- `showMessage()`
- `showMessageAt()`
- `clear()`
- `setBrightness()`

These methods use FreeRTOS queues (`xQueueSend()`) for safe inter-task communication.

## Potential Improvements

1. Add priority levels for urgent vs. normal display updates
2. Implement double-buffering for smoother animations
3. Add display sleep/wake management
4. Implement frame rate limiting
