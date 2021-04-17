#include "global.h"
#include "motor_driver/motor_driver.h"

Global_t Global;

void global_init()
{
    Global.pi = pigpio_start(NULL, NULL);

    /* init steppers & pins */
    // Global.all_steppers[STEPPER_GK]     = init_stepper(STEPPER_ARM_GK_STEP_PIN, STEPPER_ARM_GK_DIR_PIN); // Devre disi. SYNC Motor kontrolu yapiliyor.
    Global.all_steppers[STEPPER_SNT]    = init_stepper(STEPPER_ARM_SNT_STEP_PIN, STEPPER_ARM_SNT_DIR_PIN);

    /* servo pins */
    set_mode(Global.pi, SERVO_SNT_SIGNAL_PIN, PI_OUTPUT);
    set_mode(Global.pi, SERVO_GK_SIGNAL_PIN, PI_OUTPUT);

    // init snt arm end stop switch pin
    set_mode(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_INPUT);
    set_pull_up_down(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_PUD_UP);
}