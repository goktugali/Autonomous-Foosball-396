#include "game_control.hpp"
#include <fcntl.h>

void align_arms(uint16_t ball_x, uint16_t ball_y, uint16_t stepper_pos)
{
    /* If any goal risk from opponent, go defence position */
    /*
    if(ball_x < DEFENCE_POSITON_MAX && ball_x> DEFENCE_POSITION_MIN)
    {
        if(ball_y > FIELD_Y_LENGTH)
            arm_move_sync((FIELD_Y_LENGTH / 2) + 20);
        else if(ball_y < FIELD_Y_LENGTH)
            arm_move_sync((FIELD_Y_LENGTH / 2) -  20);
        else
            arm_move_sync((FIELD_Y_LENGTH / 2));
        return;
    }
     */

    int16_t differences[3];
    uint16_t second_kicker_pos  = stepper_pos;
    uint16_t first_kicker_pos   = stepper_pos - DISTANCE_BETWEEN_KICKERS;
    uint16_t third_kicker_pos   = stepper_pos + DISTANCE_BETWEEN_KICKERS;

    uint16_t target_pos;
    int16_t min_difference;
    int16_t difference_first_kicker     = abs(first_kicker_pos - ball_y);
    int16_t difference_second_kicker    = abs(second_kicker_pos - ball_y);
    int16_t difference_third_kicker     = abs(third_kicker_pos - ball_y);

    differences[0] = difference_first_kicker;
    differences[1] = difference_second_kicker;
    differences[2] = difference_third_kicker;

    /* sort the differences */
    sort_diff(differences);
    //printf("Motor pos : %d\n", stepper_pos);
    //printf("Birinci adam konum : %d\n", first_kicker_pos);
    //printf("ikinci adam konum : %d\n", second_kicker_pos);
    //printf("ucuncu adam konum : %d\n", third_kicker_pos);
    for (int i = 0; i < 3; ++i)
    {
        if(differences[i] == difference_first_kicker)
        {
            //printf("En yakin birinci adam \n");

            //printf("Birinci adam mesafe : %d\n", differences[i]);
            if(first_kicker_pos < ball_y)
            {
                target_pos = second_kicker_pos + differences[i];
            }
            else
            {
                target_pos = second_kicker_pos - differences[i];
            }

        }
        else if(differences[i] == difference_second_kicker)
        {

            //printf("En yakin ikinci adam \n");
            //printf("ikinci adam mesafe : %d\n", differences[i]);

            if(second_kicker_pos < ball_y)
            {
                target_pos = second_kicker_pos + differences[i];
            }
            else
            {
                target_pos = second_kicker_pos - differences[i];
            }
        }
        else if(differences[i] == difference_third_kicker)
        {
            //printf("En yakin ucuncu adam \n");
            //printf("ucuncu adam mesafe : %d\n", differences[i]);

            if(third_kicker_pos < ball_y)
            {
                target_pos = second_kicker_pos + differences[i];
            }
            else
            {
                target_pos = second_kicker_pos - differences[i];
            }
        }

        if(target_pos > STEPPER_MIN_POSITION && target_pos < STEPPER_MAX_POSITION) {
            //printf("Hareket komutu gonderildi %d \n", target_pos);
            arm_move_sync(target_pos);
            break;
        }
    }
}

void align_servo_positions(uint16_t ball_x)
{
    int servo = is_kick_need(ball_x);
    if(-1 != servo)
        trigger_servo_kick(servo);
}
int is_kick_need(uint16_t ball_x)
{
    /* If ball is closer to the GK, arm up the SNT */
    if(ball_x < SNT_KICK_DISTANCE_MIN ){
        pthread_mutex_lock(&Global.servo_snt_mutex);
        if(SERVO_SNT_HOME_POSITION - SERVO_KICK_DISTANCE != get_servo_pulsewidth(Global.pi, SERVO_SNT_SIGNAL_PIN))
            set_servo_pulsewidth(Global.pi, SERVO_SNT_SIGNAL_PIN, SERVO_SNT_HOME_POSITION - SERVO_KICK_DISTANCE);
        pthread_mutex_unlock(&Global.servo_snt_mutex);
    }
    else{
        pthread_mutex_lock(&Global.servo_snt_mutex);
        if(SERVO_SNT_HOME_POSITION != get_servo_pulsewidth(Global.pi, SERVO_SNT_SIGNAL_PIN))
            set_servo_pulsewidth(Global.pi, SERVO_SNT_SIGNAL_PIN, SERVO_SNT_HOME_POSITION);
        pthread_mutex_unlock(&Global.servo_snt_mutex);
    }

    if(ball_x < SNT_KICK_DISTANCE_MAX && ball_x > SNT_KICK_DISTANCE_MIN){
        return SERVO_SNT;
    }
    if(ball_x < GK_KICK_DISTANCE_MAX && ball_x > GK_KICK_DISTANCE_MIN){
        return SERVO_GK;
    }

    return -1;
}

