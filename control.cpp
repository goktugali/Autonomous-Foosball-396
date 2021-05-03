#include "control.h"

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
    uint16_t first_kicker_pos   = stepper_pos / 2;
    uint16_t second_kicker_pos  = first_kicker_pos + DISTANCE_BETWEEN_KICKERS;
    uint16_t third_kicker_pos   = second_kicker_pos + DISTANCE_BETWEEN_KICKERS;

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
    printf("Motor pos : %d\n", stepper_pos);
    for (int i = 0; i < 3; ++i)
    {
        if(differences[i] == difference_first_kicker)
        {
            printf("En yakin birinci adam \n");
            printf("Birinci adam mesafe : %d\n", differences[i]);
            printf("Birinci adam konum : %d\n", first_kicker_pos);
            if(first_kicker_pos < ball_y)
            {
                target_pos = first_kicker_pos + differences[i];
            }
            else
            {
                target_pos = first_kicker_pos - differences[i];
            }

            target_pos = target_pos*2;
        }
        else if(differences[i] == difference_second_kicker)
        {

            printf("En yakin ikinci adam \n");
            printf("ikinci adam mesafe : %d\n", differences[i]);
            printf("ikinci adam konum : %d\n", second_kicker_pos);
            if(second_kicker_pos < ball_y)
            {
                target_pos = second_kicker_pos + differences[i];
            }
            else
            {
                target_pos = second_kicker_pos - differences[i];
            }
            target_pos = (target_pos - DISTANCE_BETWEEN_KICKERS)*2;
        }
        else if(differences[i] == difference_third_kicker)
        {
            printf("En yakin ucuncu adam \n");
            printf("ucuncu adam mesafe : %d\n", differences[i]);
            printf("ucuncu adam konum : %d\n", third_kicker_pos);
            if(third_kicker_pos < ball_y)
            {
                target_pos = third_kicker_pos + differences[i];
            }
            else
            {
                target_pos = third_kicker_pos - differences[i];
            }

            target_pos = (target_pos - (2* DISTANCE_BETWEEN_KICKERS))*2;
        }

        if(target_pos >= 0 && target_pos <= FIELD_Y_LENGTH) {
            printf("Hareket komutu gonderildi %d \n", target_pos);
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
    }
}

void start_servo_kicker_thread()
{
    pthread_create(&Global.servo_kicker_thread, NULL, &servo_kicker_thread_func, NULL);
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