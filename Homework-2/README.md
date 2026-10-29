# IoT Homework 2 — ESP32 PWM Control

## Task 1: Dual-LED PWM Control

This task uses an ESP32 to control two LEDs independently using Pulse Width Modulation (PWM). The brightness of both LEDs gradually increases and decreases.

**Features:**

- Two independently controlled LEDs.
- Different PWM frequencies: 1000 Hz (LED 1) and 5000 Hz (LED 2).
- 10-bit PWM resolution (0–1023).
- Smooth LED fading effect, with each LED fading at a different speed.
- LED 1 uses LEDC channel 0 and LED 2 uses LEDC channel 2, so they sit on different LEDC timers and can run at different frequencies.

## Task 2: Dual-LED PWM with OLED Monitoring

This task extends Task 1 by adding an SSD1306 I²C OLED display to monitor the LEDs' PWM values.

**Features:**

- Displays the duty-cycle percentage of both LEDs.
- Displays the configured PWM frequency of each LED.
- Updates duty-cycle values as the LEDs fade.
- Uses I²C communication between the ESP32 and OLED.
- The OLED reads the same duty variables that are written to the PWM hardware, so the display matches the real output.

## Hardware Components

- ESP32 DevKit
- Two LEDs
- Two 220 Ω or 330 Ω current-limiting resistors (one per LED)
- SSD1306 I²C OLED display (Task 2)
- Breadboard and jumper wires
- USB data cable

## Pin Configuration

| Component | ESP32 Pin |
|---|---|
| LED 1 PWM | GPIO 2 |
| LED 2 PWM | GPIO 18 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| OLED VCC | 3V3 (if supported by module) |
| Common Ground | GND |

## Wiring Details

**LED 1 (1000 Hz)**

- ESP32 GPIO 2 → one end of resistor 1 (220 Ω).
- Other end of resistor 1 → LED 1 anode (longer leg).
- LED 1 cathode (shorter leg) → GND.

**LED 2 (5000 Hz)**

- ESP32 GPIO 18 → one end of resistor 2 (220 Ω).
- Other end of resistor 2 → LED 2 anode (longer leg).
- LED 2 cathode (shorter leg) → GND.

**OLED (SSD1306, I²C, address 0x3C)**

- OLED VCC → ESP32 3V3.
- OLED GND → ESP32 GND.
- OLED SDA → ESP32 GPIO 21.
- OLED SCL → ESP32 GPIO 22.

**Notes**

- Each LED has its own separate resistor.
- Both LED cathodes and the OLED GND share the ESP32 GND rail on the breadboard.
- Disconnect power before changing wiring, and check your OLED module's pin order (some modules swap VCC and GND).
- GPIO 2 is a strapping pin and is connected to the onboard LED on many ESP32 DevKit boards, so the onboard LED may also fade with LED 1.

## PWM Configuration

| Setting | LED 1 | LED 2 |
|---|---|---|
| GPIO | 2 | 18 |
| Frequency | 1000 Hz | 5000 Hz |
| LEDC channel | 0 | 2 |
| LEDC timer | 0 | 1 |
| Resolution | 10 bit (0–1023) | 10 bit (0–1023) |

The ESP32 has 4 LEDC timers shared by 16 channels, and channels on the same timer must use the same frequency. Channels 0 and 2 are on different timers, which is why the two LEDs can run at different frequencies.

## Software and Tools

- Visual Studio Code
- PlatformIO
- Wokwi Simulator
- Arduino Framework (Arduino-ESP32 core 2.x, LEDC API: `ledcSetup`, `ledcAttachPin`, `ledcWrite`)
- Adafruit SSD1306 Library
- Adafruit GFX Library

## OLED Output

```
PWM MONITOR
LED1:  25% F:1000Hz
LED2:  75% F:5000Hz
```

The values above are an example. The OLED shows the actual duty percentage (`duty * 100 / 1023`) and configured frequency of each LED, refreshed every 100 ms.

## Testing

The project should be tested in the Wokwi simulator and on physical ESP32 hardware. Verify LED fading, OLED initialization, and duty-cycle updates. The Serial Monitor also prints the frequency read back from the hardware (`ledcReadFreq`) to confirm 1000 Hz and 5000 Hz. Add actual screenshots and hardware photographs after testing.

## Conclusion

This project demonstrates ESP32 PWM-based LED brightness control at different frequencies. Task 2 enhances the system by displaying the current duty-cycle percentages and configured PWM frequencies on an OLED screen.