#include "global.h"
#include "motor_driver/stepper_driver.h"
#include <cstdio>

int main()
{
    global_init();
    arm_home_sync();

    while(1)
    {

        arm_move_sync(160);
        arm_move_sync(0);
    }


    return 0;
}
