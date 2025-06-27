#include <Arduino.h>

uint8_t mOut = 25;
uint8_t mIN = 34;
uint8_t buzzer = 27;

struct
{
  String name;
  String uid;
  bool loan;
  int credit;
  int time;
} userData;

int price = 100;
int loanMax = -500;
bool isFree= false;

int dstOffset = 1;

bool wifiStatus = false;
String usedSSID = "";
