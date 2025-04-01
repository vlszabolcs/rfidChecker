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
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

String getUID();
bool getUserData(String uid);
void updateUserData(String uid);
void successPurchase();
void checkSign();
void waitingPurchase();
void faildPurchase(); 
void mainDisp();
void logUserAction(String userId, int action, int remainingCredit);

bool flagMain = true;

bool minus(int credit, int price, int loanMax, bool loan)
{
  return (credit >= price) || (loan && credit > loanMax);
}

void writeCredit()
{
  digitalWrite(mOut, 0);
  delay(50);
  digitalWrite(mOut, 1);
  delay(50);
  digitalWrite(mOut, 0);
  delay(50);
  digitalWrite(mOut, 1);
}

void mainfunc()
{
  String cardUID = getUID();

  if (flagMain)
  {
    Serial.println("Waiting for card...");
    mainDisp();
    flagMain = false;
  }

  waitingPurchase();

  if (!digitalRead(mIN) && !cardUID.isEmpty())
  {
    if (getUserData(cardUID))
    {
      if (minus(userData.credit, price, loanMax, userData.loan))
      {
        userData.credit -= price;
        timeClient.update();
        userData.time = timeClient.getEpochTime();
        updateUserData(userData.uid);
        logUserAction(userData.uid, 0, userData.credit);
        bool flag = true;

        if (!digitalRead(mIN))
        {
          writeCredit();
          delay(100);
          while (digitalRead(mIN))
          {
            if (flag)
            {
              successPurchase();
              flag = false;
            }
          }
        }
        flagMain = true;
      }
      else
      {
        faildPurchase();
        logUserAction(userData.uid, 1, userData.credit);
        flagMain = true;
        delay(1000);
      }
    }
    else
    {
      flagMain = true;
    }
  }
  
}