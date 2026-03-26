#include <button.h>
setting_state::setting_state(byte up, byte down, byte select, long itval_pressd)
{
    const byte p[] = {up, down, select};
    for (size_t i = 0; i < size_btn; i++)
    {
        pin_b[i].is_btn = p[i];
        pinMode(pin_b[i].is_btn, INPUT);
        pin_b[i].interval = itval_pressd;
    }
}
void setting_state::read_btn()
{
    const long t_now = millis();
    for (size_t i = 0; i < size_btn; i++)
    {
        if (digitalRead(pin_b[i].is_btn) == HIGH && !pin_b[i].last_pressd)
        {
            if (t_now - pin_b[i].last_pressed > pin_b[i].interval)
            {
                pin_b[i].last_pressd = t_now;
                pin_b[i].last_pressd = true;
            }
        }

        if (pin_b[0].last_pressd && digitalRead(pin_b[0].is_btn) == LOW)
        {
            pin_b[0].last_pressd = false;
            dsp_menu.pointer++;
        }
        if (pin_b[1].last_pressd && digitalRead(pin_b[1].is_btn) == LOW)
        {
            pin_b[1].last_pressd = false;
            dsp_menu.pointer--;
        }
        if (pin_b[2].last_pressd && digitalRead(pin_b[2].is_btn) == LOW)
        {
            pin_b[2].last_pressd = false;
            dsp_menu.selected = true;
        }
    }
}
void setting_state::main(LiquidCrystal_I2C *dsp)
{
    is_dsp = dsp;
    read_btn();
}
