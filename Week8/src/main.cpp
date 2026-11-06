#include <Arduino.h>

#define DAC_PIN 25
#define PWM_PIN 18

#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RES 8

void setup()
{
    Serial.begin(115200);

    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    Serial.println("DAC + PWM Test");
}

void loop()
{
    // Increasing brightness
    for (int value = 0; value <= 255; value++)
    {
        dacWrite(DAC_PIN, value);
        ledcWrite(PWM_CHANNEL, value);

        delay(10);
    }

    // Decreasing brightness
    for (int value = 255; value >= 0; value--)
    {
        dacWrite(DAC_PIN, value);
        ledcWrite(PWM_CHANNEL, value);

        delay(10);
    }
}