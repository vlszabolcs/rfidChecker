#include <Arduino.h>

#include <globals.h>
#include <logic.h>
#include <sss.h>
#include <dispConf.h>
#include <rfidConf.h>
#include <wifiConf.h>
#include <firebaseConf.h>
#include <buzzer.h>

void setup(void)
{
  Serial.begin(115200);
  displayConfig();
  wifiConfig();
  ntpConf();

  pinMode(mIN, INPUT);
  pinMode(mOut, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(mOut, 1);
  digitalWrite(buzzer, 0);

  firebaseConfig();

  SPI.begin();
  rfidConfig();
}

void loop(void)
{
  /*if (!Firebase.RTDB.readStream(&fbdo)) {
     Serial.print("Stream olvasási hiba: ");
     Serial.println(fbdo.errorReason());
   }*/
  // beep();
  if (wifiMulti.run() != WL_CONNECTED)
  {
    mainfunc();
  }
  else
  {
    Serial.println("WiFi connection error");
    Serial.println("Reconnecting...");

    reconnectWIFI();
  }
}