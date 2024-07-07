/*
  buzzer-3
  lcd sda-20
  lcd scl-21
  call sw 22-53
*/
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <DFRobotDFPlayerMini.h>
#define push_UP 3
#define push_DOWN 4
LiquidCrystal_I2C lcd(0x27, 24, 4);
const int sw[2][16] =
    {{22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37},
     {38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53}},
          led_master[2] = {12, 11};
int fx;
unsigned long int timeshowPacket = 20, Waktuakhir = 0, interval = 10000, dl = 3000;
// display
bool selector = false;
int clearScrool, scrool, packet, setLANG, cursorLegt,
    lenght_1, cursorLenght2,
    clear001, clear002;
void save_memory(int addres, int nameadd) { EEPROM.put(addres, nameadd); };
bool C = false, language = false, handle = false,
     emergency = false, menu = true, lenght_10 = false;
String L, L1, L2;
const char *red = "\033[31m";
const char *green = "\033[32m";
const char *orange = "\033[33m";
const char *reset = "\033[0m";
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
  lcd.print("   ");
}
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
    {"VOTA 1 1",
     "VOTA  2",
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
        delay(80);
      }
    }
  }
  delay(2000);
  lcd.clear();
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
        lenght_1 = m.length();
        m += (lenght_1 > 0) ? "," : " ";
        m += String(packet);
        lenght_10 = (packet > 9) ? true : false;
        Waktuakhir = millis();
        clear001 = true;
      }
    }
  }
  if (menu)
  {
    clear002 = true;
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
  }
  else
  {
    clear002 = false;
    msg(m, L, lenght_1);
  }
}
void VOICE()
{

  while (handle)
  {

    if (millis() - Waktuakhir > dl)
    {
      handle = false, menu = true;
      break;
    }
    if (digitalRead(push_UP) == HIGH)
    {
      Waktuakhir = millis();
      setLANG++;
      selector = true;
      if (setLANG > 3)
      {
        setLANG = 0;
      }
      delay(250);
    }
    if (digitalRead(push_DOWN) == HIGH)
    {
      Waktuakhir = millis();
      clearScrool = setLANG--;
      dl -= 500;
      selector = false;
      if (setLANG < 0)
      {
        setLANG = 3;
        clearScrool = 0;
      }
      delay(250);
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
  }
}
void setVoice()
{
  C = true;
  do
  {
    if (millis() - Waktuakhir > dl)
    {
      handle = true, C = true, selector = true;
      clearScrool = setLANG;
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
      delay(250);
    }
    if (millis() - Waktuakhir > dl)
    {
      handle = true, C = true;
      break;
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
      Serial.print("value scrol = ");
      Serial.print(scrool);
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
    save_memory(1, scrool);

  } while (true);
}
int checkpoin(int b1, int b2, int b3, int b4)
{
  int a;
  a += 1;
  int *addres0 = &b1;
  int *addres1 = &b2;
  int *addres2 = &b3;
  int *addres3 = &b4;
  Serial.print(red);
  Serial.print(" C ");
  Serial.print(*addres0);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(*addres1);
  Serial.print(" ");
  Serial.print(reset);
  Serial.print(*addres2);
  Serial.print(" ");
  Serial.print(orange);
  Serial.print("menu ");
  Serial.print(*addres3);
  Serial.print(" ");
  if (a > 5)
  {
    Serial.println("");
    a = 0;
  }
}