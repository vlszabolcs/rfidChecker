#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Firebase objektumok
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Token frissítése
void reconnectToFirebase()
{
  if (Firebase.isTokenExpired())
  {
    Firebase.refreshToken(&config);
    Serial.println("Token refreshed");
  }
}

// Firebase hibakezelés
void handleFirebaseError(String errorReason)
{
  if (errorReason == "path not exist")
  {
    Serial.println("This card is unknown");
    u8x8.clearDisplay();
    u8x8.setCursor(0, 3);
    u8x8.println("Unknown card");
  }
  else if (errorReason.indexOf("token is not ready") != -1)
  {
    Serial.printf("Hiba az adatok lekérdezésekor: %s\n", errorReason.c_str());
    Serial.println("Token expired or revoked, reconnecting...");
    u8x8.clearDisplay();
    u8x8.setCursor(0, 3);
    u8x8.println("RTDB error");
  }
}

// Időbélyeg formázása
String formatTimestamp(int timestamp)
{
  time_t rawtime = timestamp;
  struct tm *timeinfo = localtime(&rawtime);
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}

// Felhasználói adatok lekérdezése
bool getUserData(String userId)
{
  String path = userPath;
  path += userId;
  userData.uid = userId;

  Serial.printf("Adatok lekérése innen: %s\n", path.c_str());
  

  if (Firebase.RTDB.getJSON(&fbdo, path.c_str()))
  {
    if (fbdo.dataType() == "json")
    {
      FirebaseJson json = fbdo.jsonObject();
      FirebaseJsonData jsonData;

      if (json.get(jsonData, "credit"))
        userData.credit = jsonData.intValue;
      if (json.get(jsonData, "name"))
        userData.name = jsonData.stringValue;
      if (json.get(jsonData, "update"))
        userData.time = jsonData.intValue;
      if (json.get(jsonData, "loan"))
        userData.loan = jsonData.intValue;

      Serial.println("Adatok sikeresen lekérve!");
      return true;
    }
  }
  else
  {
    handleFirebaseError(fbdo.errorReason());
    reconnectToFirebase();
    return false;
  }
  return false;
}

// Felhasználói adatok frissítése
void updateUserData(String userId)
{
  String path = userPath;
  path += userId;
  FirebaseJson json;

  json.set("credit", userData.credit);
  json.set("name", userData.name);
  json.set("update", userData.time);
  json.set("loan", userData.loan);

  Serial.printf("Adatok frissítése a Firebase-ben: %s\n", path.c_str());
  reconnectToFirebase();

  if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json))
  {
    Serial.println("Adatok sikeresen frissítve!");
  }
  else
  {
    handleFirebaseError(fbdo.errorReason());
  }
}

// Felhasználói műveletek naplózása
void logUserAction(String userId, int action, int remainingCredit)
{
  String timestamp = String(timeClient.getEpochTime());
  String path = logPath;
  path += timestamp;
  FirebaseJson json;
  json.set("userId", userId);
  json.set("action", action);
  json.set("remainingCredit", remainingCredit);

  Serial.printf("Log bejegyzés készítése a Firebase-ben: %s\n", path.c_str());
  reconnectToFirebase();

  if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json))
  {
    Serial.println("Log bejegyzés sikeresen létrehozva!");
  }
  else
  {
    handleFirebaseError(fbdo.errorReason());
  }
}

// Firebase konfiguráció inicializálása
void firebaseConfig()
{
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  //auth.user.email = USER_EMAIL;
  //auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Sikeres bejelentkezés!");
    Serial.print("ID Token: ");
    Serial.println(auth.token.uid.c_str());
  }
  else
  {
    Serial.printf("Signup error: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
}
