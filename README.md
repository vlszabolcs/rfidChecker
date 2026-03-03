## Overview

The aim of the project is to replace an NRI G-13.mft coin validator with an RFID-based coin emulator for a Saeco Royal Office coffee machine. The device allows the machine to identify users via RFID cards and based on their credit balance, either authorize or deny coffee dispensing. User data, config data and logs are stored in the Firebase Real Rime Database.
### Working demo : https://youtu.be/FtIDbL3Zp9k?si=jd4PyK9v06od8FpE


## Technical Analysis
The documentation for the [NRI G-13.mft](https://animo.eu/wp-content/uploads/2023/09/G13mft_par_GB.pdf) provided the following critical technical baseline:

#### Timing Diagram:
- Active Low logic
- Presignal
- Coin Signal
- Inhibit Signal

#### Physical Pinout and Key pins for this project:
- GND
- VCC: 10–16V
- 6PIN Inhibit Signal (Input)
- CoinPins (Output)

#### Oscilloscope Observations
Measurements on the Saeco Royal Office revealed specific behaviors:

- Inhibit Logic: The machine sets the inhibit pin high after credit entry and in case of machine error. Therefore, the timed presignal handling is not necessary and multiple credit entries can be handled.
- Voltage Levels: The coffee machine pulls the COIN lines to 5V. Because the system uses Active Low logic, the ESP32 can pull these lines to Ground (0V) directly. No level shifting is required for the output, even though the ESP32 operates at 3.3V.
- Power Supply: The 12V measured on the VCC line is regulated down to 5V to power the ESP32 and peripherals.

## Implementation
#### Tools: 
- ESP32 (ESP-WROOM-32)
- RFID-RC522
- D13-12864-I2C-BL (Driver: SH1106)
  
#### Features:
- Multi wifi connection
- Wifi error handeling
- Read user UID
- Make query form Firebase
- Handels queries  
- Check user credit amount 
- Deduct user credit
- Make log to Firebase
- Trigger coffee machine (2xpin6)
- Show remaining credit
- Read config form Firebase (free (bool), coffee price (number)),


A [Firebase Admin](https://github.com/vlszabolcs/firebaseUI) app was created to manage users and credits (Under construction!!!)
