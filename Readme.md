# Nurse Call System by CCNG

This Nurse Call System is built using an **Arduino Mega 2560** to monitor and manage patient call buttons from up to 32 beds. It displays call statuses on an LCD, uses an RGB LED as an indicator, and logs data into an SD card.

---

## Features

- Supports monitoring of **32 patient call buttons**
- Status display via **20x4 I2C LCD**
- **RGB LED** as a status indicator
- **Buzzer** for audio alerts
- **Menu navigation** with physical buttons
- **SD Card** for call logging

---

## Arduino Mega 2560 Pinout Details

### Patient Call Buttons (Inputbutton)

Each bed is assigned a dedicated button using the following digital pins:

```cpp
const uint8_t Inputbutton[SizeButton] = {
  22, 23, 24, 25, 26, 27, 28, 29, 30,
  31, 32, 33, 34, 35, 36, 37,
  38, 39, 40, 41, 42, 43, 44, 45,
  46, 47, 48, 49, 50, 51, 52, 53
};
Note:
	•	Uses digital pins 22–53.
	•	These pins do not support external interrupts.
	•	Ideal for polling input using digitalRead().

Buzzer
	•	Pin: A3
	•	Used for sound alerts.
	•	Can be toggled ON/OFF or controlled using tone() if needed.

LCD 20x4 (I2C)
	•	SDA: Pin 20
	•	SCL: Pin 21

Uses I2C communication. Only two pins needed. Recommended library: LiquidCrystal_I2C.

RGB LED
	•	Red: Pin 12 (PWM supported)
	•	Green: Pin 11 (PWM supported)
	•	Blue: Pin 13 (PWM supported)

Used to display status using color combinations. You can control brightness with analogWrite().

⸻

Menu Navigation Buttons
	•	Confirm: Pin A2
	•	Up: Pin A0
	•	Down: Pin A1

Used to navigate system menu and select or reset options.

⸻

SD Card Module (Software SPI)
	•	CS (Chip Select): Pin 7
	•	DO (MISO): Pin 6
	•	DI (MOSI): Pin 8
	•	SCK (Clock): Pin 9

Uses software SPI. Make sure your SD library supports custom SPI pins.

⸻

Components Used
	•	Arduino Mega 2560
	•	20x4 I2C LCD
	•	32x Push Buttons
	•	RGB LED (3-pin)
	•	Buzzer
	•	SD Card Module
	•	Resistors, jumper wires, breadboard or PCB

⸻

How It Works
	1.	The patient presses the call button.
	2.	The system detects the signal from the corresponding pin.
	3.	RGB LED lights up to indicate the call.
	4.	Buzzer sounds as an alert.
	5.	The call event is logged to the SD card.
	6.	Menu buttons allow staff to check and reset the calls.

⸻