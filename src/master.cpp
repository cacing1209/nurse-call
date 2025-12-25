#include <source.h>
#include <toneBuzzer.h>
setting_t set;
systemInfo systemInformation;
File root;

mainDisplay display;
LiquidCrystal_I2C lcdMsg(0x27, Range_lcdHorizontal, Range_lcdVertical);
button myButton[SizeButton];
ledState lamp;
buttonMain main_Button;
buzzer_t bz;

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
    pinMode(buzzer, OUTPUT);
    pinMode(pinButton_menuConfirm, INPUT);
    pinMode(pinButton_menuDown, INPUT);
    pinMode(pinButton_menuUp, INPUT);
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

uint8_t menuPins()
{
    if (analogRead(pinButton_menuUp) > 500)
        return 0x01;
    else if (analogRead(pinButton_menuDown) > 500)
        return 0x02;
    else if (analogRead(pinButton_menuConfirm) > 500)
        return 0x03;
    else
        return 0x00;
}
void printMenu(signed char &cursor)
{
    for (size_t i = 0; i < Range_lcdVertical; i++)
    {
        lcdMsg.setCursor(0, i);
        lcdMsg.print(set.menuMsg[i]);
        lcdMsg.setCursor(18, i);
        lcdMsg.print((cursor == i) ? "<<" : "  ");
    }
}

void Button_main(unsigned long &defferent, bool &menu_selected)
{
    static bool press = true;
    static bool last_press = false;
    if (menuPins() == 0x01)
    {
        if (press && !last_press)
            set.cusorRole = btn_up;
        press = false;
        last_press = true;
    }
    else if (menuPins() == 0x03)
    {
        if ((press && !last_press) && !menu_selected)
        {
            menu_selected = true;
            set.cusorRole = selected_menu;
        }
        press = false;
        last_press = true;
    }
    else if (menuPins() == 0x02)
    {
        if (press && !last_press)
            set.cusorRole = btn_down;
        press = false;
        last_press = true;
    }

    else
    {
        if (!press && last_press)
        {
            press = last_press;
            last_press = !press;
        }
        if (set.cusorRole == btn_down)
            set.main_cursor(-1, set.cursor);
        if (set.cusorRole == btn_up)
            set.main_cursor(1, set.cursor);
        defferent = millis();
        set.cusorRole = notset;
    }
}

void activatedMenu(unsigned long &dif_t)
{
    static bool activMenu = false;
    if (display.ShowMenu)
        return;
    if (menuPins() == 0x01 || menuPins() == 0x02)
    {
        if (millis() - dif_t > set.debounceBtn)
            activMenu = true;
        display.timeSleep = millis();
    }
    else
    {
        if (activMenu)
        {
            activMenu = false;
            display.ShowMenu = true;
            dif_t = millis();
        }
        else
        {
            dif_t = millis();
            display.ShowMenu = false;
        }
    }
}

void mainSetting()
{
    static unsigned long dif_time = 0;
    static bool menu_select = false;
    if (display.ShowMenu)
    {
        Button_main(dif_time, menu_select);
        printMenu(set.cursor);
        }
    else
        activatedMenu(dif_time);
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
    Serial.println("Deevice Ready");
}
void loop()
{
    mainSetting();
    main_Button.Call(myButton, &display);
    display.main();
    callButton();
    IndicatorMoment(display.buttonisHIGH());
    systemInformation.thread();
}
