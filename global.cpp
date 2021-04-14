#include "global.h"
#include "motor_driver/stepper_driver.h"

Global_t Global;

void global_init()
{
    Global.pi = pigpio_start(NULL, NULL);

    /* init steppers & pins */
    Global.all_steppers[STEPPER_GK]     = init_stepper(STEPPER_ARM_GK_STEP_PIN, STEPPER_ARM_GK_DIR_PIN);
    Global.all_steppers[STEPPER_SNT]    = init_stepper(STEPPER_ARM_SNT_STEP_PIN, STEPPER_ARM_SNT_DIR_PIN);

    /* init other pins */

    // init snt arm end stop switch pin
    set_mode(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_INPUT);
    set_pull_up_down(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_PUD_UP);
}