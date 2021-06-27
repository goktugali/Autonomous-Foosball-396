#include "system_control/system_control.hpp"
#include <signal.h>

void handler_sigint(int signo)
{
    stop_game_thread();
    close(Global.main_server_socket_fd);
    exit(0);
}

int main()
{
    sleep(10);
    global_init();

    /* setup the SIGINT handler */
    struct sigaction sigint_action;
    memset(&sigint_action,0,sizeof(sigint_action));
    sigint_action.sa_handler = &handler_sigint;
    sigaction(SIGINT,&sigint_action,NULL);

    start_main_server_thread();

    pthread_join(Global.main_server_thread, NULL);

    return 0;
}



