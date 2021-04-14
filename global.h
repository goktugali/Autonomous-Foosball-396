/**
 * Global degiskenler ve ilgili metodlari icerir.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <pigpiod_if2.h>
#include "defines.h"

typedef struct __Global_t{

    int             pi;
    stepper_t       all_steppers[STEPPER_NUM];

}Global_t;

/**
 * Global degiskenleri initialize et.
 */
void global_init();

extern Global_t Global;

#endif