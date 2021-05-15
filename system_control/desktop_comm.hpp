/*
 * Arauuz ile iletisim islemlerini icerir.
 */

#ifndef DESKTOP_COM_HPP
#define DESKTOP_COM_HPP

#include "../global.hpp"
#include "../defines.hpp"


void init_multicast_connection();
void send_game_data();
int stop_multicast_stream();

#endif