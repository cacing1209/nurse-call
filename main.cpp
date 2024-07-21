#include "packet.h"
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
void logversion(const char *text)
{
  lcd.clear();
  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 20; y++)
    {
      lcd.setCursor(y, x);
      lcd.print("#");
    }
    if (x == 1)
    {
      for (int i = 0; text[i] != '\0'; i++)
      {
        lcd.setCursor(i, x);
        lcd.print(text[i]);
        delay(50);
      }
    }
  }
  delay(2000);

  lcd.clear();
}
void setup()
{
  menu = true, selector = true, clearScrool = setLANG;
  scrool = 0;
  L = callsetup[0];
  L1 = menuLang[0][0];
  L2 = menuLang[0][1];
  lcd.backlight();
  lcd.init();
  Serial.begin(9600);
  Serial1.begin(300);
  EEPROM.get(0, fx);
  // EEPROM.get(1, scrool); // 100.000 function call (get/put)
  for (int l = 0; l < 3; l++)
  {
    pinMode(led_master[l], OUTPUT);
  }
  for (int i = 0; i < 2; i++)
  {
    pinMode(push_UP, INPUT);
    for (int w = 0; w < 16; w++)
    {
      pinMode(sw[i][w], INPUT);
    }
  }
  logversion("    AW TECH v1.0     ");
}
void loop()
{
  // Serial.println(millis() - Waktuakhir);
  menu = (millis() - Waktuakhir > 100) ? true : false;
  bool backlightState = (millis() -
   Waktuakhir > interval) ? false : true;
  if (digitalRead(push_UP) == HIGH || digitalRead(push_DOWN) == HIGH)
  {
    datasw();
  }
  if (handle)
  {
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
  standby(backlightState);
  callbed();
}