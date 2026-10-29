#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Create the OLED display object
Adafruit_SSD1306 display(128, 64, &Wire, -1);

unsigned long lastOled = 0;  // Stores the last OLED update time

// Pins for both LEDs
#define LED1_PIN 2
#define LED2_PIN 18

// PWM frequencies for both LEDs
#define LED1_FREQ 1000
#define LED2_FREQ 5000

// PWM resolution is 10 bits, so duty ranges from 0 to 1023
#define PWM_RES 10

// Different channels allow different PWM frequencies
#define LED1_CH 0
#define LED2_CH 2

const int MAX_DUTY = (1 << PWM_RES) - 1;  // Maximum duty value: 1023

// Initial duty values of both LEDs
int duty1 = 0;          // LED1 starts off
int duty2 = MAX_DUTY;   // LED2 starts fully on

// Controls the fading speed and direction
int step1 = 4;
int step2 = -6;

unsigned long lastStep = 0;  // Stores the last fading update time

// Sends the duty value to the selected PWM channel
void writeDuty(int ch, int duty) {
  ledcWrite(ch, duty);
}

void setup() {
  Serial.begin(115200);  // Start serial communication

  // Start I2C communication for the OLED
  Wire.begin(21, 22);  // SDA = GPIO21, SCL = GPIO22

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  }

  display.setTextColor(SSD1306_WHITE);

  // Configure PWM for LED1
  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);

  // Configure PWM for LED2
  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);

  // Print the ESP32 Arduino core version
  Serial.printf("Arduino-ESP32 core: %d.%d.%d\n",
                ESP_ARDUINO_VERSION_MAJOR,
                ESP_ARDUINO_VERSION_MINOR,
                ESP_ARDUINO_VERSION_PATCH);

  // Print the actual PWM frequencies
  Serial.printf("LED1 freq: %u Hz\n", (unsigned)ledcReadFreq(LED1_CH));
  Serial.printf("LED2 freq: %u Hz\n", (unsigned)ledcReadFreq(LED2_CH));
}

void loop() {
  // Update LED brightness every 10 milliseconds
  if (millis() - lastStep >= 10) {
    lastStep = millis();

    // Increase or decrease LED1 brightness
    duty1 += step1;

    // Reverse direction when maximum brightness is reached
    if (duty1 >= MAX_DUTY) {
      duty1 = MAX_DUTY;
      step1 = -step1;
    }

    // Reverse direction when minimum brightness is reached
    if (duty1 <= 0) {
      duty1 = 0;
      step1 = -step1;
    }

    // Increase or decrease LED2 brightness
    duty2 += step2;

    // Reverse direction at maximum brightness
    if (duty2 >= MAX_DUTY) {
      duty2 = MAX_DUTY;
      step2 = -step2;
    }

    // Reverse direction at minimum brightness
    if (duty2 <= 0) {
      duty2 = 0;
      step2 = -step2;
    }

    // Apply the new brightness values to both LEDs
    writeDuty(LED1_CH, duty1);
    writeDuty(LED2_CH, duty2);
  }

  // Update the OLED every 100 milliseconds
  if (millis() - lastOled >= 100) {
    lastOled = millis();

    display.clearDisplay();  // Clear the previous display
    display.setTextSize(1);

    // Display the heading
    display.setCursor(0, 0);
    display.println("PWM MONITOR");

    // Convert LED1 duty value into a percentage
    display.setCursor(0, 20);
    display.printf("LED1: %3d%% F:%dHz",
                   duty1 * 100 / MAX_DUTY, LED1_FREQ);

    // Convert LED2 duty value into a percentage
    display.setCursor(0, 40);
    display.printf("LED2: %3d%% F:%dHz",
                   duty2 * 100 / MAX_DUTY, LED2_FREQ);

    display.display();  // Show the updated information on the OLED
  }
}