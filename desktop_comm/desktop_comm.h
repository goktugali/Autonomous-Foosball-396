/*
 * Arauuz ile iletisim islemlerini icerir.
 */

#ifndef DESKTOP_COM_H
#define DESKTOP_COM_H

#include "../global.h"
#include "../defines.h"

void init_multicast_connection();
void send_game_data();

/**
 * Start multicast game data stream.
 * @return 0 on success, -1 on error.
 */
int start_multicast_stream();
void* multicast_stream_thread_func(void* arg);

#endif