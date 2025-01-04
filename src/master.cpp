#include <source.h>

systemInfo systemInformation;
#define getTimebutton 3 // millisecond
#define pinButton_menuDown 3
#define pinButton_menuUp 4

sdCard sdcard;
mainDisplay display;
LiquidCrystal_I2C lcdMsg(0x27, Range_lcdHorizontal, Range_lcdVertical);
button myButton[SizeButton];
ledState lamp;

/**
 * --
 * test pengisian role tombol
 * HEHE (* v *)
 */
void TEST_setRole()
{
    for (int x = 0; x < SizeButton; x++)
    {

        uint8_t random = rand();
        switch (x)
        {
        case 0 ... 5:
            myButton[x].kamar = 12;
            myButton[x].BED = 1;
            myButton[x].role = Emergency;
            break;
        case 6 ... 10:
            myButton[x].kamar = random;
            myButton[x].BED = 2;
            myButton[x].kamar = 1;
            myButton[x].BED = 1;
            myButton[x].role = patient;
            break;
        case 11 ... 15:
            myButton[x].kamar = random;
            myButton[x].BED = 3;
            myButton[x].role = Codeblue;
            break;
        case 16 ... 20:
            myButton[x].kamar = random;
            myButton[x].BED = 5;
            myButton[x].role = (x % 2 == 0) ? empty : patient;
            break;
        case 21 ... 25:
            myButton[x].kamar = random;
            myButton[x].BED = 6;
            myButton[x].role = Codeblue;
            break;
        case 26 ... 30:
            myButton[x].kamar = random;
            myButton[x].BED = 7;
            myButton[x].role = patient;
            break;
        default:
            myButton[x].kamar = random;
            myButton[x].BED = 10;
            myButton[x].role = (x % 2 == 0) ? empty : Emergency;
            break;
        }

        // Serial.println("Button Roles: " + String(x) + String(RolesOFbutton(x)));
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

void sorting()
{
    for (int x = 0; x < SizeButton - 1; x++)
    {
        for (int y = 0; y < SizeButton - x - 1; y++)
        {
            if (myButton[y].pressDuration < myButton[y + 1].pressDuration)
            {
                /*sorting 1 set  */

                button temp = myButton[y];
                myButton[y] = myButton[y + 1];
                myButton[y + 1] = temp;
            }
        }
    }
}

void button_difference(bool status_button, int button)
{
    if (!status_button)
    {
        myButton[button].difference = millis();
        myButton[button].pressDuration = 0;
        delay(getTimebutton);
        return;
    }
    myButton[button].pressDuration =
        millis() - myButton[button].difference;
    sorting();
    display.btn = myButton;
}

const char *btnRole_AND_ledRole(button *btn, uint8_t i)
{

    switch (btn[i].role)
    {
    case Emergency:
        return "E";
    case patient:
        return "P";
    case Codeblue:
        return "C";
    default:
        return "x";
        break;
    }
}
void callButton()
{

    for (int i = 0; i < SizeButton; i++)
    {
        button_difference(myButton[i].trigger, i);
        if (digitalRead(myButton[i].pin) == HIGH)
        {
            display.size_button_HIGH++;
            myButton[i].trigger = true;
            display.timeSleep = millis();
            myButton[i].STATUS = btn_ON;

            // display.checkpoint_Shorting(myButton);
        }

        else
        {
            myButton[i].STATUS = btn_OFF;
            myButton[i].trigger = false;
            display.timeOn = millis() - display.timeSleep;
        }
    }
    display.setupShowdisplay();
    // display.size_button_HIGH_previous = display.size_button_HIGH;
    display.reset_valueH();
}

bool interuptButton()
{
    for (size_t indexButton = 0; indexButton < SizeButton; indexButton++)
    {
        if (myButton[indexButton].trigger)
        {
            return false;
        }
    }
    return true;
}

void printMenu(uint8_t indexCursor)
{
    String menu[3] = {"1.SETUP Tombol", "2.setting Alarm", "3.system Info"};

    for (size_t x = 0; x < 3; x++)
    {
        lcdMsg.setCursor(0, x);
        lcdMsg.print(menu[x]);
    }
    lcdMsg.setCursor(0, indexCursor);
    lcdMsg.print(">");
}

bool activatedMenu(unsigned long int *timeReturnMenu)
{

    uint16_t IntervalPress = 1200;
    static unsigned long timePress = 0;
    if (digitalRead(pinButton_menuDown) == HIGH &&
        digitalRead(pinButton_menuUp) == HIGH)
    {
        if (millis() - timePress >= IntervalPress && interuptButton())
        {
            *timeReturnMenu = millis();
            timePress = millis();
            return true;
        }
    }
    else
    {
        timePress = millis();
        return false;
    }
}

int intterval = 500;
uint8_t debounceButton(uint8_t Buttonpin)

{
    static unsigned int priviouseTime = 0;
    if (digitalRead(pinButton_menuUp) == HIGH)
    {
        if (millis() - priviouseTime > intterval)
        {
            priviouseTime = millis();
            return 0x01;
        }
    }
    else if (digitalRead(pinButton_menuDown) == HIGH)
    {
        if (millis() - priviouseTime > intterval)
        {
            priviouseTime = millis();
            return 0x02;
        }
    }

    else
    {
        priviouseTime = millis();
        return 0x00;
    }
}
void cursor(unsigned long int *timeReturnMenu)
{
    static int indexCursor = 0;
    if (debounceButton(pinButton_menuUp) == 0X01)
    {
        indexCursor++;
        if (indexCursor > 2)
        {
            indexCursor = 0;
        }
        printMenu(indexCursor);
        *timeReturnMenu = millis();
    }
    else if (debounceButton(pinButton_menuDown) == 0X02)
    {
        indexCursor--;
        if (indexCursor < 0)
        {
            indexCursor = 2;
        }
        printMenu(indexCursor);
        *timeReturnMenu = millis();
    }
}

void menuSetting()
{
    static unsigned long int timebreak;
    if (activatedMenu(&timebreak) && !display.ShowMenu)
    {
        display.timeSleep = millis();
        display.ShowMenu = true;
        Serial.println("menu active");
        printMenu(0);
    }
    else if (display.status != dsp_menu || !interuptButton())
    {
        timebreak = millis();
        return;
    }
    else if (millis() - timebreak >= 10000)
    {
        Serial.println("return the menu program ");
        display.ShowMenu = false;
    }
    uint8_t show = 10 - (millis() - timebreak) / 1000;
    lcdMsg.setCursor(9, 3);
    lcdMsg.print("RETURN:" + String(show));

    cursor(&timebreak);
}

void setup()
{
    // Serial1.begin(300);
    Serial.begin(9600);
    lcdMsg.begin(20, 4);
    lcdMsg.backlight();
    lamp.begin();
    display.begin("Zadikirom", 7000, &lcdMsg);
    button_Begin();
    TEST_setRole();
    // systemInformation.systemInformationButton(myButton, &display);
}

void loop()
{
    // systemInformation.thread();
    display.main();
    menuSetting();
    callButton();
    // systemInformation.thread();
}