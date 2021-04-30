#include "control.h"

void align_arms(uint16_t ball_y, uint16_t stepper_pos)
{
    int16_t differences[3];
    uint16_t first_kicker_pos   = stepper_pos;
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

    for (int i = 0; i < 3; ++i)
    {
        if(differences[i] == difference_first_kicker)
        {
            if(first_kicker_pos < ball_y)
            {
             target_pos = stepper_pos + differences[i];
            }
            else
            {
                target_pos = stepper_pos - differences[i];
            }
        }
        else if(differences[i] == difference_second_kicker)
        {

            if(second_kicker_pos < ball_y)
            {
                target_pos = stepper_pos + differences[i];
            }
            else
            {
                target_pos = stepper_pos - differences[i];
            }
        }
        else if(differences[i] == difference_third_kicker)
        {
            if(third_kicker_pos < ball_y)
            {
                target_pos = stepper_pos + differences[i];
            }
            else
            {
                target_pos = stepper_pos - differences[i];
            }
        }

        if(target_pos >= 0 && target_pos <= FIELD_Y_LENGTH) {
            // Todo : do it by arm_move_sync()
            stepper_set_position(&Global.all_steppers[STEPPER_SNT], target_pos);
            break;
        }
    }
}

int is_kick_need(uint16_t ball_x)
{

    if(ball_x < SNT_KICK_DISTANCE_MAX && ball_x > SNT_KICK_DISTANCE_MIN){
        return SERVO_SNT;
    }
    if(ball_x < GK_KICK_DISTANCE_MAX && ball_x > GK_KICK_DISTANCE_MIN){
        return SERVO_GK;
    }

    return -1;
}
void* servo_kicker_thread(void* arg)
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
    pthread_create(&Global.servo_kicker_thread, NULL, &servo_kicker_thread, NULL);
}

void trigger_servo(int servo_num)
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