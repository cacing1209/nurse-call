#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define ledsize 3             // 3
#define SizeButton 0b00100000 // 32
#define Range_lcdHorizontal 0b00010100
#define Range_lcdVertical 0b0100

#include <SdFat.h>
#define SPI_SPEED SD_SCK_MHZ(4)
#define CS_PIN 10

// template <typename penambahan, typename ss>
// penambahan tambah(penambahan a, penambahan b)
// {
//     return a + b;
// }
const uint8_t Inputbutton[SizeButton] =
    {22, 23, 24, 25, 26, 27, 28, 29, 30,
     31, 32, 33, 34, 35, 36, 37,
     38, 39, 40, 41, 42, 43, 44, 45,
     46, 47, 48, 49, 50, 51, 52, 53};
enum Rolebutton
{
    Emergency,
    patient,
    Codeblue,
    empty
};
enum statusbtn
{
    btn_ON,
    btn_OFF
};
struct ledState
{
    const int led[ledsize] = {12, 11, 13};
    Rolebutton ledRole;
    void begin();
};
enum setInputbutton
{
    inputt,
    outputt,
    inputt_pull
};
struct button
{
    uint8_t BED;
    uint8_t kamar;
    uint8_t pin;
    uint32_t pressed;
    unsigned long difference;
    unsigned long pressDuration;
    bool trigger;
    Rolebutton role;
    statusbtn STATUS;
};

enum statusDisplay
{
    dsp_standby,
    dsp_ON,
    dsp_OFF,
    dsp_menu,

};

struct mainDisplay
{
    /**
     * the begin display firstly
     * initialization logo
     * setup button high of 0
     * setup the display status of satndby
     * setup value interval display sleep
     */
    void begin(const char *logoMSG, int time_Sleep, LiquidCrystal_I2C *dsp);
    button *btn;
    LiquidCrystal_I2C *lcdMsg;

    // size of all button trigger is HIGH
    uint8_t size_button_HIGH;
    // set the button HIGH to size priviouse button HIGH
    uint8_t size_button_HIGH_previous;
    // reset the value size button high
    void reset_valueH();

    // time preiviouse display off
    unsigned long timeSleep;
    // difference time value
    unsigned long timeOn;
    // interval time sleep
    unsigned long intervalSleep;
    // action display
    statusDisplay status;
    // displays the set of role arguments
    void displayAction();
    // execution of stetment diplay role
    void main();

    uint8_t clear, previouseClear;
    void functionClear();
    void transisi(LiquidCrystal_I2C *lcd);

    // name logo
    String logo;
    // range 0 - 128
    uint8_t logoTime = 100;

    // display MENU config
    bool ShowMenu = false;

    // setup show button HIGH
    void setupShowdisplay();
    // sort buttons by time
    void checkpoint_Shorting();
    String display_1[SizeButton];
};

struct SetButton_Room
{
    void MsgRead(HardwareSerial *serial, button *btn);
    String Msg_Queue;
    String MsgReadySet[SizeButton][12];
};

struct sdCard
{
    SdFat sd;
    // void begine(int baudSerial);
    HardwareSerial *serialMsg;
};

struct systemInfo
{
    void systemInformationButton(button *info_button, mainDisplay *display);
    void thread();
    uint8_t debounceLoopsystem;
};
