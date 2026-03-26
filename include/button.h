#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#ifndef BUTTON_H
#define size_btn 3
#define range_vertical 0b0100

struct btn_s
{
    unsigned long interval;
    unsigned long t_lastPressed;
    bool last_pressd;
    byte is_btn;
};
struct menu_dsp
{
    String variable[10];
    signed char pointer;
    bool selected;
    bool is_idleMenu;
};
enum class nav_pcc : byte
{
    home = 0x00,
    log,
    buzzer,
    none
};
struct setting_state
{
private:
    btn_s pin_b[size_btn];
    void read_btn();
    menu_dsp menu[4];
    nav_pcc pcc = nav_pcc::home;

public:
    bool show_menu = false;
    void begin(byte up, byte down, byte select, long itval_pressed);
    void main(LiquidCrystal_I2C *dsp);
    void logic_gate();
};
#endif