#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

String getUID();
bool getUserData(String uid);
void updateUserData(String uid);
void dispUserData();
void successPurchase();
void logUserAction(String userId, String action, int remainingCredit);

int minus(int credit, int price, int loanMax, bool loan)
{
  if (credit > 0)
  {
    credit -= price;
    return credit;
  }
  else if (loan && credit > loanMax)
  {
    credit -= price;
    return credit;
  }
  else
  {
    return credit; // megoldani hogy itt ne térjen vissza semmivel
  }
}



void mainfunc()
{
  String cardUID = getUID();
  if (!digitalRead(mIN)){
    if (cardUID != ""){
      if (getUserData(cardUID)){
        userData.credit = minus(userData.credit, price, loanMax, userData.loan);
        timeClient.update();
        // int time = timeClient.getEpochTime();
        Serial.println(timeClient.getEpochTime());
        userData.time = timeClient.getEpochTime();
        updateUserData(userData.uid);

        logUserAction(userData.uid, "purchase", userData.credit);

        if (!digitalRead(mIN)){
          digitalWrite(mOut, 0);
          delay(50);
          digitalWrite(mOut, 1);
          delay(50);
          digitalWrite(mOut, 0);
          delay(50);
          digitalWrite(mOut, 1);
          successPurchase();
          // Buzz here
        }
      }
    }
  } else{

      Serial.println("Machine is busy");
      if (cardUID != "")
      {
        if (getUserData(cardUID))
        {
          dispUserData();
        }
      }
    }
}