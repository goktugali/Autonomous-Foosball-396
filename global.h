/**
 * Global degiskenler ve ilgili metodlari icerir.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <pigpiod_if2.h>
#include <netinet/in.h>
#include "defines.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct __Global_t{

    int                     pi;
    stepper_t               all_steppers[STEPPER_NUM];

    /* game data */
    uint16_t                ball_position_x;
    uint16_t                ball_position_y;
    uint16_t                arm_snt_position;
    uint16_t                arm_gk_position;
    uint16_t                robot_score;
    uint16_t                human_score;

    int                     target_kick_servo;
    int                     ball_not_found;

    /* Threads */
    pthread_t               servo_kicker_thread;
    pthread_t               ball_tracker_warning_thread;
    pthread_t               multicast_stream_thread;

    /* sync stuff */
    pthread_mutex_t         servo_track_mutex;
    pthread_mutex_t         servo_snt_mutex;
    pthread_mutex_t         ball_info_mutex;
    pthread_mutex_t         ball_warning_mutex;
    pthread_mutex_t         multicast_stream_state_mutex;
    pthread_cond_t          servo_track_condvar;
    pthread_cond_t          ball_warning_condvar;

    /* Comm stuff */
    int                     multicast_socket_fd;
    struct sockaddr_in      multicast_addr;
    int                     multicast_stream_state;

}Global_t;

/**
 * Global degiskenleri initialize et.
 */
void global_init();

extern Global_t Global;

#endif