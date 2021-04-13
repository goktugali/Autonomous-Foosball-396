#include "global.h"
#include "motor_driver/stepper_driver.h"
#include <cstdio>

int main()
{
    global_init();

    stepper_t stepper;
    stepper = init_stepper(20,21);
    stepper_t stepper2;
    stepper2 = init_stepper(23,24);
    uint16_t pos;


    int c;

    while(1)
    {
        stepper_home(&stepper);
        stepper_set_position(&stepper, 160);

        scanf("%d",&c);
    }



    /*
    while(1)
    {
        stepper_set_position(&stepper, 0);
        //sleep(3);
        stepper_set_position(&stepper, 320);
        //sleep(3);
        printf("Position : %d\n", stepper.position);
        //stepper_set_position(&stepper2, 60);
        //stepper_go(&stepper2,STEP_DIR_CW, 10000, DEGREE_TO_STEPS(60));
        //stepper_go(&stepper2,STEP_DIR_CCW, 10000, DEGREE_TO_STEPS(60));
    }
     */

    return 0;
}
