/*
 *  Based on the following examples:
 *  WiFi > WiFiMulti: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiMulti/WiFiMulti.ino
 *  WiFi > WiFiScan: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiScan/WiFiScan.ino
 *  Complete project details at our blog: https://RandomNerdTutorials.com/
 *
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiMulti wifiMulti;

extern NTPClient timeClient;


void wifiConfig()
{
  WiFi.mode(WIFI_STA);
  // Add list of wifi networks
  wifiMulti.addAP(ssid1, pw1);
  wifiMulti.addAP(ssid2, pw2);
  ///wifiMulti.addAP(ssid3, pw3);

  // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
  Serial.println("Connecting Wifi...");
  u8x8.println("Connecting Wifi...");
  if (wifiMulti.run() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    u8x8.println("WiFi connected");
    u8x8.println("IP address: ");
    u8x8.println(WiFi.localIP());
    u8x8.println(WiFi.SSID());
    u8x8.print("RSSI: ");
    u8x8.println(WiFi.RSSI());

    u8x8.refreshDisplay();
  }
}

int determineDSTOffset(time_t utcEpoch) {
    struct tm *timeinfo;
    time_t rawtime = utcEpoch;
    timeinfo = gmtime(&rawtime);
    int year = timeinfo->tm_year + 1900;

    // DST start: march last sunday 2:00 (CET -> CEST)
    struct tm lastMarchSunday = {0, 0, 2, 31, 2, year - 1900};
    mktime(&lastMarchSunday);
    while (lastMarchSunday.tm_wday != 0) {
        lastMarchSunday.tm_mday--;
        mktime(&lastMarchSunday);
    }
    time_t dstStart = mktime(&lastMarchSunday);

    // DST end: october last sunday 3:00 (CEST -> CET)
    struct tm lastOctoberSunday = {0, 0, 3, 31, 9, year - 1900};
    mktime(&lastOctoberSunday);
    while (lastOctoberSunday.tm_wday != 0) {
        lastOctoberSunday.tm_mday--;
        mktime(&lastOctoberSunday);
    }
    time_t dstEnd = mktime(&lastOctoberSunday);

    return (utcEpoch >= dstStart && utcEpoch < dstEnd) ? 2 : 1; // CEST:2, CET:1
}

void ntpConf()
{
  timeClient.begin();
  timeClient.update();
  dstOffset = determineDSTOffset(timeClient.getEpochTime());
}




