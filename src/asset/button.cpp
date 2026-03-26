#include <button.h>
void setting_state::begin(byte up, byte down, byte select, long itval_pressd)
{
    const byte p[size_btn] = {up, down, select};
    for (size_t i = 0; i < size_btn; i++)
    {
        pin_b[i].is_btn = p[i];
        pinMode(pin_b[i].is_btn, INPUT);
        pin_b[i].interval = itval_pressd;
        pin_b[i].last_pressd = false;
    }
}
void setting_state::read_btn()
{
    unsigned long t_now = millis();
    static unsigned long timeout_menu = 0;
    for (size_t i = 0; i < size_btn; i++)
    {
        if (digitalRead(pin_b[i].is_btn) == HIGH && !pin_b[i].last_pressd)
        {
            if (t_now - pin_b[i].t_lastPressed > pin_b[i].interval)
            {
                pin_b[i].t_lastPressed = t_now;
                pin_b[i].last_pressd = true;
                timeout_menu = t_now;
                show_menu = true;
                if (pcc == nav_pcc::none)
                    pcc = nav_pcc::home;
                return;
            }
        }
        else
        {
            pin_b[i].t_lastPressed = t_now;
        }

        if (pin_b[0].last_pressd && digitalRead(pin_b[0].is_btn) == LOW)
        {
            pin_b[0].last_pressd = false;
            menu[static_cast<byte>(pcc)].pointer++;
            if (menu[static_cast<byte>(pcc)].pointer > 3)
                menu[static_cast<byte>(pcc)].pointer = 0;
        }
        if (pin_b[1].last_pressd && digitalRead(pin_b[1].is_btn) == LOW)
        {
            pin_b[1].last_pressd = false;
            menu[static_cast<byte>(pcc)].pointer--;
            if (menu[static_cast<byte>(pcc)].pointer == -1)
                menu[static_cast<byte>(pcc)].pointer = 3;
        }
        if (pin_b[2].last_pressd && digitalRead(pin_b[2].is_btn) == LOW)
        {
            pin_b[2].last_pressd = false;
            menu[static_cast<byte>(pcc)].selected = true;

        }
    }
    if (t_now - timeout_menu < 50000)
        menu[static_cast<byte>(pcc)].is_idleMenu = false;
    else
        menu[static_cast<byte>(pcc)].is_idleMenu = true;
}
void setting_state::main(LiquidCrystal_I2C *dsp)
{
    read_btn();
    if (!show_menu)
        return;
    dsp->backlight();
    for (size_t i = 0; i < range_vertical; i++)
    {
        dsp->setCursor(2, i);
        dsp->print(menu[static_cast<byte>(pcc)].variable[i]);
        if (i == menu[static_cast<byte>(pcc)].pointer)
        {
            dsp->setCursor(0, i);
            dsp->print('>');
        }
        else
        {
            dsp->setCursor(0, i);
            dsp->print(' ');
        }
    }
}
void setting_state::logic_gate()
{
}