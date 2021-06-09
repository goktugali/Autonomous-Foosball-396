#include "global.hpp"
#include "motor_driver/motor_driver.hpp"

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

    // init led pin, initially led is high
    set_mode(Global.pi, RED_LED_PIN, PI_OUTPUT);
    gpio_write(Global.pi, RED_LED_PIN, PI_HIGH);

    // goal sensor pins
    set_mode(Global.pi, GOAL_SENSOR_HUMAN, PI_INPUT);
    set_mode(Global.pi, GOAL_SENSOR_ROBOT, PI_INPUT);

    /************************** GPIO PIN INIT *******************************/

    /************************* SYNCHRONIZATION STUFF ***********************/
    pthread_mutex_init(&Global.servo_track_mutex, NULL);
    pthread_mutex_init(&Global.servo_snt_mutex, NULL);
    pthread_mutex_init(&Global.ball_warning_mutex, NULL);
    pthread_mutex_init(&Global.game_state_mutex, NULL);
    pthread_mutex_init(&Global.servo_kicker_state_mutex, NULL);
    pthread_mutex_init(&Global.ball_warning_thread_state_mutex, NULL);
    pthread_mutex_init(&Global.db_json_file_mutex, NULL);

    pthread_cond_init(&Global.servo_track_condvar, NULL);
    pthread_cond_init(&Global.ball_warning_condvar, NULL);
    /************************* SYNCHRONIZATION STUFF ***********************/

    /************************* NETWORKING STUFF ****************************/
    Global.multicast_socket_fd      = -1;
    Global.main_server_socket_fd    = -1;
    /************************* NETWORKING STUFF ****************************/

    /************************* STATE VARIABLES ****************************/
    Global.game_play_state              = STATE_STOPPED;
    Global.servo_kicker_state           = STATE_STOPPED;
    Global.ball_warning_thread_state    = STATE_STOPPED;
    Global.main_server_thread_state     = STATE_PLAYING;
    /************************* STATE VARIABLES ****************************/

    /************************* OTHER VARIABLES ******************************/
    Global.target_kick_servo                = -1;
    Global.ball_not_found                   = 1;
    Global.stepper_speed_default            = GAME_SPEED_STEPPER_MEDIUM;
    Global.stepper_speed_multiplier_default = GAME_SPEED_MULTIPLIER_MEDIUM;
    /************************* OTHER VARIABLES ******************************/
}