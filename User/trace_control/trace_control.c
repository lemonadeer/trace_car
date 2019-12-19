#include "trace_control.h"
#include "simple.h"
#include "motor.h"
#include "bsp_delay.h"
#include "bsp_pwm.h"
#include "bsp_led.h"

extern __IO uint8_t need_process;
extern __IO uint8_t left_spin_lock;
extern __IO uint8_t right_spin_lock;
extern __IO uint8_t nr_case;

extern uint8_t MOTOR_NORMAL_SPEED_LEVEL;
extern uint8_t MOTOR_SPIN_SPEED_LEVEL;
extern uint8_t MOTOR_SLOW_SPEED_LEVEL;

extern __IO uint8_t flag_spcecial;	//0 mean ignore,1 mean turn left when enter case8, 2 mean turn right */

void trace_normal(void)
{
    /* PWM increase */
    if(SPEED_LEFT_MOTOR < SPEED_RIGHT_MOTOR) {
        SPEED_LEFT_MOTOR = SPEED_RIGHT_MOTOR;
    }
    else {
        SPEED_RIGHT_MOTOR = SPEED_LEFT_MOTOR;
    }
    if( (SPEED_LEFT_MOTOR < MOTOR_NORMAL_SPEED_LEVEL) || \
            (SPEED_RIGHT_MOTOR < MOTOR_NORMAL_SPEED_LEVEL)) {
        SPEED_LEFT_MOTOR += 1;
        SPEED_RIGHT_MOTOR += 1;
    }
}

void trace_adjust_left(void)
{
    SPEED_LEFT_MOTOR = 65;
}

void trace_adjust_right(void)
{
    SPEED_LEFT_MOTOR = 65;
}

void trace_spin_left(void)
{
    uint8_t case_tmp = nr_case;

    motor_brake();
    motor_slow();
    /* case */
    if (SENSOR1_BIT && SENSOR2_BIT) {
        while(SENSOR1_BIT && SENSOR2_BIT) {
            if( ! SENSOR0_BIT) {
                if((flag_spcecial == 2) && (case_tmp == case8_turn_away)) {
                    motor_stop();
                    while(1);
                    motor_spin_left();
                    while(SENSOR2_BIT);
                    motor_slow();
                }
                goto normal_spin;
                return;
            }
        }
        if( ! SENSOR3_BIT) {
            goto normal_spin;
        }
        /* case 8 */
        LED3_TOGGLE;	//case 8
        if(nr_case == case8_turn_enter) {
            if(flag_spcecial == 1) {	/* turn left */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_left();
                return;
            }
            else {		/* turn right */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_right();
                return;
            }
        }
        if(nr_case == case8_turn_away) {
            if(flag_spcecial == 1) {	/* turn right */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_right();
                return;
            }
            else {		/* turn left */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_left();
                return;
            }
        }

    }

normal_spin:
    while(SENSOR1_BIT && SENSOR2_BIT);

    /* now sensor1 or 2 are pass the line or both , but need to check the wrong signal */
    /* when need spin left the sensor2 must be 0 , and sensor1 must be 0 when spin right */
    if(SENSOR2_BIT) {
        trace_spin_right();
        return;
    }

    /* Spin */
    motor_spin_left();

    while( ! (SENSOR1_BIT && SENSOR2_BIT));	/* until sensor1 and 2 in line meanwhile */

    motor_slow();
}

void trace_spin_right(void)
{
    uint8_t case_tmp = nr_case;

    motor_brake();
    motor_slow();
    /* case */
    if (SENSOR1_BIT && SENSOR2_BIT) {
        while(SENSOR1_BIT && SENSOR2_BIT) {
            if( ! SENSOR3_BIT) {
                if((flag_spcecial == 1) && (case_tmp == case8_turn_away)) {
                    motor_spin_right();
                    while(SENSOR1_BIT);
                    motor_slow();
                }
                goto normal_spin;
                return;
            }
        }
        if( ! SENSOR0_BIT) {
            goto normal_spin;
        }
        /* case 8 */
        LED3_TOGGLE;//case 8
        if(nr_case == case8_turn_enter) {
            if(flag_spcecial == 1) {	/* turn left */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_left();
                return;
            }
            else {		/* turn right */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_right();
                return;
            }
        }
        if(nr_case == case8_turn_away) {
            if(flag_spcecial == 1) {	/* turn right */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_right();
                return;
            }
            else {		/* turn left */
                while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                trace_spin_left();
                return;
            }
        }

    }

normal_spin:
    while(SENSOR1_BIT && SENSOR2_BIT);

    /* now sensor1 or 2 are pass the line or both , but need to check the wrong signal */
    /* when need spin left the sensor2 must be 0 , and sensor1 must be 0 when spin right */
    if(SENSOR1_BIT) {
        trace_spin_left();
        return;
    }

    /* Spin */
    motor_spin_right();

    while( ! (SENSOR1_BIT && SENSOR2_BIT));	/* until sensor1 and 2 in line meanwhile */

    motor_slow();
}

