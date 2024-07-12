#include "packet.h"
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
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
  // C = (clear001 == true && clear002 == true) ? true : false;
  lcd.setBacklight((millis() - Waktuakhir > interval) ? LOW : HIGH);
  // checkpoin();
  menu = (millis() - Waktuakhir > 100) ? true : false;
  int backlightState = (millis() - Waktuakhir > interval) ? LOW : HIGH;
  lcd.setBacklight(backlightState);
  if (backlightState == LOW && millis() - Waktuakhir > interval + (noDisplay - 44))
  {
    digitalWrite(led_master[0], LOW);
    digitalWrite(led_master[1], HIGH);

    lcd.noDisplay();
    do
    {
      if (emergency)
      {
        break;
      }
      for (int i = 0; i < 2; i++)
      {
        for (int b = 0; b < 16; b++)
        {
          packet = sw[i][b] - 21;
          if (digitalRead(sw[i][b]) == HIGH)
          {
            if (emergency)
            {
              break;
            }
            emergency = true;
          }
          else
          {
            if (digitalRead(push_UP) == HIGH || digitalRead(push_DOWN) == HIGH)
            {
              emergency = true;
              break;
            }
          }
        }
      }
    } while (!emergency);
  }
  else
  {
    digitalWrite(led_master[1], LOW);
    digitalWrite(led_master[0], HIGH);
    lcd.display();
    noDisplay = 5000;
  }
  if (C)
  {
    lcd.clear();
    clear001 = false, clear002 = false;
    delay(50);
  }
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
  callbed();
}