#include "system_control/system_control.hpp"

int main()
{
    global_init();
    start_main_server_thread();

    pthread_join(Global.main_server_thread, NULL);

    return 0;
}



