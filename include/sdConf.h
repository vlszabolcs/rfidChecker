#include <SPI.h>
#include <SD.h>

const int chipSelect = 4; // Change this to your SD card's chip select pin

void initSDCard() {
    if (!SD.begin()) {
        Serial.println("SD card initialization failed!");
        return;
    }
    Serial.println("SD card initialized.");
}