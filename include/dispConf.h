#include <U8x8lib.h>


U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

void dispUserData(){
  Serial.print("Credit: ");
  Serial.println(userData.credit);
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.print("Credit: ");
  u8x8.println(userData.credit);

  Serial.print("Name: ");
  Serial.println( userData.name);
  u8x8.println( userData.name);

  
  Serial.print("Last Update (timestamp): ");
  Serial.println(userData.time);
  u8x8.print("time: ");
  u8x8.println(userData.time);
 
}

void successPurchase(){
  Serial.print("Credit: ");
  Serial.println(userData.credit);
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.print("Current Credit: ");
  u8x8.println(userData.credit);

  Serial.print("Name: ");
  Serial.println( userData.name);
  u8x8.println( userData.name);

  
  Serial.print("Last Update (timestamp): ");
  Serial.println(userData.time);
  u8x8.print("time: ");
  u8x8.println(userData.time);
 
}

void displayConfig(void)
{
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
}