#ifndef CONTROL_HPP
#define CONTROL_HPP

#include "../defines.hpp"
#include "../global.hpp"
#include "../motor_driver/motor_driver.hpp"
#include "../ball_tracker/ball_tracker.hpp"
#include "../system_control/match_data_utils.hpp"
#include "../system_control/desktop_comm.hpp"
#include <pthread.h>

void sort_diff(int16_t* diffs);
void align_arms(uint16_t ball_x,uint16_t ball_y, uint16_t stepper_pos);
int is_kick_need(uint16_t ball_x);
void* servo_kicker_thread_func(void* arg);
int start_servo_kicker_thread();
int stop_servo_kicker_thread();
void trigger_servo_kick(int servo_num);
void align_servo_positions(uint16_t ball_x);
void* ball_tracker_warning_thread_func(void* arg);
int start_ball_tracker_warning_thread();
int stop_ball_tracker_warning_thread();
void* game_thread_func(void* arg);

#endif