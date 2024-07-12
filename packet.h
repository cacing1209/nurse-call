/*
  buzzer-3
  lcd sda-20
  lcd scl-21
  call sw 22-53
*/
#include "fx.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <DFRobotDFPlayerMini.h>
#define push_UP 3   // sw setting
#define push_DOWN 4 // sw setting
#define buz 10      // buzzer
LiquidCrystal_I2C lcd(0x27, 24, 4);
const int sw[2][16] =
    {{22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37},
     {38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53}},
          led_master[3] = {12, 11, 13};
String position[32] = {
    "bed 1",
    "bed 2",
    "bed 3",
    "bed 4",
    "bed 5",
    "bed 6",
    "bed 7",
    "bed 8",
    "bed 9",
    "bed 10",
    "bed 11",
    "bed 12",
    "bed 13",
    "bed 14",
    "bed 15",
    "bed 16",
    "bed 17",
    "bed 18",
    "bed 19",
    "bed 20",
    "bed 21",
    "bed 22",
    "bed 23",
    "bed 24",
    "bed 25",
    "kamar mandi",
    "bed 27",
    "bed 28",
    "bed 29",
    "bed 30",
    "bed 31",
    "bed 32"};
int fx;
unsigned long int timeshowPacket = 20, Waktuakhir = 0, interval = 10000, dl = 9000, noDisplay;
// display
bool selector = false, synonim = false;
int clearScrool, scrool, packet, setLANG, cursorLegt,
    lenght_1, cursorLenght2,
    clear001, clear002, interval2, prev = 0, lenght_10;
void save_memory(int addres, int nameadd) { EEPROM.put(addres, nameadd); };
bool C = false, language = false, handle = false, lenght_bawah,
     emergency = false, menu = true, handle2;
