/**
 * Stepper & servo motor driverlarini icerir.
 */

#ifndef MOTOR_DRIVER_HPP
#define MOTOR_DRIVER_HPP

#include "../global.hpp"
#include "../defines.hpp"
#include <pigpiod_if2.h>
#include <pthread.h>

/**************** TANIMLAMALAR *****************/
#define DEGREE_TO_STEPS(x) (6400*x / 360)
#define STEPPER_SPEED_DEFAULT               10000
#define STEPPER_POS_MULTIPLIER              50
#define STEPPER_SPEED_MULTIPLIER            3
#define SERVO_SNT_HOME_POSITION             1400
#define SERVO_GK_HOME_POSITION              1400
#define SERVO_KICK_DISTANCE                 700
/**************** TANIMLAMALAR *****************/

/********* Alt seviye motor kontrol fonksiyonlari *********/
stepper_t init_stepper(int step_pin, int dir_pin);
void generate_ramp(int step_pin, int ramp0, int ramp1);
void stepper_go(const stepper_t* step_motor, STEP_DIRECTON dir, int speed, int steps);
void stepper_set_position(stepper_t* step_motor, uint16_t target_pos);
void stepper_home(stepper_t* step_motor);
/********* Alt seviye motor kontrol fonksiyonlari *********/


/**************** Kullanici seviyesi motor kontrol fonksiyonlari *****************/
/**
 * Kaleci ve forvet kollarini senkronize olarak hareket ettirir.
 * @param target_pos
 */
void arm_move_sync(uint16_t target_pos);

/**
 * Kaleci ve forvet kollarini home pozisyonuna goturur.
 * Islem tamamlanandiktan sonra return olur. (Blocked call)
 * Todo : Non-Block versiyonu yazilacak.
 */
void arm_home_sync();

/**
 * Belirtilen kolu hareket ettirir.
 * Islem tamamlanandiktan sonra return olur. (Blocked call)
 * Todo : Non-Block versiyonu yazilacak.
 * @param step_motor Hareket ettirilecek kol (step motor).
 * @param target_pos Hareket mesafesi.
 */
void arm_move(stepper_t* step_motor,  uint16_t target_pos);

/**
 * Belirtilen kolu home pozisyonuna goturur.
 * Islem tamamlanandiktan sonra return olur. (Blocked call)
 * Todo : Non-Block versiyonu yazilacak.
 * @param step_motor Hareket ettirilecek kol (step motor).
 */
void arm_home(stepper_t* step_motor);

/**
 * Forvet servosunu home pozisyonuna goturur.
 * Todo : Non-Block versiyonu yazilacak.
 * Islem tamamlanandiktan sonra return olur. (Blocked call)
 */
void servo_SNT_home();

/**
 * Forvet servosunu vurus icin tetikler.
 * Islem tamamlanandiktan sonra return olur. (Blocked call)
 */
void servo_SNT_kick();

/**
 * Kaleci servosunu home pozisyonuna goturur.
 * Islem tamamlandiktan sonra return olur. (Blocked call)
 */
void servo_GK_home();

/**
 * Kaleci servosunu vurus icin tetikler.
 * Islem tamamlanandiktan sonra return olur. (Blocked call)
 */
void servo_GK_kick();
/**************** Kullanici seviyesi motor kontrol fonksiyonlari *****************/

#endif