void* servo_kicker_thread_func(void* arg)
{
    pthread_mutex_lock(&Global.servo_kicker_state_mutex);
    if(STATE_STOPPED == Global.servo_kicker_state)
        Global.servo_kicker_state = STATE_PLAYING;
    pthread_mutex_unlock(&Global.servo_kicker_state_mutex);

    // Home all servos
    servo_GK_home();
    servo_SNT_home();

    printf("Servo kicker thread started\n");
    while(true)
    {
        pthread_mutex_lock(&Global.servo_track_mutex);
        if(-1 == Global.target_kick_servo)
            pthread_cond_wait(&Global.servo_track_condvar, &Global.servo_track_mutex);

        int target_servo = Global.target_kick_servo;

        // Mark as kick operation done.
        Global.target_kick_servo = -1;
        pthread_mutex_unlock(&Global.servo_track_mutex);

        if(SERVO_SNT == target_servo){
            servo_SNT_kick();
        }

        else if(SERVO_GK == target_servo){
            servo_GK_kick();
        }

        pthread_mutex_lock(&Global.servo_kicker_state_mutex);
        int state = Global.servo_kicker_state;
        pthread_mutex_unlock(&Global.servo_kicker_state_mutex);

        if(STATE_STOP_REQUESTED == state)
            break;
    }

    pthread_mutex_lock(&Global.servo_kicker_state_mutex);
    Global.servo_kicker_state = STATE_STOPPED;
    pthread_mutex_unlock(&Global.servo_kicker_state_mutex);
    printf("Servo kicker thread stopped\n");
}

int start_servo_kicker_thread()
{
    pthread_create(&Global.servo_kicker_thread, NULL, &servo_kicker_thread_func, NULL);
    return 0;
}

int stop_servo_kicker_thread()
{
    pthread_mutex_lock(&Global.servo_kicker_state_mutex);
    if(STATE_PLAYING == Global.servo_kicker_state)
        Global.servo_kicker_state = STATE_STOP_REQUESTED;
    pthread_mutex_unlock(&Global.servo_kicker_state_mutex);

    pthread_mutex_lock(&Global.servo_track_mutex);
    Global.target_kick_servo = -2;
    pthread_cond_signal(&Global.servo_track_condvar);
    pthread_mutex_unlock(&Global.servo_track_mutex);

    return 0;
}

void trigger_servo_kick(int servo_num)
{
    pthread_mutex_lock(&Global.servo_track_mutex);
    Global.target_kick_servo = servo_num;
    pthread_cond_signal(&Global.servo_track_condvar);
    pthread_mutex_unlock(&Global.servo_track_mutex);
}

void sort_diff(int16_t* diffs)
{
    int i, j, min_idx;

    for (i = 0; i < 2 ; i++)
    {
        min_idx = i;
        for (j = i+1; j < 3 ; j++)
            if (diffs[j] < diffs[min_idx])
                min_idx = j;

        int16_t temp;
        temp = diffs[min_idx];
        diffs[min_idx] = diffs[i];
        diffs[i] = temp;
    }
}

