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

/**
 * Calculate target position and align robot arms to target position.
 * @param ball_x x position of the ball.
 * @param ball_y y position of the ball.
 * @param stepper_pos Current stepper (mid player) position.
 */
void align_arms(uint16_t ball_x,uint16_t ball_y, uint16_t stepper_pos);

/**
 * Checks that if kick need or not.
 * @param ball_x x position of the ball.
 * @return 1 on kick need state, 0 on not need state.
 */
int is_kick_need(uint16_t ball_x);

/**
 * Servo kicker thread method. This thread is responsible for kick movement of the servos.
 * Game thread will trigger this thread if any kick state occurs.
 */
void* servo_kicker_thread_func(void* arg);

/**
 * Starts the servo kicker thread.
 * @return 0 on success, -1 on error.
 */
int start_servo_kicker_thread();

/**
 * Stops the servo kicker thread.
 * @return 0 on success, -1 on error.
 */
int stop_servo_kicker_thread();

/**
 * Trigger servo kicker thread.
 * @param servo_num ID of the servo motor.
 */
void trigger_servo_kick(int servo_num);

/**
 * Align servo positions.
 * @param ball_x x position of the ball.
 */
void align_servo_positions(uint16_t ball_x);

/**
 * Ball tracker (red led warning) thread method.
 * This thread blinks the red led if ball is not found while game playing.
 */
void* ball_tracker_warning_thread_func(void* arg);

/**
 * Start ball tracker warning thread.
 */
int start_ball_tracker_warning_thread();

/**
 * Stop ball tracker warning thread.
 */
int stop_ball_tracker_warning_thread();

/**
 * Main game thread method.
 */
void* game_thread_func(void* arg);

#endif