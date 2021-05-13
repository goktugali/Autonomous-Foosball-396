#include "system_control/system_control.hpp"

int main()
{
    global_init();
    start_main_server_thread();

    while(true)
    {
        sleep(5);

    }

    return 0;
}



