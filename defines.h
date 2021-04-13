/**
 * Sabit tanimlamalari icerir.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <pigpiod_if2.h>

/************ CONSTANT DEFINES ***********/
#define ENDSTOP_ARM_SNT_PIN     16




/************ CONSTANT DEFINES ***********/


/************ TYPE DEFINES **************/
typedef struct __stepper_t{
    int step_pin;
    int dir_pin;
    uint16_t position;
}stepper_t;

typedef enum{
    STEP_DIR_CW = 0,
    STEP_DIR_CCW
}STEP_DIRECTON;

typedef enum {
    ROBOT_GK = 0,
    ROBOT_SNT,
    ROBOT_ARM_NUM
}ROBOT_ARMS;
/************ TYPE DEFINES **************/

#endif