#include "packet.h"
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
void logversion(const char *text, const char *text2)
{
  lcd.clear();
  for (int x = 0; x < 4; x++)
  {
    switch (x)
    {
    case 1:
      for (int i = 0; text[i] != '\0'; i++)
      {
        lcd.setCursor(i, x);
        lcd.print(text[i]);
        delay(30);
      }
      break;
    case 2:
      for (int i = 0; text2[i] != '\0'; i++)
      {
        lcd.setCursor(i, x);
        lcd.print(text2[i]);
        delay(30);
      }
      break;
    default:
      for (int y = 0; y < 20; y++)
      {
        if (x == 1 || x == 2)
        {
          continue;
        }
        lcd.setCursor(y, x);
        lcd.print("#");
      }

      break;
    }
  }
  delay(2000);
  lcd.clear();
  char *animation;
  const char *name = "CCNG";
  for (int y = 0; y < 4; y++)
  {
    for (int x = 0; x < 10; x++)
    {
      animation = ">";
      lcd.setCursor(x, y);
      lcd.print(animation);
      delay(25 + (y * 9));
    }
    for (int x_ = 19; x_ > 10; x_--)
    {
      animation = "<";
      lcd.setCursor(x_, y);
      lcd.print(animation);
      // delay(25);
      delay(25 + (y * 9));
    }
    for (int i = 0; name[i] != '\0'; i++)
    {
      lcd.setCursor(10, y);
      lcd.print(name[y]);
      delay(10);
    }
    checkpoin(y);
  }
  lcd.clear();
  opator1.waktu = millis();
  return;
}
void setup()
{
  menu = true, selector = true, clearScrool = setLANG;
  lcd.backlight();
  lcd.init();
  Serial.begin(9600);
  Serial1.begin(300);
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
  logversion(">>> CACENG  v1.0 <<<",
             ">>  HELLO   ALL   <<");
}
int a, a1;
void gabut()
{
  a += rand();
  a1++;
  Serial.print((a % 3) ? green : reset);
  Serial.print(a, HEX);
  Serial.print(" ");
  Serial.print(red);
  Serial.print(a1, HEX);
  Serial.print(" ");
  if (a > 1000)
  {
    a = -1000;
  }
  else
  {
    if (a1 > 16)
    {
      a1 = 0, Serial.println();
    }
  }
  // opator1.waktu = millis(); bypas mode
}

void loop()
{
  gabut();
  // Serial.println(millis() - Waktuakhir);
  menu = (millis() - Waktuakhir > 100) ? true : false;

  if (digitalRead(push_UP) == HIGH ||
      digitalRead(push_DOWN) == HIGH)
  {
    datasw();
  }
  else if (millis() - opator1.waktu > interval && menu)
  {

    standby();
  }

  callbed();
}