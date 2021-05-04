#include "global.h"
#include "motor_driver/motor_driver.h"

Global_t Global;

void global_init()
{
    /* init pigpiod daemon */
    Global.pi = pigpio_start(NULL, NULL);

    /************************** GPIO PIN INIT *******************************/
    // init steppers & pins
    // Global.all_steppers[STEPPER_GK]     = init_stepper(STEPPER_ARM_GK_STEP_PIN, STEPPER_ARM_GK_DIR_PIN); // Devre disi. SYNC Motor kontrolu yapiliyor.
    Global.all_steppers[STEPPER_SNT]    = init_stepper(STEPPER_ARM_SNT_STEP_PIN, STEPPER_ARM_SNT_DIR_PIN);

    // servo pins
    set_mode(Global.pi, SERVO_SNT_SIGNAL_PIN, PI_OUTPUT);
    set_mode(Global.pi, SERVO_GK_SIGNAL_PIN, PI_OUTPUT);

    // init stepper and endstop pin
    set_mode(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_INPUT);
    set_pull_up_down(Global.pi, ENDSTOP_ARM_SNT_PIN, PI_PUD_UP);

    // init led pin
    set_mode(Global.pi, RED_LED_PIN, PI_OUTPUT);

    /************************** GPIO PIN INIT *******************************/

    /************************* SYNCHRONIZATION STUFF ***********************/
    pthread_mutex_init(&Global.servo_track_mutex, NULL);
    pthread_mutex_init(&Global.servo_snt_mutex, NULL);
    pthread_mutex_init(&Global.ball_info_mutex, NULL);
    pthread_mutex_init(&Global.ball_warning_mutex, NULL);
    pthread_mutex_init(&Global.multicast_stream_state_mutex ,NULL);
    pthread_cond_init(&Global.servo_track_condvar, NULL);
    pthread_cond_init(&Global.ball_warning_condvar, NULL);
    /************************* SYNCHRONIZATION STUFF ***********************/

    /************************* NETWORKING STUFF ****************************/
    Global.multicast_socket_fd = -1;
    Global.multicast_stream_state = MULTICAST_STATE_STOPPED;
    /************************* NETWORKING STUFF ****************************/

    /************************* OTHER VARIABLES ******************************/
    Global.target_kick_servo = -1;
    Global.ball_not_found    = 1;

    /************************* OTHER VARIABLES ******************************/
}