String L, L1, L2, show_interval2;
const char *red = "\033[31m";
const char *green = "\033[32m";
const char *orange = "\033[33m";
const char *reset = "\033[0m";
void buzz()
{
  int voicecover;
  switch (setLANG)
  {
  case 0:
    voicecover = NOTE_D5;
    break;
  case 1:
    voicecover = NOTE_A4;
    break;
  case 2:
    voicecover = NOTE_AS3;
    break;
  case 3:
    voicecover = NOTE_F5;
  default:
    voicecover = NOTE_DS5;
    break;
  }
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(led_master[2], HIGH);
    digitalWrite(led_master[0], LOW);
    tone(buz, voicecover);
    delay(250);
    noTone(buz);
    digitalWrite(led_master[2], LOW);
    digitalWrite(led_master[0], HIGH);
    delay(250);
  }
  for (int i = 0; i < 2; i++)
  {
    for (int x = 0; x < 16; x++)
    {
      if (digitalRead(sw[i][x]) == HIGH)
      {
        Waktuakhir = millis();
      }
    }
  }
}
void checkpoin()
{
  Serial.print(red);
  Serial.print(menu, HEX);
  Serial.print(" ");
  Serial.print(menu);
  Serial.print(" ");
  Serial.print(reset);
  Serial.print(clear001, HEX);
  Serial.print(" ");
  Serial.print(clear001);
  Serial.print(" ");
  Serial.print(clear002, HEX);
  Serial.print(" ");
  Serial.print(clear002);
  Serial.print(" ");
  Serial.print(C, HEX);
  Serial.print(" ");
  Serial.print(C);
  Serial.print(" ");
  //
  Serial.print(green);
  Serial.print(emergency, HEX);
  Serial.print(" ");
  Serial.print(emergency);
  Serial.print(" ");
  Serial.print(lenght_10, HEX);
  Serial.print(" ");
  Serial.print(lenght_10);
  Serial.print(" ");
  Serial.print(handle2, HEX);
  Serial.print(" ");
  Serial.print(handle2);
  Serial.print(" ");
  Serial.print(handle, HEX);
  Serial.println(" ");
  Serial.print(handle);
  Serial.println(" ");
  return;
}
void msg(String msg1, String msg2, int leng)
{

  int up = 1;
  int x = (lenght_10) ? 3 : (lenght_1 > 0) ? 3
                                           : 2;
  lcd.setCursor(0, 0);
  lcd.print(msg2);
  lcd.setCursor(0, up);
  lcd.print(msg1);
  lcd.setCursor(leng + x, up);
  // lcd.print("   ");
  lcd.print("   ");
  return;
}
String customName[32];
char menuLang[4][2][26] =
    {
        {"SISTEM PANGGIL", ">>DOKTER<<"},
        {"EMERGENCY SYS", ">>DOCTOR CALL<<"},
        {"OPROEP SYSTEEM", // "紧急系统",
         ">>DOKTER<<"},    // belanda
        {"LLAMADA AL SISTEMA",
         ">>DOCTOR<<"} // spanyol
};
char callsetup[4][16] = {"PANGGILAN", "CALLED", "DAGVAARDING", "CITACIÓN"};
char setupLang[4][18] = {"INDONESIAN", "ENGLISH", "BELANDA", "SPANYOL"};
char lang[4][8][13] = {
    {"SUARA 1",
     "SUARA 2",
     "SUARA 3",
     "SUARA 4"},
    {"VOICE 1",
     "VOICE 2",
     "VOICE 3",
     "VOICE 4"},
    {"STEM 1",
     "STEM 2",
     "STEM 3",
     "STEM 4"},
    {"VOTA 1",
     "VOTA 2",
     "VOTA 3",
     "VOTA 4"}};
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
void callbed()
{
  bool buzconfirm;
  String m;
  if (clear001 && clear002)
  {
    lcd.clear();
    clear001 = false, clear002 = false;
  }
  
  if(m.length()!=m.length()){lcd.clear();}
  
  for (int i = 0; i < 2; i++)
  {
    for (int b = 0; b < 16; b++)
    {
      packet = sw[i][b] - 21;
      if (digitalRead(sw[i][b]) == HIGH)
      {
        m += position[packet];
        clear001 = true;
        emergency = false;
        Waktuakhir = millis();
      }
    }
  }

  Serial.print(packet);
  Serial.print(" ");
  Serial.println(m.length());
  int menu_index = 0, lenghtLCD = 19;
  String menu_indexStr;
  menu_index = menu_indexStr.length();
  if (menu_index <= lenghtLCD)
  {
    int x, y;
    x++;
    lcd.setCursor(x, y);
    lcd.print(m);
  }
  if (menu)
  {
    switch (scrool)
    {
    case 0:
      cursorLegt = 3, cursorLenght2 = L2.length() - 5;
      break;
    case 1:
      cursorLegt = 4, cursorLenght2 = L2.length() - 12;
      break;
    case 2:
      cursorLegt = 3, cursorLenght2 = L2.length() - 5;
      break;
    case 3:
      cursorLegt = 1;
      cursorLenght2 = L2.length() - 5;
      break;
    }
    lcd.setCursor(cursorLegt, 1);
    lcd.print(L1);
    lcd.setCursor(cursorLenght2, 2);
    lcd.print(L2);
    delay(10);
    clear002 = true;
    buzconfirm = false;
  }
  else
  {
    // Serial.print(packet);
    // Serial.print(" ");
    // Serial.println(m.length());
    buzconfirm = true;
    clear001 = true;
  }
  if (buzconfirm)
  {
    buzz();
  }
  return;
}
void VOICE()
{
  while (handle)
  {
    if (!handle)
    {
      Waktuakhir = millis();
      break;
    }
    if (digitalRead(push_UP) == HIGH)
    {
      setLANG++;
      selector = true;
      if (setLANG > 3)
      {
        setLANG = 0;
      }
      delay(250);
      handle2 = false;
      interval2 = 11000;
    }
    if (digitalRead(push_DOWN) == HIGH)
    {
      Waktuakhir = millis();
      clearScrool = setLANG--;
      selector = false;
      if (setLANG < 0)
      {
        setLANG = 3;
        clearScrool = 0;
      }
      delay(250);
      handle2 = false;
      interval2 = 11000;
    }
    for (int i = 0; i < 4; i++)
    {
      lcd.setCursor(2, i);
      lcd.print(lang[scrool][i]);
      lcd.setCursor(0, setLANG);
      lcd.print(">");
      if (selector)
      {
        lcd.setCursor(0, setLANG - 1);
        lcd.print(" ");
      }
      else
      {
        lcd.setCursor(0, clearScrool);
        lcd.print(" ");
      }
    }
    if (handle2 && millis() - interval2 >= 1000)
    {
      prev = millis();
      interval2 -= 100;
    }
    show_interval2 = (millis() - interval2 > 1) ? String(interval2 / 1000) + " " : "     ";
    lcd.setCursor(17, 3);
    lcd.print(show_interval2);
    if (interval2 <= 0)
    {
      handle2 = false;
    }
    else
    {
      handle2 = true;
    }
    if (millis() - Waktuakhir > dl)
    {
      handle = false, menu = true, clear001 = true;
      interval2 = 11000;
      lcd.clear();
      handle2 = true;
    }
  }
  if (!handle)
  {
    Waktuakhir = millis();
    delay(100);
    emergency = false;
    return;
  }
}
void setVoice()
{
  delay(500);
  do
  {
    if (millis() - Waktuakhir > dl)
    {
      handle = true, selector = true,
      clearScrool = setLANG;
      handle2 = false,
      interval2 = 11000,
      Waktuakhir = millis(),
      lcd.clear();

      break;
    }
    if (digitalRead(push_UP) == HIGH)
    {
      Waktuakhir = millis();
      scrool++;
      selector = true;
      if (scrool > 3)
      {
        scrool = 0;
      }
      handle2 = false;
      interval2 = 11000;
      delay(250);
    }

    if (digitalRead(push_DOWN) == HIGH)
    {
      Waktuakhir = millis();
      clearScrool = scrool--;
      selector = false;
      if (scrool < 0)
      {
        scrool = 3;
        clearScrool = 0;
      }
      handle2 = false;
      interval2 = 11000;
      delay(250);
    }

    for (int x = 0; x < 4; x++)
    {
      lcd.setCursor(2, x);
      lcd.print(setupLang[x]);
      lcd.setCursor(0, scrool);
      lcd.print(">");
      if (selector)
      {
        lcd.setCursor(0, scrool - 1);
        lcd.print(" ");
      }
      else
      {
        lcd.setCursor(0, clearScrool);
        lcd.print(" ");
      }
    }
    if (handle2 && millis() - interval2 >= 1000)
    {
      prev = millis();
      interval2 -= 100;
    }
    show_interval2 = (millis() - interval2 > 1) ? String(interval2 / 1000) + " " : "     ";
    lcd.setCursor(17, 3);
    lcd.print(show_interval2);
    save_memory(1, scrool);
    if (interval2 <= 0)
    {
      handle2 = false;
    }
    else
    {
      handle2 = true;
    }

  } while (true);
}
void datasw()
{
  lcd.backlight();
  lcd.display();
  lcd.clear();
  handle2 = false;
  interval2 = 11000;
  Waktuakhir = millis();
  setVoice();
  return;
}