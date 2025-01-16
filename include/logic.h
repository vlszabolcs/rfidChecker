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
void checkSign();
void disMachineBusy();
void finishedPurchase();
void faildPurchase();
void logUserAction(String userId, int action, int remainingCredit);

bool minus(int credit, int price, int loanMax, bool loan)
{
  if (!(credit >= price))
  {
    if (!(loan && credit > loanMax))
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  else
  {
    return true;
  }
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
  if (!digitalRead(mIN))
  {
    if (cardUID != "")
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

          if (!digitalRead(mIN))
          {
            writeCredit();
            checkSign();
            delay(500);
            successPurchase();
            delay(3000);
          }
        }
        else
        {
          faildPurchase();
          logUserAction(userData.uid, 1, userData.credit);
          delay(1000);
        }
      }
    }
  }
  else
  {
    finishedPurchase();
    // disMachineBusy(); // learn how the machine works and how to handle the inhibit
    if (cardUID != "")
    {
      /*if (getUserData(cardUID))
      {
        dispUserData();
      }*/
    }
  }
}