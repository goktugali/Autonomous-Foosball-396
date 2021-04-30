#ifndef CONTROL_H
#define CONTROL_H

#include "global.h"
#include "defines.h"
#include "motor_driver/motor_driver.h"
#include "ball_tracker/ball_tracker.h"
#include <pthread.h>

void sort_diff(int16_t* diffs);
void align_arms(uint16_t ball_y, uint16_t stepper_pos);
int is_kick_need(uint16_t ball_x);
void* servo_kicker_thread_func(void* arg);
void start_servo_kicker_thread();
void trigger_servo(int servo_num);

#endif