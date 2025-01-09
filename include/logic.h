#include <NTPClient.h>
#include <WiFiUdp.h>

/*00 purchase
  01 faild purchase
  
  10 modify credit
  11 modify loan
  12 modify name

  20 add user
  21 remove user
  
  30 machine busy
  31 machine ready
  
  40 modify conf free 
  41 modify conf price
  42 modify conf loan
  43 modify conf loanMax
  */
  

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

String getUID();
bool getUserData(String uid);
void updateUserData(String uid);
void dispUserData();
void successPurchase();
void disMachineBusy();
void logUserAction(String userId, int action, int remainingCredit);

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

        logUserAction(userData.uid, 0, userData.credit);

        if (!digitalRead(mIN)){ //func 
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

      disMachineBusy();
      if (cardUID != "")
      {
        /*if (getUserData(cardUID))
        {
          dispUserData();
        }*/
      }
    }
}