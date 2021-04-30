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

    // Todo : play game another thread.
    // Todo : implement server thread.
    while(true)
    {
        uint16_t ball_x;
        uint16_t ball_y;
        get_ball_position(&ball_x, &ball_y);
        cout << " Ball Position : " << "(" << ball_x << "," << ball_y << ")" << endl;
        uint16_t stepper_pos = Global.all_steppers[STEPPER_SNT].position;
        align_arms(ball_y, stepper_pos);

        int servo = is_kick_need(ball_x);
        if(-1 != servo)
            trigger_servo(servo);
    }

    return 0;
}



