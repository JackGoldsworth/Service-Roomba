#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "timer.h"

/**
* Initialize the servo motor.
*/
void servo_init(void);

/**
* Move the servo to a certain degree.
*/
int servo_move(float degrees);
