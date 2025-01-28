#include <source.h>

void mainDisplay::checkpoint_Shorting(button *btn)
{
    for (uint8_t indexing = 0; indexing <
                               total_HighButton;
         indexing++)
    {
        Serial.print(String(btn[indexing].pin) + " ");
    }
    Serial.println(" ");
}

const char *PickRole_button(uint8_t indexbutton, button *btn)
{
    switch (btn[indexbutton].role)
    {
    case Emergency:
        return "E";
    case Codeblue:
        return "C";
    case patient:
        return "P";
    }
}

void buttonMain::getTime(button *btn)
{
    for (size_t i = 0; i < SizeButton; i++)
    {
        if (btn[i].STATUS == btn_ON)
        {
            btn[i].pressDuration = millis() - btn[i].difference;
            continue;
        }
        btn[i].difference = millis();
        btn[i].pressDuration = 0;
    }
    Sorting(btn);
}

void setMessageDisplay(mainDisplay *dsp, button *btn, uint8_t sizeButton_HIGH)
{
    String incomingMessage;
    for (size_t index = 0; index < sizeButton_HIGH; index++)
    {
        incomingMessage += String(btn[index].pin) + char('-') +
                           String(btn[index].kamar) +
                           String(PickRole_button(index, btn)) + char('.');
    }
    dsp->Message_button = incomingMessage;
}

void buttonMain::Call(button *btn, mainDisplay *dsp)
{
    for (size_t i = 0; i < SizeButton; i++)
    {
        if (digitalRead(btn[i].pin) == HIGH)
        {
            dsp->button_HIGH++;
            btn[i].STATUS = btn_ON;
            dsp->timeSleep = millis();
        }
        else
        {
            btn[i].STATUS = btn_OFF;

        }
    }
    dsp->timeOn = millis() - dsp->timeSleep;
    setMessageDisplay(dsp, btn, dsp->button_HIGH);
    if (dsp->status == dsp_OFF)
        return;
    getTime(btn);
}

void buttonMain::Sorting(button *btn)
{
    for (size_t i = 0; i < SizeButton; i++)
    {
        for (size_t j = i + 1; j < SizeButton; j++)
        {
            if (btn[i].pressDuration < btn[j].pressDuration)
            {
                button temp = btn[i];
                btn[i] = btn[j];
                btn[j] = temp;
            }
        }
    }
}

void mainDisplay::setupShowdisplay(button *btn)
{
}

void mainDisplay::functionClear()
{

    if (clear != previouseClear)
    {
        previouseClear = clear;
        lcdMsg->clear();
    }
}

void mainDisplay::displayAction()
{
    if (timeOn >= intervalSleep)
    {
        ShowMenu = false;
        status = dsp_OFF;
    }
    else if (ShowMenu)
    {
        status = dsp_menu;
        timeSleep = millis();
    }
    else if (total_HighButton == 0)
    {
        ShowMenu = false;
        status = dsp_standby;
    }
    else if (total_HighButton != button_HIGH && status == dsp_ON)
    {
        status = dsp_updateValue;
        transisi();
    }
    else
        status = dsp_ON;
    functionClear();
    reset_valueH();
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
    case dsp_updateValue:
        clear = 0x05;
        break;
    default:
        Serial.println("lcd no Roles");
        break;
    }
}
void mainDisplay::begin(const char *logoMSG, int time_Sleep, LiquidCrystal_I2C *dsp)
{
    total_HighButton = 0b0000;
    button_HIGH = 0b0000;
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
bool mainDisplay::buttonisHIGH()
{
    if (button_HIGH <= 0)
    {
        return false;
    }
    return true;
}
void mainDisplay::reset_valueH()
{
    // Serial.println(button_HIGH);
    total_HighButton = button_HIGH;
    button_HIGH = 0;
}

void mainDisplay::transisi()
{
    for (size_t y = 0; y < Range_lcdVertical; y++)
    {

        for (size_t x = 0; x < Range_lcdHorizontal; x++)
        {
            lcdMsg->setCursor(x, y);
            lcdMsg->print("*");
            delay(10);
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
