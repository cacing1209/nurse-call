#include <source.h>
// #include <Arduino.h>

void mainDisplay::checkpoint_Shorting()
{
    for (uint8_t indexing = 0; indexing <
                               size_button_HIGH_previous;
         indexing++)
    {
        Serial.print(String(btn[indexing].pin) + " ");
    }
    Serial.println(" ");
}

void SetButton_Room::MsgRead(HardwareSerial *serial, button *btn)
{
}

const char *PickRole_button(uint8_t indexbutton, button *btn)
{
    switch (btn[indexbutton].role)
    {
    case Emergency:
        return "E.";
    case Codeblue:
        return "C.";
    case patient:
        return "P.";
    }
}
void mainDisplay::setupShowdisplay()
{
    uint8_t total_tombolHigh = size_button_HIGH_previous;
    if (size_button_HIGH == 0)
    {
        return;
    }
    for (size_t incomingValue = 0; incomingValue <
                                   total_tombolHigh;
         incomingValue++)
    {
        display_1[incomingValue] = String(btn[incomingValue].kamar) +
                                   '-' +
                                   String(btn[incomingValue].BED) +
                                   PickRole_button(incomingValue, btn);
        Serial.print(display_1[incomingValue]);
    }
    Serial.println();
}

void mainDisplay::functionClear()
{
    static uint8_t valueQueue = 0x00;
    if (clear != previouseClear ||
        size_button_HIGH_previous != valueQueue)
    {
        previouseClear = clear;
        valueQueue = size_button_HIGH_previous;
        lcdMsg->clear();
    }
}

void mainDisplay::displayAction()
{
    functionClear();
    if (timeOn >= intervalSleep)
    {
        ShowMenu = false;
        status = dsp_OFF;
    }
    else if (ShowMenu)
    {
        status = dsp_menu;
        timeSleep = millis();
        return;
    }
    else if (size_button_HIGH_previous == 0)
    {
        ShowMenu = false;
        status = dsp_standby;
        return;
    }
    else
    {
        status = dsp_ON;
    }
}

void mainDisplay::main()
{

    displayAction();
    switch (status)
    {
    case dsp_OFF:
        lcdMsg->noBacklight();
        lcdMsg->noDisplay();
        clear = 0x00;
        break;
    case dsp_ON:
        lcdMsg->display();
        lcdMsg->backlight();
        clear = 0x01;
        break;
    case dsp_standby:
        lcdMsg->setCursor(5, 1);
        lcdMsg->print(logo);
        clear = 0x02;

        break;
    case dsp_menu:
        lcdMsg->display();
        lcdMsg->backlight();
        clear = 0x04;
        break;
    default:
        Serial.println("lcd no Roles");
        break;
    }
}
void mainDisplay::begin(const char *logoMSG, int time_Sleep, LiquidCrystal_I2C *dsp)
{
    size_button_HIGH_previous = 0b0000;
    size_button_HIGH = 0b0000;
    logo = logoMSG;
    status = dsp_standby;
    lcdMsg = dsp;
    intervalSleep = (time_Sleep <= 250) ? 1000 : time_Sleep;
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
        return "EMergency ";
    case patient:
        return "Patient ";
    case Codeblue:
        return "CodeBlue ";
    default:
        return "no role ";
        break;
    }
}

void systemInfo::systemInformationButton(button *info_button, mainDisplay *display)
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
        // Serial.print(" b " + String(millis() - second));
    }
    else
    {
        Scale_actionCheck = true;
        // Serial.println(" a " + String(millis() - second));
        debounceLoopsystem = millis() - second;
    }
}
