#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
    SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

// Pin settings
const int buttonPin = 18;

// Debounce settings
const unsigned long debounceTime = 50;

int buttonState = HIGH;
int lastButtonReading = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long pressCount = 0;

// Display current status on OLED
void updateDisplay()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(15, 5);
    display.println("ESP32 STATUS");

    display.drawLine(0, 17, 127, 17, SSD1306_WHITE);

    display.setCursor(0, 25);
    display.print("Button: ");

    if (buttonState == LOW)
        display.println("PRESSED");
    else
        display.println("RELEASED");

    display.setCursor(0, 43);
    display.print("Press Count: ");
    display.println(pressCount);

    display.display();
}

void setup()
{
    Serial.begin(115200);

    pinMode(buttonPin, INPUT_PULLUP);

    // Initialize I2C using SDA = 21 and SCL = 22
    Wire.begin(21, 22);

    // Initialize OLED at I2C address 0x3C
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("OLED initialization failed!");
        while (true)
        {
            // Stop if the OLED cannot initialize
        }
    }

    display.clearDisplay();
    updateDisplay();

    Serial.println("ESP32 Button Status Display Started");
    Serial.println("Press the button to increase the counter.");
}



void loop()
{
    int reading = digitalRead(buttonPin);

    if (reading != lastButtonReading)
    {
        lastDebounceTime = millis();
    }

    if (millis() - lastDebounceTime >= debounceTime)
    {
        if (reading != buttonState)
        {
            buttonState = reading;

            if (buttonState == LOW)
            {
                pressCount++;
                Serial.print("Accepted press count: ");
                Serial.println(pressCount);
            }

            updateDisplay();
        }
    }

    lastButtonReading = reading;
}



