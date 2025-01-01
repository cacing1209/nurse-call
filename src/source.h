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
enum statusDisplay
{
    dsp_standby,
    dsp_ON,
    dsp_OFF
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
    /**
     * Menghitung total penggunaan memori:
     *
     * 1. uint8_t pin -> 1 byte
     * 2. uint32_t pressed -> 4 byte
     * 3. unsigned long difference -> 4 byte
     * 4. unsigned long pressDuration -> 4 byte
     * 5. bool trigger -> 1 byte
     * 6. Rolebutton role -> 1 byte (asumsi enum disimpan dalam 1 byte)
     * 7. statusbtn STATUS -> 1 byte (asumsi enum disimpan dalam 1 byte)
     *
     * Total per tombol: 1 + 4 + 4 + 4 + 1 + 1 + 1 = 16 byte
     *
     * Jika terdapat 32 tombol:
     * 16 byte * 32 tombol = 512 byte
     *
     * Total dalam satuan lain:
     * - 512 byte = 0.5 KB (kilobyte)
     * - 0.5 KB = 0.00048828125 MB (megabyte)
     *
     * HEHE (* v *)
     **/
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

struct mainDisplay
{
    /**
     * uint8_t standby[3] = {0x01, 0x02, 0x03};
     * logo = stanby,startup initialized
     *
     */
    uint8_t clear, previouseClear;
    unsigned long timeSleep,
        timeOn,
        intervalSleep;

    String logo;
    uint8_t logoTime = 100;
    String msg_ReadyQueue[Range_lcdVertical];

    statusDisplay status;
    uint16_t size_button_HIGH, size_button_HIGH_previous;
    button *btn;
    LiquidCrystal_I2C *lcdMsg;
    void reset_valueH();
    void functionClear();
    void main();
    void transisi(LiquidCrystal_I2C *lcd);
    void begin(const char *logoMSG, int time_Sleep);
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