void* ball_tracker_warning_thread_func(void* arg)
{
    pthread_mutex_lock(&Global.ball_warning_thread_state_mutex);
    if(STATE_STOPPED == Global.ball_warning_thread_state)
        Global.ball_warning_thread_state = STATE_PLAYING;
    pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);

    gpio_write(Global.pi, RED_LED_PIN, PI_LOW);
    printf("Ball tracker warning thread started\n");
    while(true)
    {
        pthread_mutex_lock(&Global.ball_warning_mutex);
        if(!Global.ball_not_found) {
            pthread_cond_wait(&Global.ball_warning_condvar, &Global.ball_warning_mutex);
        }
        pthread_mutex_unlock(&Global.ball_warning_mutex);

        gpio_write(Global.pi, RED_LED_PIN, PI_HIGH);
        usleep(75000);
        gpio_write(Global.pi, RED_LED_PIN, PI_LOW);
        usleep(75000);

        gpio_write(Global.pi, RED_LED_PIN, PI_HIGH);
        usleep(75000);
        gpio_write(Global.pi, RED_LED_PIN, PI_LOW);
        usleep(75000);

        usleep(1750000);

        pthread_mutex_lock(&Global.ball_warning_thread_state_mutex);
        int state = Global.ball_warning_thread_state;
        pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);

        if(STATE_STOP_REQUESTED == state)
            break;
    }

    pthread_mutex_lock(&Global.ball_warning_thread_state_mutex);
    Global.ball_warning_thread_state = STATE_STOPPED;
    gpio_write(Global.pi, RED_LED_PIN, PI_HIGH);
    printf("ball warning tracker thread stopped\n");
    pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);
}
int start_ball_tracker_warning_thread()
{
    pthread_mutex_lock(&Global.ball_warning_thread_state_mutex);
    int state = Global.ball_warning_thread_state;
    pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);

    if(STATE_STOPPED != state)
        return -1;

    pthread_create(&Global.ball_tracker_warning_thread, NULL, &ball_tracker_warning_thread_func, NULL);
    return 0;
}

int stop_ball_tracker_warning_thread()
{
    pthread_mutex_lock(&Global.ball_warning_thread_state_mutex);
    if(STATE_PLAYING == Global.ball_warning_thread_state)
        Global.ball_warning_thread_state = STATE_STOP_REQUESTED;
    pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);

    pthread_mutex_lock(&Global.ball_warning_mutex);
    Global.ball_not_found = 1;
    pthread_mutex_unlock(&Global.ball_warning_mutex);
    pthread_cond_signal(&Global.ball_warning_condvar);

    return 0;
}

void* game_thread_func(void* arg)
{
    pthread_mutex_lock(&Global.game_state_mutex);
    if(STATE_STOPPED == Global.game_play_state)
        Global.game_play_state = STATE_PLAYING;
    pthread_mutex_unlock(&Global.game_state_mutex);

    arm_home_sync();
    init_camera_params();
    printf("Game thread started\n");

    uint16_t ball_x;
    uint16_t ball_y;
    uint16_t arm_human_snt_pos;
    uint16_t arm_human_gk_pos;
    uint16_t stepper_pos;

    char start_time[24];
    get_current_date_time(start_time);
    set_current_match_start_date(start_time);
    update_current_match_data(0,0);
    init_multicast_connection();

    while(true)
    {
        // Get ball and stepper positions.
        get_ball_and_arm_positions(&ball_x, &ball_y, &arm_human_gk_pos, &arm_human_snt_pos);
        stepper_pos = Global.all_steppers[STEPPER_SNT].position;

        //printf("Ball pos : %d,%d \n",ball_x, ball_y);
        //printf("1 : %d\n", stepper_pos - DISTANCE_BETWEEN_KICKERS);
        //printf("2 : %d\n", stepper_pos);
        //printf("3 : %d\n", stepper_pos + DISTANCE_BETWEEN_KICKERS);

        pthread_mutex_lock(&Global.ball_info_mutex);
        Global.ball_position_x          = ball_x;
        Global.ball_position_y          = ball_y;
        Global.arm_robot_snt_position   = stepper_pos;
        Global.arm_robot_gk_position    = Global.arm_robot_snt_position;
        Global.arm_human_gk_position    = arm_human_gk_pos;
        Global.arm_human_snt_position   = arm_human_snt_pos;
        pthread_mutex_unlock(&Global.ball_info_mutex);
        align_servo_positions(ball_x);
        align_arms(ball_x, ball_y, stepper_pos);
        send_game_data();

        // get game state
        pthread_mutex_lock(&Global.game_state_mutex);
        int game_state = Global.game_play_state;
        pthread_mutex_unlock(&Global.game_state_mutex);

        if(STATE_STOP_REQUESTED == game_state)
            break;
    }

    pthread_mutex_lock(&Global.game_state_mutex);
    Global.game_play_state = STATE_STOPPED;
    printf("Game thread stopped\n");
    pthread_mutex_unlock(&Global.game_state_mutex);

    stop_multicast_stream();

    // add new match data in database file.
    pthread_mutex_lock(&Global.current_match_data_mutex);
    add_new_match_data(&Global.current_match_data);
    pthread_mutex_unlock(&Global.current_match_data_mutex);
}