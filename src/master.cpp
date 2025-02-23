#include <source.h>
settings_t mainSetting;

systemInfo systemInformation;
// #define getTimebutton 3 // millisecond
// #define pinButton_menuConfirm A2
// #define pinButton_menuUp A1
// #define pinButton_menuDown A0
// #define buzzer 10

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
        digitalWrite(buzzer, LOW);
        return;
    }
    if (millis() - timepreviouse_led >= intervalLamp)
    {
        if (digitalRead(lamp.led[ledUse()]) == HIGH)
            Serial.println(ledUse());
        digitalWrite(lamp.led[ledUse()], !digitalRead(lamp.led[ledUse()]) == HIGH);
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
        {
            Serial.println(">>-- no Button");
        }
        else
        {
            displayShowButton();
        }
    }
}

void printDirectory(File dir, int numTabs)
{
    while (true)
    {

        File entry = dir.openNextFile();
        if (!entry)
        {
            // no more files
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
            // files have sizes, directories do not
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
void initSd_card()
{
    for (size_t i = 0; i < 12; i++)
    {
        sys.erorList[i] = false;
    }

    if (!SD.begin(CS_PIN))
    {
        Serial.println("CARD IS NOT DETECT");
        sys.erorList[0] = 1;
    }
    root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("");
}

uint8_t menuPins()
{
    if (analogRead(pinButton_menuUp) > 500)
    {
        return 0x01;
    }
    else if (analogRead(pinButton_menuDown) > 500)
        return 0x02;
    else if (analogRead(pinButton_menuConfirm) > 500)
        return 0x03;
    else
        return 0x00;
}

const int interval_b = display.menuSetting.intervalpressbtn;

void printMenu(String *message, signed char cursor);
void ActivatedMenu()
{
    static bool present = false;
    bool *menuActive = &display.ShowMenu;

    if (display.ShowMenu)
        return;

    if (menuPins() == 0x01 || menuPins() == 0x02 && display.menuSetting.role == subMenu)
    {
        if (millis() - display.menuSetting.TimePriviosebtn > interval_b)
        {
            Serial.println("present");
            present = true;
        }
    }
    else
    {
        display.menuSetting.TimePriviosebtn = millis();
        if (present)
        {
            *menuActive = true;
            present = false;
            display.menuSetting.pinMenu = 0x00;
            display.menuSetting.TimePriviosebtn = millis();
            printMenu(display.menuSetting.menuMSG, display.menuSetting.pinMenu);
        }
    }
}
void printMenu(String *message, signed char cursor)
{
    for (signed char i = 0; i < Range_lcdVertical; i++)
    {
        lcdMsg.setCursor(0, i);
        lcdMsg.print(message[i]);
        lcdMsg.setCursor(18, i);
        lcdMsg.print((i == cursor) ? "<<" : "  ");
    }
}
void swappingRoles_menu(signed char cursor)
{
    // static String *temp[Range_lcdVertical];
    String *message = display.menuSetting.menuMSG;

    switch (cursor)
    {
    case 0x00:
        message[0] = "Wait for", message[1] = "connection", message[2] = "\n", message[3] = "\n";
        display.menuSetting.role = setButton;
        break;
    case 0x01:
        message[0] = "Mode 1:1",
        message[1] = "Mode 1:2",
        message[2] = "Mode 2:1",
        message[3] = "Mode 2:2";
        message[4] = "Kembali";
        display.menuSetting.role = setBuzer;
        break;
    case 0x02:
        message[0] = "calling BED";
        display.menuSetting.role = eventlog;
        break;
    case 0x03:
        display.menuSetting.role = systemlog;
        break;
    default:
        return;
    }
    display.menuSetting.pinMenu = 0x00;
}
void msgMenu_Swap(bool *swap)
{
    String *manipulated = display.menuSetting.menuMSG;
    if (*swap == false)
    {
        Serial.println("yapping yaping");
        for (size_t i = 0; i < Range_lcdVertical + 1; i++)
        {
            String temp = manipulated[i];
            manipulated[i] = manipulated[i + 1];
            manipulated[i + 1] = temp;
        }
        // *swap = true;
    }
}
void updatePins(signed char pins)
{
    unsigned long current = millis();
    static bool scrollingMenu = false;

    if (current - display.menuSetting.TimePriviosebtn > interval_b)
    {
        if (pins == 0x03)
        {
            swappingRoles_menu(display.menuSetting.pinMenu);
            return;
        }

        signed char *pinMenuindex = &display.menuSetting.pinMenu;
        *pinMenuindex += pins;

        if (*pinMenuindex > 3)
        {
            if (scrollingMenu)
            {
                *pinMenuindex = 0;
                scrollingMenu = false;
            }
            else
            {
                Serial.println("ancok");
                msgMenu_Swap(&scrollingMenu);
                *pinMenuindex = 3;
            }
        }

        if (*pinMenuindex < 0)
        {
            if (scrollingMenu)
            {
                *pinMenuindex = 3;
                scrollingMenu = false;
            }
            else
            {
                *pinMenuindex = 0;
            }
        }
        display.menuSetting.TimePriviosebtn = current;
        display.timeSleep = millis();
    }
}
void main_Cursor()
{
    if (display.interuptButton(myButton))
        return;
    else if (menuPins() == 0x01)
        updatePins(1);
    else if (menuPins() == 0x02)
        updatePins(-1);
    else if (menuPins() == 0x03)
        updatePins(0x03);
    else
        display.menuSetting.TimePriviosebtn = millis();
}
void clear_dspmenu(mainDisplay *dspmenu)
{
    static uint8_t clear = 0x05;
    if (dspmenu->menuSetting.role != clear)
    {
        lcdMsg.clear();
        clear = dspmenu->menuSetting.role;
    }
}
void main_setting()
{
    ActivatedMenu();
    clear_dspmenu(&display);
    main_Cursor();
    if (display.status == dsp_menu)
        printMenu(display.menuSetting.menuMSG, display.menuSetting.pinMenu);
}
void setup()
{
    Serial1.begin(9600);
    Serial.begin(9600);
    lcdMsg.begin(20, 4);
    lcdMsg.backlight();
    lamp.begin();
    display.begin("'MZK--TECH'", 15000, &lcdMsg);
    button_Begin();
    TEST_setRole();
    // systemInformation.systemInformationButton(myButton, &display);
    display.functionClear();
    // bz.begin(buzzer, 2000, 1000); //check it
}
void loop()
{
    main_setting();
    main_Button.Call(myButton, &display);
    display.main();
    callButton();
    IndicatorMoment(display.buttonisHIGH());
    // bz.main_flipFlop(); waduh anjeng
    systemInformation.thread();
}