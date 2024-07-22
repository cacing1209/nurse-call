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
    "Ebed 26",
    "bed 27",
    "bed 28",
    "bed 29",
    "bed 30",
    "bed 31",
    "bed 32"};
int fx, menu_index;
unsigned long int timeshowPacket = 20, Waktuakhir = 0, interval = 10000, dl = 9000, noDisplay;
// display
unsigned int lenghtLCD = 19, clear;
bool selector = false;
int clearScrool, scrool, packet, setLANG, cursorLegt,
    lenght_1, cursorLenght2,
    clear001, clear002, interval2, prev = 0, lenght_10;
bool language = false, handle = false,
     menu = true, handle2;
String L, L1, L2, show_interval2;
const char *red = "\033[31m";
const char *green = "\033[32m";
const char *orange = "\033[33m";
const char *reset = "\033[0m";

void save_memory(int addres, int nameadd) { EEPROM.put(addres, nameadd); };
void buzz(bool check)
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
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(led_master[2], HIGH);
    digitalWrite(led_master[0], LOW);
    tone(buz, voicecover);
    delay(200);
    noTone(buz);
    digitalWrite(led_master[2], LOW);
    digitalWrite(led_master[0], HIGH);
    delay(200);
  }
  if (check)
  {
    return;
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
void startmenu(int value)
{
  switch (value)
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
}
struct stb
{
  bool wakeup;
  bool sleep;
  unsigned long int waktu = 0;
};
stb opator1;
void standby()
{

  while (opator1.sleep)
  {
    // lcd.noBacklight();
    if (opator1.wakeup)
    {
      lcd.backlight();
      lcd.display();
      for (int i = 0; i < 100; i += 10)
      {
        opator1.waktu = millis();
        delay(i);
      }
      break;
    }
    else
    {
      if ((millis() - opator1.waktu) > (interval + 5000))
      {
        lcd.noDisplay();
      }
      lcd.noBacklight();
    }
    for (int x = 0; x < 2; x++)
    {
      for (int y = 0; y < 16; y++)
      {
        if (digitalRead(sw[x][y]) == HIGH ||
            digitalRead(push_DOWN) == HIGH ||
            digitalRead(push_UP) == HIGH)
        {
          opator1.wakeup = true;
        }
      }
    }
  }
}
void callbed()
{
  String m;
  for (int i = 0; i < 2; i++)
  {
    for (int b = 0; b < 16; b++)
    {
      packet = sw[i][b] - 21;
      if (digitalRead(sw[i][b]) == HIGH)
      {
        m += position[packet - 1];
        clear001 = true;
        Waktuakhir = millis();
        opator1.waktu = millis();
      }
    }
  }
  if (m.length() != clear)
  {
    Serial.println("|<<<============================>>>|");
    lcd.clear();
    clear = m.length();
  }
  if (menu)
  {
    opator1.wakeup = false;
    opator1.sleep = true;
    clear001 = false;
    if (clear001 != clear002)
    {
      lcd.clear();
      clear002 = clear001;
    }
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
    // buzconfirm = false;
  }
  else
  {

    String length_chr;
    int x, y;
    if (clear001 != clear002)
    {
      lcd.clear();
      clear002 = clear001;
    }
    length_chr = m;
    menu_index = length_chr.length();
    if (length_chr.length() <= lenghtLCD)
    {
      lcd.setCursor(x, y);
      lcd.print(m);
    }
    else
    {
    }
    buzz(false);
    Serial.println(length_chr);
  }
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
    else if (digitalRead(push_UP) == HIGH)
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
    else if (digitalRead(push_DOWN) == HIGH)
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
      handle = false, menu = true;
      interval2 = 11000;
      lcd.clear();
      handle2 = true;
      opator1.waktu = millis();
    }
  }
  if (!handle)
  {
    Waktuakhir = millis();
    delay(100);
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
      lcd.clear();
      save_memory(0, scrool);
      save_memory(1, setLANG);
      Waktuakhir = millis();
      startmenu(scrool);
      VOICE();
      break;
    }
    else if (digitalRead(push_UP) == HIGH)
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

    else if (digitalRead(push_DOWN) == HIGH)
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
  lcd.clear();
  handle2 = false;
  interval2 = 11000;
  Waktuakhir = millis();
  setVoice();
  // return;
}

// typedef struct
// {
//   unsigned long int waktu,
//       standbyTimer;
//   bool emrgncy = false;

// } data;
// void hibernate()
// {
//   data p2;
//   if (p2.waktu > p2.standbyTimer)
//   {
//     if (p2.emrgncy)
//     {
//       return;
//     }
//     lcd.clear();
//     lcd.noDisplay();
//   }
// }
// void standby(int *wktu)
// {
//   data p1;
//   p1.standbyTimer = 15000;
//   p1.waktu = millis();
//   for (int i = 0; i < 2; i++)
//   {
//     for (int x = 0; x < 16; x++)
//     {
//       if (digitalRead(sw[i][x]) == HIGH)
//       {
//         p1.emrgncy = true;
//         p1.waktu = millis();
//       }
//     }
//   }
//   hibernate();
// }
void checkpoin(int value)
{
  char *message;
  // String message;
  switch (value)
  {
  case 0:
    Serial.print(reset);
    Serial.println("STATUS PIN");
    for (int x = 0; x < 2; x++)
    {
      for (int y = 0; y < 16; y++)
      {
        Serial.print(
            (digitalRead(sw[x][y]) == HIGH) ? reset : red);
        Serial.print(sw[x][y] - 21);
        Serial.print((digitalRead(sw[x][y]) == HIGH) ? " HIGH " : " LOW ");
      }
      Serial.println();
    }
    break;
    case 1:
    startmenu(scrool);
    break;
  }
  return;
}