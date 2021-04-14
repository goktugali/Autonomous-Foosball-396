/**
 * Sabit tanimlamalari icerir.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

/************ GPIO PIN DEFINES ***********/
#define ENDSTOP_ARM_SNT_PIN             16
#define ENDSTOP_ARM_GK_PIN              99999
#define STEPPER_ARM_SNT_STEP_PIN        20
#define STEPPER_ARM_SNT_DIR_PIN         21
#define STEPPER_ARM_GK_STEP_PIN         23
#define STEPPER_ARM_GK_DIR_PIN          24


/************ GPIO PIN DEFINES ***********/


/************ TYPE DEFINES **************/
typedef struct __stepper_t{
    int         step_pin;
    int         dir_pin;
    uint16_t    position;
}stepper_t;

typedef struct __hardware_data_pckt_t{
    uint16_t    stepper_gk_position;
    uint16_t    stepper_snt_position;
    uint16_t    ball_position;
    uint8_t     robot_score;
    uint8_t     human_score;
}hardware_data_pckt_t;

/************ TYPE DEFINES **************/

/************ ENUMARATIONS **************/
typedef enum{
    STEP_DIR_CW = 0,
    STEP_DIR_CCW
}STEP_DIRECTON;

typedef enum {
    STEPPER_GK = 0,
    STEPPER_SNT,
    STEPPER_NUM
}ROBOT_ARMS;
/************ ENUMARATIONS **************/


#endif