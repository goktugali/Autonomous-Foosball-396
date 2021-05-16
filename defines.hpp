/**
 * Sabit tanimlamalari icerir.
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

/************ GPIO PIN DEFINES ***********/
#define ENDSTOP_ARM_SNT_PIN                 16
#define STEPPER_ARM_SNT_STEP_PIN            20
#define STEPPER_ARM_SNT_DIR_PIN             21
#define STEPPER_ARM_GK_STEP_PIN             23 // Devre disi
#define STEPPER_ARM_GK_DIR_PIN              24 // Devre disi
#define SERVO_SNT_SIGNAL_PIN                27
#define SERVO_GK_SIGNAL_PIN                 22
#define RED_LED_PIN                         4
/************ GPIO PIN DEFINES ***********/


/********* BALL TRACKER DEFINES **********/
// Todo : Update here for ball and red arm.
#define imageLowH                           90
#define imageHighH                          130

#define imageLowS                           130
#define imageHighS                          255

#define imageLowV                           95
#define imageHighV                          255
/********* BALL TRACKER DEFINES **********/


/********** SIZE DEFINES ****************/
#define STEPPER_MAX_POSITION                 320
#define STEPPER_MIN_POSITION                 168
#define DISTANCE_BETWEEN_KICKERS             113
#define SNT_KICK_DISTANCE_MAX                400 // Todo : Look here.
#define SNT_KICK_DISTANCE_MIN                375 // Todo : Look here.
#define GK_KICK_DISTANCE_MAX                 75
#define GK_KICK_DISTANCE_MIN                 48
/********** SIZE DEFINES ****************/


/************* COMM DEFINES *****************/
#define MULTICAST_GROUP_IP                  "234.5.5.5"
#define MULTICAST_PORT                      10200
#define MAIN_SERVER_PORT                    7080
#define COMM_DATA_PACKET_SIZE               8  //  = 16byte
#define JSON_DATABASE_FILE_PATH                  "/home/pi/Desktop/database/database.json"
/************* COMM DEFINES *****************/

/************ TYPE DEFINES **************/

/* Step Motor Struct */
typedef struct __stepper_t{
    int         step_pin;
    int         dir_pin;
    uint16_t    position;
}stepper_t;

typedef struct __match_data{
    char date[24];
    char human_score[5];
    char robot_score[5];
}match_data_t;

/************ TYPE DEFINES **************/

/************ ENUMARATIONS **************/
typedef enum{
    STEP_DIR_CCW = 0,
    STEP_DIR_CW
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

typedef enum {
    SERVO_KICK = 0,
    SERVO_UP,
    KICK_TYPE_NUM
}KICK_TYPE;

typedef enum{
    STATE_PLAYING = 0,
    STATE_STOPPED,
    STATE_STOP_REQUESTED,
    STATE_NUM
}PLAY_STATE;

/************ ENUMARATIONS **************/


#endif