#include <source.h>

systemInfo systemInformation;
#define getTimebutton 3 // millisecond
#define pinButton_menuDown 3
#define buzzer 10
#define pinButton_menuUp 4

sdCard sdcard;
mainDisplay display;
LiquidCrystal_I2C lcdMsg(0x27, Range_lcdHorizontal, Range_lcdVertical);
button myButton[SizeButton];
ledState lamp;
buttonMain main_Button;

/**
 * --
 * test pengisian role tombol
 * HEHE (* v *)
 */

void TEST_setRole()
{
    for (int buttonIndex = 0; buttonIndex < SizeButton; buttonIndex++)
    {
        myButton[buttonIndex].kamar = buttonIndex / 3;
        myButton[buttonIndex].BED = buttonIndex % 6;

        if (buttonIndex % 3 == 0)
        {
            myButton[buttonIndex].role = Emergency;
        }
        else if (buttonIndex % 3 == 1)
        {
            myButton[buttonIndex].role = Codeblue;
        }
        else
        {
            myButton[buttonIndex].role = patient;
        }
    }
}

void buzzerON(bool isEmergency, int timeONbuzzer)
{
    if (!isEmergency)
    {
        return;
    }
    static int prev = 0;
    if (millis() - prev >= 1000)
    {
        // digitalWrite(buzzer, !(digitalRead(buzzer)));
        prev = millis();
    }
}

void button_Begin()
{

    digitalWrite(lamp.led[0], HIGH);
    delay(2500);
    for (uint8_t i = 0; i < SizeButton; i++)
    {
        if (i <= display.logo.length())
        {
            digitalWrite(lamp.led[0], LOW);

            digitalWrite(lamp.led[1], HIGH);

            lcdMsg.setCursor(i + 5, 1);
            lcdMsg.print(display.logo[i]);
            delay(display.logoTime);
        }
        digitalWrite(lamp.led[1], LOW);
        myButton[i].pin = Inputbutton[i];
        myButton[i].pressDuration = 0;
        myButton[i].difference = 0;
        myButton[i].role = empty;
        myButton[i].trigger = false;
        myButton[i].STATUS = btn_OFF;
        display.timeOn = 0;
        pinMode(myButton[i].pin, INPUT);
        digitalWrite(lamp.led[2], HIGH);
        delay(100);
        digitalWrite(lamp.led[2], LOW);
    }
    pinMode(pinButton_menuDown, INPUT);
    pinMode(pinButton_menuUp, INPUT);
    display.timeSleep = millis();
}

void ledHigh(bool btnHigh)
{
    if (!btnHigh)
    {
        digitalWrite(lamp.led[2], LOW);
        return;
    }
    static long int timePreviouse = 0;
    int delayofLEdHIGH = 500;
    if (millis() - timePreviouse >= delayofLEdHIGH)
    {
        digitalWrite(lamp.led[2], !digitalRead(lamp.led[2]) == HIGH);
        timePreviouse = millis();
    }
}

int BuzzerFlipFlop()
{
    for (size_t index = 0; index < SizeButton; index++)
    {
        if (myButton[index].role == Emergency)
        {
            return 1000;
        }
    }
    return 500;
}

void callButton()
{
    // Serial.print(String(display.total_HighButton) + ' ');

    if (display.total_HighButton != display.button_HIGH && display.button_HIGH > 0)
    {
        for (int i = 0; i < display.button_HIGH; i++)
        {
            Serial.print(String(myButton[i].pin) + ",");
        }
        Serial.println();
    }
}

bool interuptButton()
{
    for (size_t indexButton = 0; indexButton < SizeButton; indexButton++)
    {
        if (myButton[indexButton].trigger)
        {
            display.functionClear();
            return false;
        }
    }
    return true;
}

bool activationMenu()
{
    static unsigned int priviouseTime;
    if (digitalRead(pinButton_menuUp) == HIGH || digitalRead(pinButton_menuDown) == HIGH)
    {
        if (millis() - priviouseTime >= 200)
        {
            return true;
        }
    }
    else
    {
        priviouseTime = millis();
        return false;
    }
}
void setup()
{
    // Serial1.begin(300);
    Serial.begin(9600);
    lcdMsg.begin(20, 4);
    lcdMsg.backlight();
    lamp.begin();
    display.begin("'Zadikirom'", 15000, &lcdMsg);
    button_Begin();
    TEST_setRole();
    // systemInformation.systemInformationButton(myButton, &display);
    display.functionClear();
    pinMode(buzzer, OUTPUT);
}

void loop()
{
    main_Button.Call(myButton, &display);
    main_Button.getTime(myButton);
    callButton();
    display.main();
    ledHigh(display.buttonisHIGH());
    // buzzerON(display.buttonisHIGH(), BuzzerFlipFlop());
    // systemInformation.thread();
}