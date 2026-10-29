#include <Arduino.h>

// LED pins
#define LED1_PIN 2
#define LED2_PIN 18

// PWM frequency for each LED in Hz
#define LED1_FREQ 1000
#define LED2_FREQ 5000

// PWM resolution is 10 bits, so duty ranges from 0 to 1023
#define PWM_RES 10

// Different PWM channels for both LEDs
#define LED1_CH 0
#define LED2_CH 2

const int MAX_DUTY = (1 << PWM_RES) - 1;  // Maximum duty value: 1023

// Initial brightness values
int duty1 = 0;          // LED1 starts off
int duty2 = MAX_DUTY;   // LED2 starts fully on

// Controls brightness change and fading direction
int step1 = 4;
int step2 = -6;

unsigned long lastStep = 0;  // Stores the last update time

// Set the brightness of an LED using PWM
void writeDuty(int pin, int ch, int duty) {
  ledcWrite(ch, duty);
}

void setup() {
  Serial.begin(115200);  // Start serial communication

  // Configure PWM for LED1
  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);

  // Configure PWM for LED2
  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);
}

void loop() {
  // Update LED brightness every 10 milliseconds
  if (millis() - lastStep >= 10) {
    lastStep = millis();

    // Change LED1 brightness
    duty1 += step1;

    // Reverse direction at maximum brightness
    if (duty1 >= MAX_DUTY) {
      duty1 = MAX_DUTY;
      step1 = -step1;
    }

    // Reverse direction at minimum brightness
    if (duty1 <= 0) {
      duty1 = 0;
      step1 = -step1;
    }

    // Change LED2 brightness
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
    writeDuty(LED1_PIN, LED1_CH, duty1);
    writeDuty(LED2_PIN, LED2_CH, duty2);
  }
}