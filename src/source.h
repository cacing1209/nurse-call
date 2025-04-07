#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define ledsize 3             // 3
#define SizeButton 0b00100000 // 32
#define Range_lcdHorizontal 0b00010100
#define Range_lcdVertical 0b0100
#define baudrate_S0 9600
#define baudrate_S1 4800
#include <SD.h>
#define SPI_SPEED SD_SCK_MHZ(4)
#define CS_PIN 7
#define DB_pressButton 600 // millisecond
#define pinButton_menuConfirm A2
#define pinButton_menuUp A0
#define pinButton_menuDown A1
const int buzzer = A3;

enum status_pinMenu_t
{
    notset = 0x00,
    setButton = 0x01,
    Kembali = 0x02,
    eventlog = 0x03,
};
struct setting_t
{
    bool Action_DSP;
    signed char cursor;
    char menuMsg[Range_lcdVertical][20];
    status_pinMenu_t cusorRole;
    const int debounceBtn = 350;
    void main_cursor(signed char Mapcursor, signed char *cursor);
    setting_t()
    {
        cursor = 0;
        cusorRole = notset;
        Action_DSP = false;
    }
};

const uint8_t Inputbutton[SizeButton] = {
    22, 23, 24, 25, 26, 27, 28, 29, 30,
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
struct buzzer_t
{
    uint8_t buzzer_pin;
    int intervalOn;
    int intervalOff;
    bool flipFlopState;
    unsigned long lastTime;

    buzzer_t(); // Konstruktor
    void begin(uint8_t pin, int onTime, int offTime);
    void main_flipFlop();
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
    unsigned long int difference;
    unsigned long int pressDuration;
    Rolebutton role;
    statusbtn STATUS;
};

enum statusDisplay
{
    dsp_standby,
    dsp_ON,
    dsp_OFF,
    dsp_menu,
    dsp_updateValue,

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
    LiquidCrystal_I2C *lcdMsg;

    // size of all button trigger is HIGH
    uint8_t button_HIGH;
    // set the button HIGH to size priviouse button HIGH
    uint8_t total_HighButton;
    // reset the value size button high
    bool buttonisHIGH();
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
    bool interuptButton(button *btn);

    uint8_t clear, previouseClear;
    void functionClear();
    void transisi();

    // name logo
    String logo;
    // range 0 - 128
    uint8_t logoTime = 100;

    // display MENU config
    bool ShowMenu = false;
    String Message_button;
    // setup show button HIGH
    void setupShowdisplay(button *btn);

    // sort buttons by time
    void checkpoint_Shorting(button *btn);
    // String display_2[SizeButton];
};

struct buttonMain
{
    void Call(button *btn, mainDisplay *dsp);
    void getTime(button *btn);
    void Sorting(button *btn);
};
struct setup_button
{
    String incomingData = "";
    void ReadSerial(button *btn);
    void button_set(button *btn);
};

struct systemInfo
{
    void systemInformationButton(button *info_button, mainDisplay *display);
    void thread();
    bool erorList[12];
    uint8_t debounceLoopsystem;
};