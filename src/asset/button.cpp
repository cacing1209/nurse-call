#include <source.h>

void menu_btn(bool *show_menu)
{
    static unsigned long last_t = 0;
    const unsigned long now_t = millis();
    unsigned long different_t = now_t - last_t;
    bool press = (digitalRead(pinButton_menu) == HIGH);
    static bool last_pressed = false;
    if (press && !last_pressed)
    {
        if (different_t > 2500)
        {
            last_t = now_t;
            last_pressed = press;
        }
    }
    else if (!press && last_pressed)
    {
        last_pressed = false;
        *show_menu = true;
        Serial.println("show menu" + String(*show_menu));
    }
}