#if 0
void trace_spin_left(void)
{
    motor_brake();
    motor_slow();
    /* Case */
    if (SENSOR1_BIT && SENSOR2_BIT) {
        systick_delay_us(100000);	//50ms wait to check
        if( ! (SENSOR1_BIT && SENSOR2_BIT) ) {
            goto normal_spin;
        }

        if (SENSOR0_BIT && SENSOR3_BIT) {		/* CASE 8 */
            LED3_TOGGLE;//////
            if(flag_spcecial) {
                if(nr_case == case8_turn_enter) {
                    if(flag_spcecial == 1) {	/* turn left */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_left();
                        return;
                    }
                    else {		/* turn right */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_right();
                        return;
                    }
                }
                if(nr_case == case8_turn_away) {
                    if(flag_spcecial == 1) {	/* turn right */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_right();
                        return;
                    }
                    else {		/* turn left */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_left();
                        return;
                    }
                }
            }
            while(SENSOR1_BIT && SENSOR2_BIT);	/* sensor 1 or 2 pass the line */
            goto normal_spin;
        }
        //if((nr_case == case8_turn_enter) && (flag_spcecial == 1)){

        if((nr_case == case8_turn_away) && (flag_spcecial == 2)) {
            while(SENSOR0_BIT);
            motor_spin_left();
            while(SENSOR2_BIT);
            trace_spin_left();
            return;
        }

        while(SENSOR1_BIT && SENSOR2_BIT);	/* until sensor1 or 2 pass the line */
    }

normal_spin:
    /* now sensor1 or 2 are pass the line or both , but need to check the wrong signal */
    /* when need spin left the sensor2 must be 0 , and sensor1 must be 0 when spin right */
    if(SENSOR2_BIT) {
        trace_spin_right();
        return;
    }

    /* Spin */
    motor_spin_left();

    while( ! (SENSOR1_BIT && SENSOR2_BIT));	/* until sensor1 and 2 in line meanwhile */

    motor_slow();
}
#endif

#if 0
void trace_spin_right(void)
{
    motor_brake();
    motor_slow();
    /* Case */
    if (SENSOR1_BIT && SENSOR2_BIT) {
        systick_delay_us(100000);	//50ms wait to check
        if( ! (SENSOR1_BIT && SENSOR2_BIT) ) {
            goto normal_spin;
        }

        if (SENSOR0_BIT && SENSOR3_BIT) {		/* CASE 8 */
            if(flag_spcecial ) {
                if(nr_case == case8_turn_enter) {
                    if(flag_spcecial == 1) {	/* turn left */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_left();
                        return;
                    }
                    else {		/* turn right */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_right();
                        return;
                    }
                }
                if(nr_case == case8_turn_away) {
                    if(flag_spcecial == 1) {	/* turn right */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_right();
                        return;
                    }
                    else {		/* turn left */
                        while(SENSOR1_BIT || SENSOR2_BIT);	/* sensor 1 and 2 pass the line meanwhile */
                        trace_spin_left();
                        return;
                    }
                }
            }

            while(SENSOR1_BIT && SENSOR2_BIT);	/* sensor 1 or 2 pass the line */
            goto normal_spin;
        }

        if((nr_case == case8_turn_away) && (flag_spcecial == 1)) {
            while(SENSOR3_BIT);
            motor_spin_right();
            while(SENSOR1_BIT);	/* until sensor1 and sensor2 is 0 */
            goto normal_spin;
        }

        while(SENSOR1_BIT && SENSOR2_BIT);	/* until sensor1 or 2 pass the line */
    }

normal_spin:
    /* now sensor1 or 2 are pass the line or both , but need to check the wrong signal */
    /* when need spin left the sensor2 must be 0 , and sensor1 must be 0 when spin right */
    if(SENSOR1_BIT) {
        trace_spin_left();
        return;
    }

    motor_spin_right();

    while(! (SENSOR1_BIT && SENSOR2_BIT) );

    motor_slow();
}
#endif









