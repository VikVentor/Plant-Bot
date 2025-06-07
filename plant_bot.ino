#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "animate.h"  // Must include: playhappyAnimation(), playneutralAnimation(), playsadAnimation()

#define SCREEN_I2C_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RST_PIN -1

#define SOIL_SENSOR_PIN A0        // Analog pin (GPIO2 on Xiao ESP32-C3)
#define SENSOR_POWER_PIN D7       // GPIO pin to power the sensor

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST_PIN);

// Timer variables
unsigned long lastReadTime = 0;
const unsigned long sensorInterval = 2000;  // Read every 2 seconds

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR_POWER_PIN, LOW);  // Sensor initially OFF

  // Initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);
  display.clearDisplay();

  // --- Startup message (once) ---
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 10);
  display.print("Planto");

  display.setTextSize(1);
  display.setCursor(20, 35);
  display.print("Keep them Healthy!");
  display.display();
  delay(4000);  // Show startup message for 4 seconds

  // --- Ready message (once) ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Soil Monitor Ready");
  display.display();
  delay(1000);
}

void loop() {
  unsigned long now = millis();

  // Read sensor and show animation every 2 seconds
  if (now - lastReadTime >= sensorInterval) {
    lastReadTime = now;

    int soilValue = readSoilSensor();
    Serial.print("Soil Moisture Value: ");
    Serial.println(soilValue);

    // Choose and run animation based on soil moisture value
    if (soilValue < 1800) {
      playhappyAnimation(display);
    } else if (soilValue <= 2000) {
      playneutralAnimation(display);
    } else {
      playsadAnimation(display);
    }
  }
}

// Read soil sensor with power cycling
int readSoilSensor() {
  digitalWrite(SENSOR_POWER_PIN, HIGH);
  delay(300);  // Wait for sensor to stabilize
  int value = analogRead(SOIL_SENSOR_PIN);
  digitalWrite(SENSOR_POWER_PIN, LOW);
  return value;
}
