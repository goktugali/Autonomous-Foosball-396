#include "control.h"
#include "global.h"
#include "defines.h"
#include "motor_driver/motor_driver.h"
#include "ball_tracker/ball_tracker.h"
#include <cstdio>
#include <pthread.h>
#include <bits/stdc++.h>
#include "desktop_comm/desktop_comm.h"
#include <cmath>

int main()
{
    global_init();
    servo_GK_home();
    servo_SNT_home();
    arm_home_sync();
    init_camera_params();
    start_servo_kicker_thread();
    //arm_move_sync(STEPPER_MAX_POSITION);
    start_ball_tracker_warning_thread();
    //init_multicast_connection();
    //start_multicast_stream();

    // Todo : play game another thread.
    // Todo : implement server thread.


    while(true)
    {
        uint16_t ball_x;
        uint16_t ball_y;
        get_ball_position(&ball_x, &ball_y);
        printf("Ball pos : %d,%d \n",ball_x, ball_y);
        uint16_t stepper_pos = Global.all_steppers[STEPPER_SNT].position;
        printf("1 : %d\n", stepper_pos - DISTANCE_BETWEEN_KICKERS);
        printf("2 : %d\n", stepper_pos);
        printf("3 : %d\n", stepper_pos + DISTANCE_BETWEEN_KICKERS);
        /*
        pthread_mutex_lock(&Global.ball_info_mutex);
        Global.ball_position_x = ball_x;
        Global.ball_position_y = ball_y;
        Global.arm_snt_position = stepper_pos / 2;
        Global.arm_gk_position = Global.arm_snt_position;
        pthread_mutex_unlock(&Global.ball_info_mutex);
        */
        align_servo_positions(ball_x);
        align_arms(ball_x, ball_y, stepper_pos);

    }

    return 0;
}



