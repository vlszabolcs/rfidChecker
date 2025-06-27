The aim of the project is to replace an [NRI G-13.mft](https://animo.eu/wp-content/uploads/2023/09/G13mft_par_GB.pdf) coin validator with an RFID-based coin emulator for a Saeco Royal Office coffee machine. The device allows the machine to identify users via RFID cards and based on their credit balance, either authorize or deny coffee dispensing. User data, config data and logs are stored in the Firebase Real Rime Database.

<ins>Tools:</ins>
- ESP32 (ESP-WROOM-32)
- RFID-RC522
- D13-12864-I2C-BL (Driver: SH1106)
  
<ins>Features:</ins>
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

To Do:
- loan (bool), loan price (number)
- Replace anonymous login to the Firebase

A [Firebase Admin](https://github.com/vlszabolcs/firebaseUI) app was created to manage users and credits (Under construction!!!)
