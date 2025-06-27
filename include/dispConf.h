#include <U8x8lib.h>

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

void checkSign()
{
  Serial.println("RFID: OK");
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_open_iconic_check_4x4);
  u8x8.drawGlyph(6, 2, 'C'); // Replace 'A' with the appropriate character code for the glyphs
}

void successPurchase()
{
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.println("Egyenleg:");
  u8x8.setFont(u8x8_font_profont29_2x3_f);
  u8x8.setCursor(1, 3);
  u8x8.print(userData.credit);
  u8x8.println(" Ft");
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  Serial.print("Name: ");
  Serial.println(userData.name);

  Serial.print("Last Update (timestamp): ");
  Serial.println(userData.time);
}
void faildPurchase()
{
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_open_iconic_check_4x4);
  u8x8.drawGlyph(6, 2, 'B'); // Replace 'A' with the appropriate character code for the glyph
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(3, 0);
  u8x8.println("Nincs kredit: ");
  u8x8.println(userData.credit);
  Serial.println("Nincs kredit");
}

void noCard(){
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_open_iconic_check_4x4);
  u8x8.drawGlyph(6, 2, 'B'); // Replace 'A' with the appropriate character code for the glyph
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(3, 0);
  u8x8.println("Nem regisztrált: ");
  u8x8.println(userData.uid);
  Serial.println("Nincs ilyen kártya");
}

void rtdbError(){
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_open_iconic_check_4x4);
  u8x8.drawGlyph(6, 2, 'B'); // Replace 'A' with the appropriate character code for the glyph
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(3, 0);
  u8x8.println("RTDB error!");
  Serial.println("RTDB error");
}

void mainDisp()
{
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(4, 3);
  u8x8.println("Fizess!");
}

void freeModeDisp()
{
  u8x8.clearDisplay();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(4, 3);
  u8x8.println("FREE!");
}

void waitingPurchase()
{
  static String lastTime = "";
  timeClient.update();
  time_t localTime = timeClient.getEpochTime() + dstOffset * 3600;

  struct tm *timeinfo = localtime(&localTime);
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  String currentTime = String(timeStr);

  if (currentTime != lastTime)
  {
    u8x8.setCursor(0, 0);
    u8x8.clearLine(0);
    u8x8.println(usedSSID);

    lastTime = currentTime;
    u8x8.clearLine(6);
    u8x8.setCursor(4, 6);
    u8x8.print(currentTime);
  }
}

void reconnectWIFIDisp()
{
  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.println("Reconnecting...");
}

void displayConfig(void)
{
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}