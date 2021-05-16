#ifndef SYSTEM_CONTROL_HPP
#define SYSTEM_CONTROL_HPP

#include "../global.hpp"
#include "../defines.hpp"
#include "../game_control/game_control.hpp"
#include "desktop_comm.hpp"
#include "serv_commands.hpp"

#define RESPONSE_TIME_SEC 3

/**
 * Stars the main game thread.
 * @return 0 on success, -1 on error.
 */
int start_game_thread();

/**
 * Stops the main game thread.
 * @return 0 on success, -1 on error.
 */
int stop_game_thread();

/**
 * Send old scores data to the client (Desktop application).
 * This method reads the content of the json database file and send it to the client.
 * @param client_fd client socket (connection) file descriptor.
 */
void send_old_scores_data(int client_fd);

/**
 * Sets the game difficulty level.
 * @param set_level_command set level command received from client.
 */
void set_game_diffuculty_level(const char* set_level_command);

/**
 * Sends OK response to the client.
 */
void send_OK_response();

/**
 * Initializes the main server connection (sockets.)
 */
void init_main_server_connection();

/**
 * Starts the main server as a different thread.
 */
void start_main_server_thread();

/**
 * Main server thread method.
 */
void* main_server_thread_func(void* arg);
#endif