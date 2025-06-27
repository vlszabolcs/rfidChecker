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
    noCard();
    delay(1000);
  }
  else if (errorReason.indexOf("token is not ready") != -1)
  {
    Serial.printf("Hiba az adatok lekérdezésekor: %s\n", errorReason.c_str());
    Serial.println("Token expired or revoked, reconnecting...");
    rtdbError();
    delay(1000);
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
  path += "/credit";

  Serial.printf("Credit lekérése innen: %s\n", path.c_str());
  reconnectToFirebase();

  if (Firebase.RTDB.getInt(&fbdo, path.c_str()))
  {
    userData.credit = fbdo.intData();
    Serial.printf("Credit sikeresen lekérve: %d\n", userData.credit);
    return true;
  }
  else
  {
    handleFirebaseError(fbdo.errorReason());
    return false;
  }
}

// Felhasználói adatok frissítése
void updateUserData(String userId)
{
  String path = userPath;
  path += userId;
  FirebaseJson json;

  json.set("credit", userData.credit);
  // json.set("name", userData.name);
  json.set("update", userData.time);
  // json.set("loan", userData.loan);

  Serial.printf("Adatok frissítése a Firebase-ben: %s\n", path.c_str());
  reconnectToFirebase();

  if (Firebase.RTDB.updateNode(&fbdo, path.c_str(), &json))
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
  // auth.user.email = USER_EMAIL;
  // auth.user.password = USER_PASSWORD;
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

void getPrice() {
  String path = String(confPath) ;
  path += "price";
  Serial.printf("Price lekérése innen: %s\n", path.c_str());
  reconnectToFirebase();

  if (Firebase.RTDB.getInt(&fbdo, path.c_str())) {
    price = fbdo.intData(); 
    Serial.printf("Price sikeresen lekérve: %d\n", price);
  } else {
    handleFirebaseError(fbdo.errorReason());
    price = -1; 
    Serial.println("Price lekérdezése sikertelen, alapértelmezett érték: -1");
  }
}

void getFreeStatus() {
  String path = confPath;
  path += "free"; 
  Serial.printf("Free státusz lekérése innen: %s\n", path.c_str());
  reconnectToFirebase();

  if (Firebase.RTDB.getBool(&fbdo, path.c_str())) {
    isFree = fbdo.boolData();
    Serial.printf("Free státusz sikeresen lekérve: %s\n", free ? "true" : "false");
  } else {
    handleFirebaseError(fbdo.errorReason());
    isFree = false; 
    Serial.println("Free státusz lekérdezése sikertelen, alapértelmezett érték: false");
  }
}