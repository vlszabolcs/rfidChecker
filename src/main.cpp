#include <Arduino.h>

#include <globals.h>
#include <logic.h>
#include <sss.h>
#include <dispConf.h>
#include <rfidConf.h>
#include <wifiConf.h>
#include <firebaseConf.h>
//#include <otaConf.h>
#include <buzzer.h>

bool wififlag = false;

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
  //otaConf();
}

void loop(void)
{

  //ArduinoOTA.handle();

  /*if (!Firebase.RTDB.readStream(&fbdo)) {
     Serial.print("Stream olvasási hiba: ");
     Serial.println(fbdo.errorReason());
   }*/
  // beep();

  wifiStatus = wifiMulti.run() == WL_CONNECTED;
  usedSSID = WiFi.SSID();
  if (wifiStatus)
  {
    wififlag = true;
    mainfunc();
  }
  else
  {
    if (wififlag)
    {
      Serial.println("Reconnecting...");
      reconnectWIFIDisp();
      wififlag = false;
    }
  }
}