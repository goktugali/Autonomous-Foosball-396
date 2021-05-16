/*
 * Arauuz ile iletisim islemlerini icerir.
 */

#ifndef DESKTOP_COM_HPP
#define DESKTOP_COM_HPP

#include "../global.hpp"
#include "../defines.hpp"

/**
 * Initializes the multicast connection.
 */
void init_multicast_connection();

/**
 * Sends real time game data over multicast stream.
 */
void send_game_data();

/**
 * Stops, closes the multicast connection.
 * @return 0 on success, -1 on error.
 */
int stop_multicast_stream();

#endif