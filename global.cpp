#include "global.h"

Global_t Global;

void global_init()
{
    Global.pi = pigpio_start(NULL, NULL);

    /* init steppers & pins */

    /* init other pins */
    set_mode(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_INPUT);
    set_pull_up_down(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_PUD_UP);
}