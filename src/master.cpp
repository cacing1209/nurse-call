#include <source.h>

systemInfo systemInformation;
#define getTimebutton 3 // millisecond
#define pinButton_menuConfirm A2
#define pinButton_menuUp A1
#define pinButton_menuDown A0
const int buzzer = A3;

File root;

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
    if (!btnHigh)
    {
        digitalWrite(lamp.led[1], LOW);
        digitalWrite(lamp.led[2], LOW);
        analogWrite(buzzer, 0);
        return;
    }
    static long timePreviouse = 0;
    static bool buzzer_e = false;
    if (millis() - timePreviouse >= 500)
    {
        if (analogRead(buzzer) > 500)
            buzzer_e = !buzzer_e;

        analogWrite(buzzer, buzzer_e ? 1000 : 0);

        digitalWrite(lamp.led[ledUse()], !(digitalRead(lamp.led[ledUse()]) == HIGH));
        timePreviouse = millis();
    }
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
            lcdMsg.setCursor(0, i);
            lcdMsg.print(xx[i]);
        }
    }
    // for (size_t i = 0; i < 4; i++)
    // {
    //     Serial.print(xx[i]);
    // }
    // Serial.println();
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

String Cursor = "<<";
String Msg[4] = {"1.Setup button",
                 "2.Alarm song",
                 "3.event Log",
                 "4.System log"};

static signed char pinMenu = 0x00;
static unsigned long DebouncePresButton = 0;
long interval = 900;

void updatepinMenu(signed char direction)
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
        DebouncePresButton = millis();
    }
}

void activationMenu()
{
    static bool incrementKey = false;
    static uint8_t key = 0;
    if (display.ShowMenu || display.interuptButton(myButton))
        return;

    if (key == 2)
    {
        key = 0;
        display.ShowMenu = true;
        DebouncePresButton = millis();
        return;
    }

    int menuDown = analogRead(pinButton_menuDown);
    int menuUp = analogRead(pinButton_menuUp);

    unsigned long currentTime = millis();
    static unsigned long previousTime = 0, returning = 0;

    if (currentTime - returning > 1200)
        key = 0;

    if (menuDown > 500 || menuUp > 500)
    {
        if (display.status == dsp_OFF)
        {
            delay(100);
            display.timeSleep = currentTime;
            key = 0;
            return;
        }
        if (currentTime - previousTime > 300 && currentTime - previousTime < 1000)
        {
            incrementKey = true;
            previousTime = currentTime;
            display.timeSleep = currentTime;
        }
    }
    else if (incrementKey)
    {
        key++;
        incrementKey = false;
        previousTime = currentTime;
        returning = millis();
    }
    else
    {
        previousTime = currentTime;
    }
    lcdMsg.setCursor(19, 3);
    lcdMsg.print((key >= 2 || key == 0) ? " " : String(key));
}

void mainMenu(signed char *index)
{
    setup_button SerialComunication;
    lcdMsg.clear();
    unsigned long current = millis();
    static unsigned long dif_Time = millis();
    static unsigned long difT = 0;
    static uint8_t px = 0;
    const unsigned long interval_Returning = 15000;
    uint8_t speedLoop_animation = 25;
    bool cl = true;
    while (*index == 0)
    {

        if (display.interuptButton(myButton) || millis() - dif_Time > interval_Returning)
        {
            if (display.interuptButton(myButton))
                return;
            else if (millis() - dif_Time > interval_Returning + 5000)
            {
                index = 0;
                dif_Time = current;
                DebouncePresButton = millis();
                lcdMsg.clear();
                return;
            }
            else if (cl)
            {
                lcdMsg.clear();
                cl = false;
            }
            else
            {
                lcdMsg.setCursor(6, 1);
                lcdMsg.print(">RETURN<");
                lcdMsg.setCursor(4, 2);
                lcdMsg.print("NOT CONNECTED");
            }
        }
        else if (millis() - difT > speedLoop_animation)
        {
            if (millis() - dif_Time > interval_Returning - 5000)
            {
                lcdMsg.setCursor(17, 0);
                lcdMsg.print(15 - ((millis() - dif_Time) / 1000));
            }
            lcdMsg.setCursor(6, 0);
            lcdMsg.print("Wait for");
            lcdMsg.setCursor(6, 1);
            lcdMsg.print("Conection");
            lcdMsg.setCursor(px, 2);
            lcdMsg.print("*");
            lcdMsg.setCursor(19 - px, 3);
            lcdMsg.print("*");
            px++;
            difT = millis();
        }
        else if (px == 20)
        {
            for (size_t xx = 0; xx < 20; xx++)
            {
                Serial.println(20 - xx);
                lcdMsg.setCursor(xx, 2);
                lcdMsg.print(" ");
                lcdMsg.setCursor(19 - xx, 3);
                lcdMsg.print(" ");
            }
            px = 0;
        }
        SerialComunication.ReadSerial(myButton);
    }
}
void menu()
{
    activationMenu();
    if (display.interuptButton(myButton))
        return;

    else if (analogRead(pinButton_menuDown) >= 500 && display.ShowMenu)
    {
        updatepinMenu(1);
        display.timeSleep = millis();
    }
    else if (analogRead(pinButton_menuUp) >= 500 && display.ShowMenu)
    {
        updatepinMenu(-1);
        display.timeSleep = millis();
    }
    else if (analogRead(pinButton_menuConfirm) >= 500 &&
             display.status == dsp_menu)
    {
        mainMenu(&pinMenu);
    }
    else if (display.status == dsp_menu &&
             millis() - DebouncePresButton > 5000)
    {
        Serial.print(millis() - DebouncePresButton);
        Serial.println("-> no Action");
        // display.transisi();
        lcdMsg.clear();
        lcdMsg.setCursor(18, pinMenu);
        lcdMsg.print(Cursor);
        lcdMsg.setCursor(7, pinMenu);
        display.ShowMenu = false;
        lcdMsg.print("RETURN");
        delay(1500);
        return;
    }

    if (display.status == dsp_menu)
    {
        for (signed char index = 0; index < 4; index++)
        {
            lcdMsg.setCursor(0, index);
            lcdMsg.print(Msg[index]);
            lcdMsg.setCursor(18, index);
            lcdMsg.print(pinMenu == index ? Cursor : "  ");
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

void setup()
{
    Serial1.begin(9600);
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
    IndicatorMoment(display.buttonisHIGH());
    systemInformation.thread();
}