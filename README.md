# ESP32 Virtual Windshield Wiper

An interactive project connecting a physical windshield wiper to a JavaScript-based virtual rain simulation.

## Overview

This project demonstrates real-time interaction between embedded hardware and a browser-based simulation.

A physical wiper driven by an ESP32-controlled servo motor clears virtual raindrops displayed on a laptop screen.

## Hardware

- ESP32 development board
- MG90S positional servo motor
- External regulated 5V power supply
- Handmade windshield wiper
- USB cable

## Software

- ESP-IDF (C)
- HTML, CSS, JavaScript
- Web Serial API
- PWM servo control

## How It Works

1. Keyboard input is processed by JavaScript.
2. JavaScript calculates the target wiper angle.
3. The browser sends the angle to the ESP32 through USB serial communication.
4. The ESP32 generates a PWM signal to position the servo motor.
5. JavaScript uses the commanded angle to calculate the virtual wiping path and remove raindrops.

**Control flow:**

Keyboard → JavaScript → USB → ESP32 → PWM → Servo

## Features

- Keyboard-controlled physical wiper
- PWM-based servo positioning
- Continuous virtual rain simulation
- Real-time wiping effect
- Browser-to-ESP32 serial communication

## Running the Project

1. Build and flash the ESP32 firmware using ESP-IDF.
2. Open `windshield.html` in Chrome.
3. Connect the ESP32 using the webpage's serial connection control.
4. Use the left and right arrow keys to move the wiper.

## Notes

The servo should be powered using a suitable external 5V supply with a common ground shared with the ESP32.

The virtual wiping effect uses the commanded servo angle rather than measured physical position.
