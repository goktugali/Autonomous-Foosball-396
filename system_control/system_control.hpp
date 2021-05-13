#ifndef SYSTEM_CONTROL_HPP
#define SYSTEM_CONTROL_HPP

#include "../global.hpp"
#include "../defines.hpp"
#include "../game_control/game_control.hpp"
#include "desktop_comm.hpp"
#include "serv_commands.hpp"

int start_game_thread();
int stop_game_thread();

void init_main_server_connection();
void start_main_server_thread();
void* main_server_thread_func(void* arg);
#endif