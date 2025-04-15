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
void setting_t::main_cursor(signed char Mapcursor, signed char *cursor)
{
    const byte rangeCursor = 2;
    *cursor += Mapcursor;
    if (*cursor > rangeCursor)
        *cursor = 0;
    if (*cursor == -1)
        *cursor = rangeCursor;
}

void setup_button::button_set(button *btn)
{
    uint8_t index = 0;
    uint8_t strt = 0;

    while (strt < incomingData.length() && index < SizeButton)
    {
        uint8_t dashPos = incomingData.indexOf('-', strt);
        uint8_t lastCharPos = incomingData.indexOf('.', strt);

        if (dashPos == -1 || lastCharPos == -1)
            break;
        btn[index].kamar = incomingData.substring(strt, dashPos).toInt();
        btn[index].BED = incomingData.substring(dashPos + 1, lastCharPos - 1).toInt();

        char roleChar = incomingData.charAt(lastCharPos - 1);
        switch (roleChar)
        {
        case 'E':
            btn[index].role = Emergency;
            break;
        case 'C':
            btn[index].role = Codeblue;
            break;
        case 'P':
            btn[index].role = patient;
            break;
        default:
            btn[index].role = empty;
            break;
        }

        strt = lastCharPos + 1;
        index++;
    }
}
void setup_button::ReadSerial(button *btn)
{

    while (Serial.available())
    {
        char incomingByte = Serial.read();
        incomingData += incomingByte;

        if (incomingByte == '\n')
        {
            Serial.println("Data diterima:");
            Serial.println(incomingData);
            button_set(btn);
            incomingData = "";
            delay(300);
        }
    }
}
bool mainDisplay::interuptButton(button *btn)
{

    for (size_t index = 0; index < SizeButton; index++)
    {
        if (digitalRead(btn[index].pin) == HIGH)
        {
            ShowMenu = false;
            timeSleep = millis();
            return true;
        }
    }
    return false;
}
void swap(button *xp, button *yp);
void buttonMain::getTime(button *btn)
{
    for (size_t i = 0; i < SizeButton; i++)
    {
        if (btn[i].STATUS == btn_ON)
        {
            delay(i * 2);
            btn[i].pressDuration = (millis() - btn[i].difference);
            continue;
        }
        btn[i].difference = millis();
        btn[i].pressDuration = 0;
    }
    Sorting(btn);
}

buzzer_t::buzzer_t() : buzzer_pin(0), intervalOn(1000), intervalOff(1000), flipFlopState(false), lastTime(0) {}

void buzzer_t::begin(uint8_t pin, int onTime, int offTime)
{
    buzzer_pin = pin;
    intervalOn = onTime;
    intervalOff = offTime;
    pinMode(buzzer_pin, OUTPUT);
    digitalWrite(buzzer_pin, LOW);
    lastTime = millis();
}

void buzzer_t::main_flipFlop()
{
    unsigned long current_T = millis();

    if (flipFlopState && current_T - lastTime >= intervalOn)
    {
        digitalWrite(buzzer_pin, LOW);
        flipFlopState = false;
        lastTime = current_T;
    }
    else if (!flipFlopState && current_T - lastTime >= intervalOff)
    {
        digitalWrite(buzzer_pin, HIGH);
        flipFlopState = true;
        lastTime = current_T;
    }
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
    setMessageDisplay(dsp, btn, dsp->total_HighButton);
    if (dsp->status == dsp_OFF)
        return;
    static uint8_t swHIGH;
    if (dsp->total_HighButton != swHIGH)
    {
        getTime(btn);
        swHIGH = dsp->total_HighButton;
    }
}

void swap(button *xp, button *yp)
{
    button xx = *xp;
    *xp = *yp;
    *yp = xx;
}

// swapping by time
void buttonMain::Sorting(button *btn)
{
    for (size_t i = 0; i < SizeButton; i++)
    {
        for (size_t j = i + 1; j < SizeButton; j++)
        {
            if (btn[i].pressDuration < btn[j].pressDuration)
            {
                swap(&btn[j], &btn[i]);
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
        timeSleep = millis();
        status = dsp_menu;
    }
    else if (total_HighButton == 0)
    {
        ShowMenu = false;
        status = dsp_standby;
    }
    else if (total_HighButton != button_HIGH &&
             status == dsp_ON && button_HIGH <= 13)
    {
        status = dsp_updateValue;
        transisi();
    }
    else
    {
        status = dsp_ON;
    }
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
        lcdMsg->display();
        lcdMsg->backlight();
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
    if (total_HighButton <= 0)
    {
        return false;
    }
    return true;
}
void mainDisplay::reset_valueH()
{
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
            delay(5);
        }
        delay(13);
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
            case 2:
                Serial.println("menu Setup");
            default:
                break;
            }
        }
    }
}
void systemInfo::thread()
{
    static unsigned long second;
    static bool Scale_actionCheck = true;

    if (Scale_actionCheck)
    {
        Scale_actionCheck = false;
        second = millis();
    }
    else
    {
        Scale_actionCheck = true;
        debounceLoopsystem = millis() - second;
    }
}