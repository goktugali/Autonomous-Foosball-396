#include "global.h"
#include "defines.h"
#include "motor_driver/motor_driver.h"
#include <cstdio>
#include <pthread.h>

void fonksiyonel_test_1();
void* stepper_thread_func(void* arg);

int main()
{
    fonksiyonel_test_1();
    return 0;
}

void fonksiyonel_test_1()
{
    global_init();
    servo_GK_home();
    servo_SNT_home();
    arm_home_sync();

    pthread_t stepper_thread;
    //pthread_create(&stepper_thread,NULL,&stepper_thread_func,NULL);

    /* servolari hareket ettir */
    while(1)
    {
        servo_GK_kick();
        servo_SNT_kick();
        //sleep(2);
    }
}

void* stepper_thread_func(void* arg)
{
    while(1)
    {
        arm_move_sync(260);
        arm_move_sync(0);
    }
}

