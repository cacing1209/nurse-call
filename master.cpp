#include <source.h>
#define getTimebutton 3 // millisecond
mainDisplay display;
LiquidCrystal_I2C lcdMsg(0x27, Range_lcdHorizontal, Range_lcdVertical);
button myButton[SizeButton];
ledState lamp;

/**
 * test pengisian role tombol
 * HEHE (* v *)
 */

int pnj_ARR(int *source)
{
    return sizeof(source) / source[0];
}

int size_STR(String &msgSC)
{
    String *msg = &msgSC;
    return msg->length();
}

void TEST_setRole()
{
    for (int x = 0; x < SizeButton; x++)
    {
        switch (x)
        {
        case 0 ... 5:
            myButton[x].role = Emergency;
            break;
        case 6 ... 10:
            myButton[x].role = patient;
            break;
        case 11 ... 15:
            myButton[x].role = Codeblue;
            break;
        case 16 ... 20:
            myButton[x].role = (x % 2 == 0) ? empty : patient;
            break;
        case 21 ... 25:
            myButton[x].role = Codeblue;
            break;
        case 26 ... 30:
            myButton[x].role = patient;
            break;
        default:
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
    for (int i = 0; i < SizeButton; i++)
    {
        if (i <= size_STR(display.logo))
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
}

bool clearLCD(uint8_t *value_previouse, uint8_t *current_Value)
{
    if (value_previouse != current_Value)
    {
        *value_previouse = *current_Value;
        return true;
    }

    return false;
}

const char *btnRole_AND_ledRole(button *btn, uint8_t i)
{

    switch (btn[i].role)
    {
    case Emergency:
        return "EM ";
    case patient:
        return "PT ";
    case Codeblue:
        return "CD ";
    default:
        return "x ";
        break;
    }
}

uint8_t cekString(String *input, uint8_t RangeclumbStr)
{
    for (size_t i = 0; i < Range_lcdVertical; i++)
    {
        if (input[i].length() <= 20)
        {
            return i;
        }
    }
}

void queue_showBtn_2(button *btn, uint16_t sizePixel)
{
}
void queue_showBtn_1(button *btn, uint16_t sizePixel)
// CHECKPOINTTT
{
    if (sizePixel == 0)
    {
        return;
    }
    uint8_t Valueinput[Range_lcdVertical], setCursor;
    String msg_queue[Range_lcdVertical], Ready_messageBtn[Range_lcdVertical];

    for (uint8_t i = 0; i < sizePixel; i++)
    {
        Valueinput[i] = btn[i].pin - 21; // covert to index 1,2,3,4,5;
        if (i < 4)
        {
            msg_queue[0] += String(Valueinput[i]) + btnRole_AND_ledRole(btn, i);
            display.msg_ReadyQueue[0] = msg_queue[0];
            setCursor = 0x00;
        }
        else if (i < 8)
        {
            msg_queue[1] += String(Valueinput[i]) + btnRole_AND_ledRole(btn, i);
            display.msg_ReadyQueue[1] = msg_queue[1];
            setCursor = 0x01;
        }
        else if (i < 12)
        {
            msg_queue[2] += String(Valueinput[i]) + btnRole_AND_ledRole(btn, i);
            display.msg_ReadyQueue[2] = msg_queue[2];
            setCursor = 0x02;
        }
        else if (i < 16)
        {
            msg_queue[3] += String(Valueinput[i]) + btnRole_AND_ledRole(btn, i);
            display.msg_ReadyQueue[3] = msg_queue[3];
            setCursor = 0x03;
        }
    }
    for (size_t indexCursor = 0; indexCursor <= setCursor; indexCursor++)
    {
        lcdMsg.setCursor(0, indexCursor);
        lcdMsg.print(display.msg_ReadyQueue[indexCursor]);
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
            myButton[i].STATUS == btn_ON;
            // display.checkpoint_Shorting(myButton);
        }

        else
        {
            myButton[i].STATUS == btn_OFF;
            myButton[i].trigger = false;
            display.timeOn = millis() - display.timeSleep;
        }
    }

    display.size_button_HIGH_previous = display.size_button_HIGH;
    display.reset_valueH();
}

void setup();
void functionClear(mainDisplay *dsp);
void displayAction()
{
    functionClear(&display);
    if (display.timeOn >= display.intervalSleep)
    {
        display.status = dsp_OFF;
    }

    else if (display.size_button_HIGH_previous == 0)
    {
        display.status = dsp_standby;
        return;
    }

    else
    {
        display.status = dsp_ON;
    }
}

void functionClear(mainDisplay *dsp)
{
    static uint8_t valueQueue = 0;
    if (dsp->clear != dsp->previouseClear ||
        dsp->size_button_HIGH_previous != valueQueue)
    {
        dsp->previouseClear = dsp->clear;
        valueQueue = dsp->size_button_HIGH_previous;
        // dsp->transisi(&lcdMsg); // bjir masih kek taiiii

        lcdMsg.clear();
    }
}

void displayMain()
{
    displayAction();
    switch (display.status)
    {
    case dsp_OFF:
        lcdMsg.noBacklight();
        lcdMsg.noDisplay();
        display.clear = 0x00;
        break;
    case dsp_ON:
        display.clear = 0x01;
        lcdMsg.display();
        lcdMsg.backlight();
        break;
    case dsp_standby:
        display.clear = 0x02;
        lcdMsg.setCursor(5, 1);
        lcdMsg.print(display.logo);
        break;
    default:
        Serial.println("lcd no Roles");
        break;
    }
}
void setup()
{
    // systemInfo system;
    // lcdMsg.begin(20, 4);
    // lcdMsg.backlight();
    // lamp.begin();
    // display.begin("Zadikirom", 7000);
    // button_Begin();
    // TEST_setRole();
    // system.systemInformation(myButton, &display);
    Serial1.begin(300);
    Serial.begin(9600);
}
// void loop()
// {
//     systemInfo currenTimeprossed;
//     systemInfo priviouseTimeprossed;

//     currenTimeprossed.thread();
//     queue_showBtn_1(myButton, display.size_button_HIGH_previous);
//     callButton();
//     displayMain();
//     priviouseTimeprossed.thread();
// }
void checkptr(String *ptrSC)
{
    for (size_t i = 0; i < 6; i++)
    {
        Serial.println(ptrSC[i]);
    }
}

void ReadSerial(HardwareSerial *ssr)
{
    // String a = "-kamar2@EM-kamar1@PT1-kamar3@PT1-kamar1@EM-kamar1@PT2";
    String a = String(Serial.read());
    if (ssr->available() == 0)
    {
        return;
    }
    do
    {
        a += (char)ssr->read();
    } while (ssr->available() > 0);

    static uint8_t indexArr_Queue = 0;
    static String Queue[30];

    for (uint8_t i = 0; i < a.length(); i++)
    {
        if (a[i] == '-')
        {
            for (size_t IncludeStr = 0; IncludeStr < 9; IncludeStr++)
            {
                Queue[indexArr_Queue] += a[i + (IncludeStr + 1)];
            }
            delay(2000);
            checkptr(Queue);
            indexArr_Queue++;
        }
    }

    delay(2000);
}
void loop()
{
    ReadSerial(&Serial);
    
}
// -kamar2@EM-kamar1@PT1-kamar3@PT1-kamar1@EM-kamar1@PT2-kamar2@EM-kamar1@PT1-kamar3@PT1-kamar1@EM-kamar1@PT2