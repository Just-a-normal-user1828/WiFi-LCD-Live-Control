#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

LiquidCrystal_I2C lcd(0x27, 16, 2);
ESP8266WebServer server(80);

String htmlPage = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <title>ESP LCD Control</title>
    <style>
      body {
        margin: 0;
        padding: 0;
        font-family: "Segoe UI", sans-serif;
        background: linear-gradient(135deg, #1f4037, #99f2c8);
        color: #ffffff;
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        height: 100vh;
        text-align: center;
      }
      h1 {
        font-size: 2.5em;
        margin-bottom: 20px;
        text-shadow: 2px 2px 5px #000;
      }
      input[type="text"] {
        font-size: 1.2em;
        padding: 10px 15px;
        width: 80%;
        max-width: 400px;
        border: none;
        border-radius: 10px;
        box-shadow: 0 0 10px rgba(0,0,0,0.2);
        outline: none;
        transition: box-shadow 0.3s ease, background-color 0.3s ease;
      }
      input[type="text"].error {
        background-color: #ffcccc;
        box-shadow: 0 0 10px rgba(255, 0, 0, 0.7);
      }
      #status {
        margin-top: 15px;
        font-weight: bold;
        color: #eaffff;
        text-shadow: 1px 1px 2px #000;
      }
    </style>
  </head>
  <body>
    <h1>LCD Live Control</h1>
    <input type="text" id="wordInput" maxlength="32" placeholder="Type and see it on LCD...">
    <p id="status">Enter text...</p>

    <script>
      let timeout = null;

      function containsCyrillic(text) {
        return /[а-яёА-ЯЁ]/.test(text);
      }

      document.getElementById("wordInput").addEventListener("input", function () {
        clearTimeout(timeout);
        const input = document.getElementById("wordInput");
        const status = document.getElementById("status");
        let word = input.value;

        if (containsCyrillic(word)) {
          input.classList.add("error");
          status.innerText = "Cyrillic letters not supported!";
          return;
        } else {
          input.classList.remove("error");
        }

        if (word.length > 32) {
          word = word.slice(0, 32);
          input.value = word;
        }

        timeout = setTimeout(() => {
          fetch("/send?msg=" + encodeURIComponent(word))
            .then(response => response.text())
            .then(data => {
              status.innerText = "Displaying: " + word;
            })
            .catch(err => {
              status.innerText = "Failed to send!";
            });
        }, 200);
      });
    </script>
  </body>
</html>
)=====";

void handleSend() {
  String msg = server.arg("msg");
  Serial.println("[Web] Received: " + msg);
  lcd.clear();
  lcd.setCursor(0, 0);
  if (msg.length() > 16) {
    lcd.print(msg.substring(0, 16));
    lcd.setCursor(0, 1);
    lcd.print(msg.substring(16, 32));
  } else {
    lcd.print(msg);
  }
  server.send(200, "text/plain", "OK");
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.print("Connecting...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
  lcd.clear();
  lcd.print("IP:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();
  Serial.println("Server started!");
}

void loop() {
  server.handleClient();
}
