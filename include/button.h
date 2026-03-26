#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#ifndef BUTTON_H
#define size_btn 3
#define range_vertical 0b0100

struct btn_s
{
    unsigned long interval;
    unsigned long last_pressed;
    bool last_pressd;
    byte is_btn;
};
struct menu_dsp
{
    String menu_home[range_vertical];
    signed char pointer;
    bool selected;
};
struct setting_state
{
private:
    btn_s pin_b[size_btn];
    LiquidCrystal_I2C *is_dsp;
    void read_btn();
    menu_dsp dsp_menu;

public:
    void main(LiquidCrystal_I2C *dsp);
    setting_state::setting_state(byte up, byte down, byte select, long itval_pressed);
};
#endif