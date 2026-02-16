#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 4 // Configurable, see typical pin layout above
#define SS_PIN 2  // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

void beepAccept();
void beepError();

String getUID()
{
  // Check if a card is present
  if (!rfid.PICC_IsNewCardPresent())
  {
    return ""; // No card detected
  }

  // Read the card
  if (!rfid.PICC_ReadCardSerial())
  {
    return ""; // Failed to read card
    beepError();
  }

  // Convert UID to string
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    if (rfid.uid.uidByte[i] < 0x10)
    {
      uid += "0"; // Add leading zero if needed
    }
    uid += String(rfid.uid.uidByte[i], HEX); // Convert byte to HEX
  }

  uid.toUpperCase(); // Convert to uppercase
  rfid.PICC_HaltA(); // Halt the card
  checkSign();
  beepAccept();
  userData.uid = uid;
  return uid;
}

void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void rfidConfig()
{
  rfid.PCD_Init(); // Init MFRC522 card
}