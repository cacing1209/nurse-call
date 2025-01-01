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

    statusDisplay action;
    uint16_t size_button_HIGH, size_button_HIGH_previous;
    unsigned long timeSleep,
        timeOn,
        intervalSleep;
        // 
    uint8_t clear, previouseClear;
    String logo;
    uint8_t logoTime = 100;
    String msg_ReadyQueue[Range_lcdVertical];

    bool ShowMenu = false;
    statusDisplay status;
    button *btn;
    LiquidCrystal_I2C *lcdMsg;
    void reset_valueH();
    void functionClear();
    void main();
    void transisi(LiquidCrystal_I2C *lcd);
    void begin(const char *logoMSG, int time_Sleep, LiquidCrystal_I2C *dsp);
    void setupShowdisplay();
    void checkpoint_Shorting();
    void displayAction();
    String display_1[Range_lcdVertical];
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
