#include "packet.h"
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
void setup()
{
  L = callsetup[0];
  L1 = menuLang[0][0];
  L2 = menuLang[0][1];
  lcd.backlight();
  lcd.init();
  Serial.begin(9600);
  Serial1.begin(300);
  EEPROM.get(0, fx);
  // EEPROM.get(1, scrool); // 100.000 function call (get/put)
  scrool = 0;
  for (int i = 0; i < 2; i++)
  {
    pinMode(led_master[i], OUTPUT);
    pinMode(push_UP, INPUT);
    for (int w = 0; w < 16; w++)
    {
      pinMode(sw[i][w], INPUT);
    }
  }
  logversion("    AW TECH v1.0     ");
  menu = true;
}
void loop()
{
  if (C)
  {
clear001=false,clear002=false;
lcd.clear();
// return;
  }
  menu = (millis() - Waktuakhir > 80) ? true : false;
  C = (clear001 == true && clear002 == true) ? true : false;
  lcd.setBacklight((millis() - Waktuakhir > interval) ? LOW : HIGH);
  if (digitalRead(push_UP) == HIGH || digitalRead(push_DOWN) == HIGH)
  {
    lcd.clear();
    Waktuakhir = millis();
    lcd.backlight();
    setVoice();
  }
  if (handle)
  {
    dl += 500;
    lcd.clear();
    switch (scrool)
    {
    case 0:
      L = callsetup[0];
      L1 = menuLang[0][0];
      L2 = menuLang[0][1];
      break;
    case 1:
      L = callsetup[1];
      L1 = menuLang[1][0];
      L2 = menuLang[1][1];
      break;
    case 2:
      L = callsetup[2];
      L1 = menuLang[2][0];
      L2 = menuLang[2][1];
      break;
    case 3:
      L = callsetup[3];
      L1 = menuLang[3][0];
      L2 = menuLang[3][1];
      break;
    default:
      L = callsetup[0];
      L1 = menuLang[0][0];
      L2 = menuLang[0][1];
      break;
    }
    Waktuakhir = millis();
    VOICE();
  }
  callbed();
  int *a0 = (int *)&clear001,
      *a1 = (int *)&clear002,
      *a2 = (int *)&L2,
      *a3 = (int *)&menu;
  checkpoin(*a0, *a1, *a2, *a3);
  delay(20);
}