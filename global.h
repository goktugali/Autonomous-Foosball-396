/**
 * Global degiskenler ve ilgili metodlari icerir.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <pigpiod_if2.h>
#include "defines.h"

typedef struct __Global_t{

    int                     pi;
    stepper_t               all_steppers[STEPPER_NUM];

    /* game variables */
    int16_t                 ball_position_x;
    int16_t                 ball_position_y;
    int                     target_kick_servo;
    int                     servo_prev_state;

    /* Threads */
    pthread_t               ball_tracker_thread;
    pthread_t               servo_kicker_thread;

    /* sync stuff */
    pthread_mutex_t         servo_track_mutex;
    pthread_mutex_t         servo_snt_mutex;
    pthread_mutex_t         ball_info_mutex;
    pthread_cond_t          servo_track_condvar;
    pthread_cond_t          snt_servo_armup_condvar;

}Global_t;

/**
 * Global degiskenleri initialize et.
 */
void global_init();

extern Global_t Global;

#endif