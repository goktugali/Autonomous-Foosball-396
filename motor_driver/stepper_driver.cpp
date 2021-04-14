#include "stepper_driver.h"

stepper_t init_stepper(int step_pin, int dir_pin)
{
    stepper_t s;
    s.step_pin  = step_pin;
    s.dir_pin   = dir_pin;
    s.position  = 0;
    set_mode(Global.pi, step_pin, PI_OUTPUT);
    return s;
}

void generate_ramp(int step_pin, int ramp0, int ramp1)
{
    int wid = -1;
    int f = ramp0;
    int micros = (int)(500000/f);
    gpioPulse_t pulses[2] =  {{1 << step_pin, 0, micros},{0, 1 << step_pin,micros}};
    wave_add_generic(Global.pi, 2, pulses);

    wid = wave_create(Global.pi);

    int steps = ramp1;
    int x = steps & 255;
    int y = steps >> 8;
    char wave[7] = {255, 0, wid, 255, 1 , x, y};
    wave_chain(Global.pi, wave , 7);
    while(wave_tx_busy(Global.pi));

    wave_delete(Global.pi, wid);
}


void stepper_go(const stepper_t* step_motor, STEP_DIRECTON dir, int speed, int steps)
{
    gpio_write(Global.pi, step_motor->dir_pin, dir);
    generate_ramp(step_motor->step_pin, speed, steps);
}

void stepper_set_position(stepper_t* step_motor, uint16_t target_pos)
{
    if(target_pos == step_motor->position)
        return;

    if(target_pos > step_motor->position)
    {
        int travel_distance = (target_pos- step_motor->position) * STEPPER_POS_MULTIPLIER;
        int needed_speed    = travel_distance * STEPPER_SPEED_MULTIPLIER;
        stepper_go(step_motor, STEP_DIR_CCW, needed_speed, travel_distance);
    }
    else
    {
        int travel_distance = (step_motor->position - target_pos) * STEPPER_POS_MULTIPLIER;
        int needed_speed    = travel_distance * STEPPER_SPEED_MULTIPLIER;
        stepper_go(step_motor, STEP_DIR_CW, needed_speed, travel_distance);
    }

    step_motor->position = target_pos;
}

void stepper_home(stepper_t* step_motor)
{
    gpio_write(Global.pi, step_motor->dir_pin, STEP_DIR_CW);
    set_PWM_dutycycle(Global.pi, step_motor->step_pin, 128);
    set_PWM_frequency(Global.pi, step_motor->step_pin, 2000);
    while(gpio_read(Global.pi, ENDSTOP_ARM_SNT_PIN) != 0);

    set_PWM_dutycycle(Global.pi, step_motor->step_pin, 0);
    sleep(1);

    gpio_write(Global.pi, step_motor->dir_pin, STEP_DIR_CCW);
    set_PWM_dutycycle(Global.pi, step_motor->step_pin, 128);
    while(gpio_read(Global.pi, ENDSTOP_ARM_SNT_PIN) != 1);

    set_PWM_dutycycle(Global.pi, step_motor->step_pin, 0);
    stepper_go(step_motor, STEP_DIR_CCW, 2000, 20);

    step_motor->position = 0;
}

void arm_move(stepper_t* step_motor,  uint16_t target_pos)
{
    stepper_set_position(step_motor, target_pos);
}

void arm_move_sync(uint16_t target_pos)
{
    stepper_set_position(&Global.all_steppers[STEPPER_SNT], target_pos);
    Global.all_steppers[STEPPER_GK].position = Global.all_steppers[STEPPER_SNT].position;
}

void arm_home(stepper_t* step_motor)
{
    stepper_home(step_motor);
}
void arm_home_sync()
{
    stepper_home(&Global.all_steppers[STEPPER_SNT]);
    Global.all_steppers[STEPPER_GK].position = 0;
}
