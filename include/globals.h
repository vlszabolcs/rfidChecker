#include <Arduino.h>

#define mOut 25
#define mIN 34
#define buzzer 27

struct {
  String name;
  String uid;
  bool loan;
  int credit;
  int time;
}userData; 


int price = 100; 
bool test = false;
int loanMax= -500;
