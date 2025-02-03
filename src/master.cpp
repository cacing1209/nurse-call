#include <source.h>

systemInfo systemInformation;
#define getTimebutton 3 // millisecond
#define pinButton_menuConfirm A2
#define pinButton_menuUp A1
#define pinButton_menuDown A0
#define buzzer 10

sdCard sdcard;
mainDisplay display;
LiquidCrystal_I2C lcdMsg(0x27, Range_lcdHorizontal, Range_lcdVertical);
button myButton[SizeButton];
ledState lamp;
buttonMain main_Button;

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
        myButton[i].STATUS = btn_OFF;
        display.timeOn = 0;
        pinMode(myButton[i].pin, INPUT);
        digitalWrite(lamp.led[2], HIGH);
        delay(100);
        digitalWrite(lamp.led[2], LOW);
    }
    pinMode(pinButton_menuConfirm, INPUT);
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
    if (millis() - timePreviouse >= 500)
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
            return 500;
        }
    }
    return 1000;
}

void displayShowButton()
{
    uint8_t sizeMSG = 9;
    String xx[sizeMSG];
    for (size_t indexARR = 0; indexARR < sizeMSG; indexARR++)
    {
        size_t startIdx = indexARR * 20;
        if (startIdx < display.Message_button.length())
            xx[indexARR] = display.Message_button.substring(startIdx, startIdx + 20);
    }

    for (size_t i = 0; i < sizeMSG; i++)
    {
        if (i < 4)
        {
            Serial.print(xx[i]);
            lcdMsg.setCursor(0, i);
            lcdMsg.print(xx[i]);
        }

        // if (xx[i].length() == 0)
        // {
        //     Serial.print(i);
        // }
    }
    Serial.println();
}

void callButton()
{
    if (display.button_HIGH != display.total_HighButton)
    {

        if (display.total_HighButton <= 0)
        {
            Serial.println(">>-- no Button");
        }
        else
        {
            displayShowButton();
        }
    }
}

bool interuptButton(button *btn)
{
    for (size_t index = 0; index < SizeButton; index++)
    {
        if (digitalRead(btn[index].pin) == HIGH)
        {
            display.ShowMenu = false;
            return true;
        }
    }
    return false;
}

char Cursor = '<';
String Msg[4] = {"1.Setup button",
                 "2.Alarm song",
                 "3.event Log",
                 "4.System log"};

static uint8_t pinMenu = 0x00;
static unsigned long DebouncePresButton = 0;
long interval = 900;

void updatepinMenu(int direction)
{
    if (display.status != dsp_menu)
    {
        pinMenu = 0;
    }
    else if (millis() - DebouncePresButton > 200)
    {
        pinMenu += direction;
        if (pinMenu > 3)
            pinMenu = 0;
        else if (pinMenu < 0)
            pinMenu = 3;
        lcdMsg.clear();
        DebouncePresButton = millis();
    }
}

void menu()
{
    if (interuptButton(myButton))
        return;

    if (analogRead(pinButton_menuDown) >= 500)
    {
        display.ShowMenu = true;
        display.timeSleep = millis();
        updatepinMenu(1);
    }
    else if (analogRead(pinButton_menuUp) >= 500)
    {
        display.ShowMenu = true;
        display.timeSleep = millis();
        updatepinMenu(-1);
    }
    else if (analogRead(pinButton_menuConfirm) >= 500 &&
             display.status == dsp_menu)
    {
        Serial.println(pinMenu);
    }
    else if (display.status == dsp_menu &&
             millis() - DebouncePresButton > 5000)
    {
        Serial.print(millis() - DebouncePresButton);
        Serial.println("-> no Action");
        // display.transisi();
        lcdMsg.clear();
        lcdMsg.setCursor(19, pinMenu);
        lcdMsg.print(Cursor);
        lcdMsg.setCursor(7, pinMenu);
        display.ShowMenu = false;
        lcdMsg.print("RETURN");
        delay(1500);
        return;
    }
    else if (display.status == dsp_menu)
    {

        lcdMsg.setCursor(19, pinMenu);
        lcdMsg.print(Cursor);
        for (size_t index = 0; index < 4; index++)
        {
            lcdMsg.setCursor(0, index);
            lcdMsg.print(Msg[index]);
        }
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
    menu();
    display.main();
    callButton();
    ledHigh(display.buttonisHIGH());
    buzzerON(display.buttonisHIGH(), BuzzerFlipFlop());
    systemInformation.thread();
}