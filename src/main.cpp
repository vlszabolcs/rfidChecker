#include <Arduino.h>

#include <globals.h>
#include <logic.h>
#include <sss.h>
#include <dispConf.h>
#include <rfidConf.h>
#include <wifiConf.h>
#include <firebaseConf.h>

void setup(void){
  Serial.begin(115200);
  SPI.begin();
  displayConfig();
  //readConfigFromSD();
  wifiConfig();
  ntpConf();

  pinMode(mIN,INPUT);
  pinMode(mOut,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(mOut,1);
  digitalWrite(buzzer,0);
  
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.println("Fasz");

  firebaseConfig();
  rfidConfig();
}

void loop(void){
 /*if (!Firebase.RTDB.readStream(&fbdo)) {
    Serial.print("Stream olvasási hiba: ");
    Serial.println(fbdo.errorReason());
  }*/

  if(WiFi.getAutoReconnect()){
      mainfunc();
  }else{
    Serial.println("WiFi connection error");
  } 
}