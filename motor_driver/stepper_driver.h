/**
 * Stepper motor driverlarini icerir.
 */

#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#include "../global.h"
#include "../defines.h"
#include <pigpiod_if2.h>

#define DEGREE_TO_STEPS(x) (6400*x / 360)
#define STEPPER_SPEED_DEFAULT           10000
#define STEPPER_POS_MULTIPLIER          20
#define STEPPER_SPEED_MULTIPLIER        1

stepper_t init_stepper(int step_pin, int dir_pin);
void generate_ramp(int step_pin, int ramp0, int ramp1);
void stepper_go(const stepper_t* step_motor, STEP_DIRECTON dir, int speed, int steps);
void stepper_set_position(stepper_t* step_motor, uint16_t target_pos);
void stepper_home(stepper_t* step_motor);

void arm_move_sync(uint16_t target_pos);
void arm_move(stepper_t* step_motor,  uint16_t target_pos);

void arm_home(stepper_t* step_motor);
void arm_home_sync();

#endif