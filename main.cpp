#include "system_control/system_control.hpp"

int main()
{
    global_init();
    //arm_home_sync();
    start_main_server_thread();

    while(true)
    {
        //arm_move_sync(STEPPER_MAX_POSITION);
        //arm_move_sync(STEPPER_MIN_POSITION + 20);
        sleep(5);
    }

    return 0;
}



