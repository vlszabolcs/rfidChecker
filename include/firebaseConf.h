
#include <Firebase_ESP_Client.h>


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;






void fbTest(){
 // Adatok lekérése a "/users" táblából
  if (Firebase.RTDB.get(&fbdo, userPath.c_str())) {
    if (fbdo.dataType() == "json") {
      Serial.println("Lekért adatok:");
      Serial.println(fbdo.to<const char*>());
    } else {
      Serial.println("Nem JSON típusú adat érkezett:");
      Serial.println(fbdo.to<const char*>());
    }
  } else {
    Serial.print("Hiba a lekérési folyamatban: ");
    Serial.println(fbdo.errorReason());
  }
}

// Streaming események callback függvénye
void streamCallback(FirebaseStream data) {
  Serial.println("Változás történt az RTDB-ben!");
  Serial.print("Útvonal: ");
  Serial.println(data.dataPath());
  Serial.print("Adat: ");
  Serial.println(data.jsonString());

  // Ellenőrizd, hogy a gyökér szinten változott-e valami
  if (data.dataPath() == "/") {
    // Ha a gyökér szinten változott, dolgozd fel az adatokat
    FirebaseJson json = data.jsonObject();
    FirebaseJsonData jsonData;

    // `free` érték frissítése
    if (json.get(jsonData, "free") && jsonData.typeNum == FirebaseJson::JSON_BOOL) {
      test = jsonData.boolValue;
      Serial.print("Új 'free' érték: ");
      Serial.println(test ? "true" : "false");
    }

    // `price` érték frissítése
    if (json.get(jsonData, "price") && jsonData.typeNum == FirebaseJson::JSON_INT) {
      price = jsonData.intValue;
      Serial.print("Új 'price' érték: ");
      Serial.println(price);
    }
  } else {
    // Ha csak egy alsóbb szint változott, olvasd le újra a teljes csomópontot
    if (Firebase.RTDB.getJSON(&fbdo, "/conf")) {
      FirebaseJson json = fbdo.jsonObject();
      FirebaseJsonData jsonData;

      // `free` érték frissítése
      if (json.get(jsonData, "free") && jsonData.typeNum == FirebaseJson::JSON_BOOL) {
        test = jsonData.boolValue;
        Serial.print("Frissített 'free' érték: ");
        Serial.println(test ? "true" : "false");
      }

      // `price` érték frissítése
      if (json.get(jsonData, "price") && jsonData.typeNum == FirebaseJson::JSON_INT) {
        price = jsonData.intValue;
        Serial.print("Frissített 'price' érték: ");
        Serial.println(price);
      }
    } else {
      Serial.print("Hiba a teljes adat újraolvasásában: ");
      Serial.println(fbdo.errorReason());
    }
  }

  u8x8.clearDisplay();
  u8x8.setCursor(0,0);
  u8x8.print("Free?: ");
  if (test){
    u8x8.println(" YES!");
  }else{
    u8x8.println(" NO!");
  }

  u8x8.print("Price: ");
  u8x8.print(price);
  u8x8.println(" Ft");
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("Stream timeout történt, újracsatlakozás...");
  }
}


String formatTimestamp(int timestamp) {
  time_t rawtime = timestamp;
  struct tm* timeinfo = localtime(&rawtime);
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}


// Funkció az adatok lekérdezésére
bool getUserData(String userId) {
  String path = userPath;
  path+=userId;
  userData.uid=userId;
  Serial.print("Adatok lekérése innen: ");
  Serial.println(path);


  // Adatok lekérése
  if (Firebase.RTDB.getJSON(&fbdo, path.c_str())) {
    if (fbdo.dataType() == "json") {
      Serial.println("Adatok sikeresen lekérve!");
      
      FirebaseJson json = fbdo.jsonObject();
      FirebaseJsonData jsonData;

      // Kiolvasás és kiírás
      if (json.get(jsonData, "credit")) {
        userData.credit = jsonData.intValue;
      }

      if (json.get(jsonData, "name")) {
        userData.name = jsonData.stringValue;
      }

      if (json.get(jsonData, "update")) {
        userData.time = jsonData.intValue;
      }
      if (json.get(jsonData, "loan")) {
        userData.loan = jsonData.intValue;
      }
    return true;
    }
  } else {
      if(fbdo.errorReason()=="path not exist"){
        Serial.print("This card is unknown");
        u8x8.clearDisplay();
        u8x8.setCursor(0,3);
        u8x8.println("Unknown card");
      }else{
        Serial.print("Hiba az adatok lekérdezésekor: ");
        Serial.println(fbdo.errorReason());
        u8x8.clearDisplay();
        u8x8.setCursor(0,3);
        u8x8.println("RTDB error");
      }
    return false;
  }
}

void updateUserData(String userId) {
    String path = userPath ;
    path+=userId;
    
    // JSON objektum létrehozása
    FirebaseJson json;
    json.set("credit", userData.credit);
    json.set("name", userData.name);
    json.set("update", userData.time);
    json.set("loan",userData.loan);

    // Adatok küldése Firebase-be
    Serial.print("Adatok frissítése a Firebase-ben: ");
    Serial.println(path);

    if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) {
        Serial.println("Adatok sikeresen frissítve!");
    } else {
        Serial.print("Hiba az adatok frissítésekor: ");
        Serial.println(fbdo.errorReason());
    }
}

void logUserAction(String userId, int action, int remainingCredit) {
  // Get the current timestamp
  time_t now = timeClient.getEpochTime();
  struct tm* timeinfo = localtime(&now);
  String timestamp = String(now);
  // Create the log path with the current year
  String path = logPath;
 
  path+=timestamp;
  

  // Create the JSON object for the log
  FirebaseJson json;
  json.set("userId", userId);
  json.set("action", action);
  json.set("remainingCredit", remainingCredit);

  // Send the log to Firebase
  Serial.print("Log bejegyzés készítése a Firebase-ben: ");
  Serial.println(path);

  if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) {
    Serial.println("Log bejegyzés sikeresen létrehozva!");
  } else {
    Serial.print("Hiba a log bejegyzés létrehozásakor: ");
    Serial.println(fbdo.errorReason());
  }
}



void firebaseConfig(){
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);

  if (!Firebase.RTDB.beginStream(&fbdo, "/conf")) {
    Serial.print("Hiba a stream elindításában: ");
    Serial.println(fbdo.errorReason());
  }

  //Firebase.RTDB.setStreamCallback(&fbdo, streamCallback, streamTimeoutCallback);

  Firebase.reconnectWiFi(true);

}


