#include "control.h"
#include "global.h"
#include "defines.h"
#include "motor_driver/motor_driver.h"
#include "ball_tracker/ball_tracker.h"
#include <cstdio>
#include <pthread.h>
#include <bits/stdc++.h>
#include <cmath>


int main()
{
    global_init();
    servo_GK_home();
    servo_SNT_home();
    arm_home_sync();
    init_camera_params();
    start_servo_kicker_thread();
    arm_move_sync(166);

    // Todo : play game another thread.
    // Todo : implement server thread.
    sleep(2);
    while(true)
    {
        uint16_t ball_x;
        uint16_t ball_y;
        get_ball_position(&ball_x, &ball_y);
        cout << " Ball Position : " << "(" << ball_x << "," << ball_y << ")" << endl;
        uint16_t stepper_pos = Global.all_steppers[STEPPER_SNT].position;

        pthread_mutex_lock(&Global.ball_info_mutex);
        Global.ball_position_x = ball_x;
        Global.ball_position_y = ball_y;
        pthread_mutex_unlock(&Global.ball_info_mutex);

        align_arms(ball_x, ball_y, stepper_pos);
        align_servo_positions(ball_x);
    }

    return 0;
}



