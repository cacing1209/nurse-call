#include <source.h>
#include <toneBuzzer.h>
#include <eth.h>
systemInfo systemInformation;
File root;

mainDisplay display;
LiquidCrystal_I2C lcdMsg(0x27, Range_lcdHorizontal, Range_lcdVertical);
button myButton[SizeButton];
ledState lamp;
buttonMain main_Button;
buzzer_t bz;
void menu_btn(bool *show_menu);
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

void button_Begin()
{
    digitalWrite(lamp.led[0], HIGH);
    delay(2500);
    for (uint8_t i = 0; i < SizeButton; i++)
    {
        if (i <= strlen(display.logo))
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
    pinMode(buzzer, OUTPUT);
    pinMode(pinButton_menu, INPUT);
    display.timeSleep = millis();
}
uint8_t ledUse()
{
    for (size_t i = 0; i < display.total_HighButton; i++)
    {
        if (myButton[i].role == Codeblue)
        {
            digitalWrite(lamp.led[2], LOW);
            return 1;
        }
    }
    digitalWrite(lamp.led[1], LOW);
    return 2;
}
void IndicatorMoment(bool btnHigh)
{
    static long timepreviouse_led = 0;
    const int intervalLamp = 500;
    if (!btnHigh)
    {
        digitalWrite(lamp.led[1], LOW);
        digitalWrite(lamp.led[2], LOW);
        analogWrite(buzzer, noTone);
        return;
    }
    if (millis() - timepreviouse_led >= intervalLamp)
    {
        if (digitalRead(lamp.led[ledUse()]) == HIGH)
            analogWrite(buzzer, noteHigh01);
        else
            analogWrite(buzzer, noTone);

        digitalWrite(lamp.led[ledUse()], !(digitalRead(lamp.led[ledUse()]) == HIGH));
        timepreviouse_led = millis();
    }
}

void displayShowButton()
{
    const uint8_t sizeMSG = 9;
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
            lcdMsg.setCursor(0, i);
            lcdMsg.print(xx[i]);
        }
    }
}

void callButton()
{
    if (display.button_HIGH != display.total_HighButton)
    {

        if (display.total_HighButton == 0)
            Serial.println(">>-- no Button");
        else
            displayShowButton();
    }
}

void printDirectory(File dir, int numTabs)
{
    while (true)
    {

        File entry = dir.openNextFile();
        if (!entry)
        {
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        }
        else
        {
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

systemInfo sys;
void sdcard_READ(String namefile)
{
    File textFile = SD.open(namefile);
    if (textFile)
    {
        Serial.print(namefile);
        while (textFile.available())
        {
            Serial.write(textFile.read());
        }
        textFile.close();
    }
    else
    {
        Serial.println("error open " + namefile);
        sys.erorList[1] = 1;
    }
}
void initSd_card() // not use
{
    for (size_t i = 0; i < 12; i++)
    {
        sys.erorList[i] = false;
    }

    if (!SD.begin(CS_PIN))
    {
        Serial.println(" SD IS NOT DETECT");
        sys.erorList[0] = 1;
    }
    root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("");
}

void capture_events()
{
    static statusbtn prev[SizeButton] = {btn_OFF};
    for (size_t i = 0; i < SizeButton; i++)
    {
        if (myButton[i].STATUS == btn_ON && prev[i] == btn_OFF)
        {
            eth.push_event(myButton[i].pin,
                           myButton[i].kamar,
                           myButton[i].BED,
                           (uint8_t)myButton[i].role);
        }
        prev[i] = myButton[i].STATUS;
    }
}

void setup()
{
    Serial1.begin(baudrate_S1);
    Serial.begin(baudrate_S0);
    lcdMsg.begin(20, 4);
    lcdMsg.backlight();
    lamp.begin();
    display.begin("'MZK--TECH'", 15000, &lcdMsg);
    button_Begin();
    TEST_setRole();
    display.functionClear();
    bz.begin(buzzer, 100, 100);
    eth.begin(&bz);
    Serial.println("Deevice Ready");
}
void loop()
{
    menu_btn(&display.ShowMenu);
    main_Button.Call(myButton, &display);
    capture_events();
    display.main();
    callButton();
    IndicatorMoment(display.buttonisHIGH());
    eth.loop();
    systemInformation.thread();
}
