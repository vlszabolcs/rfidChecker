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

void disMachineBusy(){
  Serial.println("Machine is busy");
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.println("A gép elfoglalt");
}

void checkSign(){
  Serial.print("Credit: ");
  Serial.println(userData.credit);
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.setFont(u8x8_font_open_iconic_check_8x8);
  u8x8.drawGlyph(4, 0, 'A'); // Replace 'A' with the appropriate character code for the glyph
}

void successPurchase(){
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0,0);
  u8x8.println("Egyenleg:");
  u8x8.setFont(u8x8_font_profont29_2x3_f);
  u8x8.setCursor(1,3);
  u8x8.print(userData.credit);
  u8x8.println(" Ft");
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  Serial.print("Name: ");
  Serial.println( userData.name);

  Serial.print("Last Update (timestamp): ");
  Serial.println(userData.time);
  
}
void faildPurchase(){
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0,0);
  u8x8.println("Nincs kredit: ");
  u8x8.println(userData.credit);
  Serial.println("Nincs kredit");
}


void finishedPurchase(){
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0,0);
  u8x8.println("Válassz kávét!");
  u8x8.setFont(u8x8_font_open_iconic_arrow_8x8);
  u8x8.drawGlyph(2, 0, 'A'); // Replace 'A' with the appropriate character code for the glyph

  Serial.println("Felhasználóra várakozik");
}

void reconnectWIFI(){
  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.println("WiFi connection error");
  u8x8.println("Reconnecting...");
}

void displayConfig(void)
{
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r); 
}