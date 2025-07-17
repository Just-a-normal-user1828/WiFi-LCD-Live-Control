# WiFi LCD Live Control

A simple project for ESP8266 that allows you to control a 16x2 I2C LCD display over Wi-Fi using a modern web interface. Enter any text in your browser, and it will instantly appear on the LCD!

## Features

- Real-time text display on LCD via web interface
- Modern, responsive web UI
- Input validation (no Cyrillic letters, max 32 characters)
- Easy setup with ESP8266 and I2C LCD

## Hardware Requirements

- ESP8266 (NodeMCU, Wemos D1 mini, etc.)
- 16x2 I2C LCD display
- Jumper wires

## Wiring

| LCD Pin | ESP8266 Pin |
|---------|-------------|
| VCC     | 3V3         |
| GND     | GND         |
| SDA     | D2 (GPIO4)  |
| SCL     | D1 (GPIO5)  |

## Getting Started

1. Clone this repository.
2. Open `lcd_live_control_esp8266.ino` in Arduino IDE.
3. Install the required libraries:
    - LiquidCrystal_I2C
    - Wire
4. Set your Wi-Fi credentials in the code:
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```
5. Upload the sketch to your ESP8266.
6. After connecting, the LCD will display the device's IP address.
7. Open the shown IP address in your browser in the format: http://your_IP/
8. Type your message and see it appear on the LCD!

## Web Interface

- Enter up to 32 Latin characters.
- Cyrillic letters are not supported.
- The first 16 characters are shown on the first LCD line, the next 16 on the second.

## License

MIT License

---

Created by Just-a-normal-user1828
