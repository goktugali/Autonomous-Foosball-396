/**
 * Global degiskenler ve ilgili metodlari icerir.
 */

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <pigpiod_if2.h>
#include <netinet/in.h>
#include "defines.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct __Global_t{

    int                     pi;
    stepper_t               all_steppers[STEPPER_NUM];

    /* multicast game data */
    uint16_t                ball_position_x;
    uint16_t                ball_position_y;
    uint16_t                arm_robot_snt_position;
    uint16_t                arm_robot_gk_position;
    uint16_t                arm_human_snt_position;
    uint16_t                arm_human_gk_position;
    uint16_t                robot_score;
    uint16_t                human_score;

    int                     target_kick_servo;
    int                     ball_not_found;
    match_data_t            current_match_data;

    /* Threads */
    pthread_t               servo_kicker_thread;
    pthread_t               ball_tracker_warning_thread;
    pthread_t               game_thread;
    pthread_t               main_server_thread;

    /* sync stuff */
    pthread_mutex_t         servo_track_mutex;
    pthread_mutex_t         servo_snt_mutex;
    pthread_mutex_t         ball_warning_mutex;
    pthread_mutex_t         game_state_mutex;
    pthread_mutex_t         servo_kicker_state_mutex;
    pthread_mutex_t         ball_warning_thread_state_mutex;
    pthread_mutex_t         db_json_file_mutex;
    pthread_cond_t          servo_track_condvar;
    pthread_cond_t          ball_warning_condvar;

    /* state variables */
    int                     game_play_state;
    int                     servo_kicker_state;
    int                     ball_warning_thread_state;
    int                     main_server_thread_state;

    /* Comm stuff */
    int                     multicast_socket_fd;
    int                     main_server_socket_fd;
    struct sockaddr_in      multicast_addr;
    struct sockaddr_in      main_server_addr;

    /* game variables */
    int                     stepper_speed_default;
    double                  stepper_speed_multiplier_default;

}Global_t;

/**
 * Global degiskenleri initialize et.
 */
void global_init();

extern Global_t Global;

#endif