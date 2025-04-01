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

void ntpConf()
{
  timeClient.begin();
}
