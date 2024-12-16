#include <source.h>
// #include <Arduino.h>

void mainDisplay::checkpoint_Shorting(button *btn)
{
    for (uint8_t indexing = 0; indexing <
                               size_button_HIGH_previous;
         indexing++)
    {
        Serial.print(String(btn[indexing].pin) + " ");
    }
    Serial.println(" ");
}

void mainDisplay::begin(const char *logoMSG, int time_Sleep)
{
    size_button_HIGH_previous = 0b0000;
    size_button_HIGH = 0b0000;
    logo = logoMSG;
    status = dsp_standby;
    intervalSleep = time_Sleep;
}
void ledState::begin()
{
    for (size_t i = 0; i < 3; i++)
    {
        pinMode(led[i], OUTPUT);
    }
}
void mainDisplay::reset_valueH()
{
    // Serial.print(size_button_HIGH_previous);
    // Serial.println("length value privious");
    size_button_HIGH_previous = size_button_HIGH;
    size_button_HIGH = 0;
}

void mainDisplay::transisi(LiquidCrystal_I2C *lcd)
{
    for (size_t y = 0; y < Range_lcdVertical; y++)
    {

        for (size_t x = 0; x < Range_lcdHorizontal; x++)
        {
            lcd->setCursor(x, y);
            lcd->print("*");
            delay(25);
        }
    }
}
const char *RolesOFbutton(button *btn, uint8_t i)
{

    switch (btn[i].role)
    {
    case Emergency:
        return "EM ";
    case patient:
        return "PT ";
    case Codeblue:
        return "CD ";
    default:
        return "empty ";
        break;
    }
}

void systemInfo::systemInformation(button *info_button, mainDisplay *display)
{
    for (size_t index = 0; index <= 2; index++)
    {
        Serial.println((index == 0) ? " \n cek Roles " : " \n cek pin Range value ");
        for (size_t i = 0; i < SizeButton; i++)
        {
            switch (index)
            {
            case 0:
                Serial.print("Button pin " + String(info_button[i].pin));
                Serial.println(" Roles " + String(RolesOFbutton(info_button, i)));
                break;
            case 1:
                Serial.print(" SET " + String(1));
                Serial.println(" Value Read button : " + String(digitalRead(info_button[i].pin)));
                break;
            default:
                break;
            }
        }
    }
}
void systemInfo::thread()
{
    static unsigned long int second;
    static bool Scale_actionCheck = true;
    if (Scale_actionCheck)
    {
        Scale_actionCheck = false;
        second = millis();
        Serial.print(" b " + String(millis() - second));
    }
    else
    {
        Scale_actionCheck = true;
        Serial.println(" a " + String(millis() - second));
    }
}
//s

