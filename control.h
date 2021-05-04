#ifndef CONTROL_H
#define CONTROL_H

#include "global.h"
#include "defines.h"
#include "motor_driver/motor_driver.h"
#include "ball_tracker/ball_tracker.h"
#include <pthread.h>

void sort_diff(int16_t* diffs);
void align_arms(uint16_t ball_x,uint16_t ball_y, uint16_t stepper_pos);
int is_kick_need(uint16_t ball_x);
int is_armup_need(uint16_t ball_x);
void* servo_kicker_thread_func(void* arg);
void* snt_servo_align_thread(void* arg);
void start_servo_kicker_thread();
void trigger_servo_kick(int servo_num);
void align_servo_positions(uint16_t ball_x);
void* stepper_aligner_thread_func(void* arg);
void start_stepper_aligner();
void* ball_tracker_warning_thread_func(void* arg);
void start_ball_tracker_warning_thread();

#endif