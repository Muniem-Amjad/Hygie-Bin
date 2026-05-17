# HygieBin - Touchless Smart Dustbin

A hygiene-focused automatic dustbin that detects hand proximity and opens or closes the lid without any physical contact, built using Arduino UNO and an ultrasonic sensor.

---

## Overview

HygieBin uses an HC-SR04 ultrasonic sensor to detect when a hand is within 15 cm of the dustbin. When detected, a servo motor automatically opens the lid. After the hand is removed and 8 consecutive clear readings are confirmed, the lid closes automatically. This eliminates the need to touch the bin, reducing the spread of germs and improving hygiene.

---

## Features

- Touchless lid operation using ultrasonic hand detection
- Servo motor controlled lid open and close mechanism
- Green LED indicator when lid is open
- Red LED indicator when lid is closed or in standby
- Buzzer alert on lid open and close using tone function
- Noise filtering using median of 5 readings
- Consecutive reading counter to prevent false closure from ghost readings
- Fully simulated on Wokwi before hardware deployment

---

## Components

| Component          | Quantity | Purpose                          |
|--------------------|----------|----------------------------------|
| Arduino UNO R3     | 1        | Main microcontroller             |
| HC-SR04            | 1        | Ultrasonic hand proximity sensor |
| SG90 Servo Motor   | 1        | Opens and closes the lid         |
| LED Green          | 1        | Lid open indicator               |
| LED Red            | 1        | Lid closed indicator             |
| Buzzer             | 1        | Audio alert on lid state change  |
| Resistor 220 ohm   | 2        | Current limiting for LEDs        |
| Breadboard         | 1        | Circuit connections               |
| Jumper Wires       | 15-20    | Wiring all components            |

---

## Pin Connections

| Component       | Component Pin | Arduino Pin |
|-----------------|---------------|-------------|
| HC-SR04         | VCC           | 5V          |
| HC-SR04         | GND           | GND         |
| HC-SR04         | TRIG          | Pin 9       |
| HC-SR04         | ECHO          | Pin 10      |
| Servo Motor     | VCC           | 5V          |
| Servo Motor     | GND           | GND         |
| Servo Motor     | PWM Signal    | Pin 6       |
| Green LED       | Anode         | Pin 4       |
| Green LED       | Cathode       | GND         |
| Red LED         | Anode         | Pin 3       |
| Red LED         | Cathode       | GND         |
| Buzzer          | Pin 1         | GND         |
| Buzzer          | Pin 2         | Pin 5       |
| Resistor R1     | Between       | Pin 4 and Green LED Anode |
| Resistor R2     | Between       | Pin 3 and Red LED Anode   |

---

## How It Works

1. The HC-SR04 sensor continuously measures distance in front of the dustbin.
2. Five raw readings are taken per cycle and the median value is used to filter noise.
3. If the median distance is 15 cm or less, the lid opens via the servo motor.
4. The green LED turns on and the buzzer plays two short beeps.
5. When the hand is removed, a consecutive clear reading counter increments.
6. Once 8 consecutive readings show distance greater than 15 cm, the lid closes.
7. The red LED turns on and the buzzer plays one low beep to confirm closure.
8. Any ghost or noise reading below 15 cm resets the counter to prevent false closure.

---

## Configuration

All key parameters are defined at the top of the sketch for easy adjustment.

```cpp
#define DETECT_DISTANCE        15   // distance in cm to trigger lid open
#define OPEN_ANGLE             90   // servo angle for open position
#define CLOSE_ANGLE            0    // servo angle for closed position
#define CLEAR_READINGS_NEEDED  8    // consecutive clear readings before closing
```

| Parameter              | Default | Effect                                      |
|------------------------|---------|---------------------------------------------|
| DETECT_DISTANCE        | 15 cm   | Increase for more sensitive detection range |
| CLEAR_READINGS_NEEDED  | 8       | Increase for slower and more stable closing |
| delay in loop          | 150 ms  | Controls how fast readings are taken        |

---

## Simulation

This project was built and tested on the Wokwi Arduino Simulator before any hardware assembly.

To run the simulation:

1. Go to https://wokwi.com
2. Create a new Arduino UNO project
3. Replace the default diagram.json with the provided wokwi diagram file
4. Paste the Arduino sketch into the editor
5. Set the buzzer component mode to smooth in its attributes
6. Click the Run button and hover over the HC-SR04 sensor to simulate hand detection

---

## Wokwi Buzzer Note

The buzzer in Wokwi requires the tone() function to produce sound. Using digitalWrite() alone will not generate any audio. Make sure the buzzer pin 1 is connected to GND and pin 2 is connected to the Arduino signal pin.

```cpp
tone(BUZZER_PIN, 1000, 100);   // plays 1000 Hz tone for 100 ms
```

---

## Serial Monitor Output

Open the Serial Monitor at 9600 baud to view live distance readings and lid state.

```
=== Smart Dustbin Ready ===
Distance: 76 cm
Distance: 14 cm
>> LID OPENED
Distance: 2 cm
Distance: 2 cm
Distance: 76 cm   [clear count: 1/8]
Distance: 76 cm   [clear count: 2/8]
Distance: 76 cm   [clear count: 8/8]
>> LID CLOSED
```

---

## Project Structure

```
HygieBin/
├── README.md
├── hygie_bin.ino          Arduino sketch
└── wokwi/
    └── diagram.json       Wokwi simulation circuit file
```

---

## Future Improvements

- Add an LCD display to show distance and bin status
- Add a fill level sensor inside the bin to alert when full
- Integrate with a mobile app over Bluetooth for status monitoring
- Add a UV LED inside the bin for automatic sanitization
- Replace servo with a linear actuator for a more realistic lid mechanism
- Add power saving sleep mode when no activity is detected

---

## Tools Used

- Arduino IDE for sketch development
- Wokwi Simulator for circuit simulation and testing
- Arduino Servo library for servo motor control

---

## Author

Built as a hygiene-focused embedded systems project using Arduino UNO.

---

## License

This project is open source and available under the MIT License.