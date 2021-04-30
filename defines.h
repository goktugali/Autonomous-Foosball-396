/**
 * Sabit tanimlamalari icerir.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

/************ GPIO PIN DEFINES ***********/
#define ENDSTOP_ARM_SNT_PIN             16
#define STEPPER_ARM_SNT_STEP_PIN        20
#define STEPPER_ARM_SNT_DIR_PIN         21
#define STEPPER_ARM_GK_STEP_PIN         23 // Devre disi
#define STEPPER_ARM_GK_DIR_PIN          24 // Devre disi
#define SERVO_SNT_SIGNAL_PIN            27
#define SERVO_GK_SIGNAL_PIN             22

/************ GPIO PIN DEFINES ***********/


/********* BALL TRACKER DEFINES **********/
// Todo : Fix here.
#define imageLowH                       90
#define imageHighH                      130

#define imageLowS                       130
#define imageHighS                      255

#define imageLowV                       95
#define imageHighV                      255
/********* BALL TRACKER DEFINES **********/


/********** SIZE DEFINES ****************/
#define FIELD_Y_LENGTH                      664
#define DISTANCE_BETWEEN_KICKERS            166
#define SNT_KICK_DISTANCE_MAX               400
#define SNT_KICK_DISTANCE_MIN               370
#define GK_KICK_DISTANCE_MAX                75
#define GK_KICK_DISTANCE_MIN                48
/********** SIZE DEFINES ****************/

/************ TYPE DEFINES **************/

/* Step Motor Struct */
typedef struct __stepper_t{
    int         step_pin;
    int         dir_pin;
    uint16_t    position;
}stepper_t;

/* Arayuze gonderilecek data paketi */
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
}STEPPER_ID;

typedef enum {
    SERVO_GK = 0,
    SERVO_SNT,
    SERVO_NUM
}SERVO_ID;
/************ ENUMARATIONS **************/


#endif