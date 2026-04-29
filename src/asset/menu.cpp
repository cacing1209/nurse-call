#include <source.h>
#include <Ethernet.h>
void menu(mainDisplay *dsp)
{
    dsp->lcdMsg->setCursor(0, 1);
    dsp->lcdMsg->print("");
}
