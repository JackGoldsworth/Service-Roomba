#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include "open_interface.h"
#include "uart.h"


int detect_tape(oi_t *sensor) {
    oi_update(sensor);
    int tape = 0;
    if(sensor->cliffFrontLeftSignal > 2600 || sensor->cliffFrontRightSignal > 2600) { // was 2400
        move_backward(sensor, 30);
        turn_clockwise(sensor, 70);
        tape = 1;
        uart_sendStr("Detected tape in the front.\r\r");
    } else if(sensor->cliffRightSignal > 2600) {
        turn_counter_clockwise(sensor, 70);
        tape = 1;
        uart_sendStr("Detected tape on the right.\r\r");
    } else if(sensor->cliffLeftSignal > 2600) {
        turn_clockwise(sensor, 70);
        tape = 1;
        uart_sendStr("Detected tape on the left.\r\r");
    }
    return tape;
}

int detect_bumper(oi_t *sensor) {
    oi_update(sensor);
    int bumper = 0;
    if(sensor->bumpLeft) {
        move_backward(sensor, 30);
        turn_clockwise(sensor, 30);
        bumper = 1;
        uart_sendStr("Bumped into an object on the left.\r\r");
    } else if(sensor->bumpRight) {
        move_backward(sensor, 30);
        turn_counter_clockwise(sensor, 30);
        bumper = 1;
        uart_sendStr("Bumped into an object on the right.\r\r");
    }
    return bumper;
}

int detect_cliff(oi_t *sensor) {
    oi_update(sensor);
    int cliff = 0;
    if(sensor->cliffFrontLeft || sensor->cliffLeft) {
        move_backward(sensor, 30);
        turn_clockwise(sensor, 60);
        move_forward(sensor, 20);
        cliff = 1;
        uart_sendStr("Found a cliff on the left.\r\r");
    } else if(sensor->cliffFrontRight || sensor->cliffRight) {
        move_backward(sensor, 30);
        turn_counter_clockwise(sensor, 60);
        move_forward(sensor, 20);
        cliff = 1;
        uart_sendStr("Found a cliff on the right.\r\r");
    }
    return cliff;
}

int move_forward(oi_t *sensor, double centimeters)
{
    oi_setWheels(120, 100);
    double sum = 0;

    while (sum < centimeters) {
        oi_update(sensor);
        int cliff = detect_cliff(sensor);
        int tape = detect_tape(sensor);
        int bumper = detect_bumper(sensor);
        sum += sensor->distance;
        if(sensor->bumpLeft || sensor->bumpRight || cliff == 1 || tape == 1 || bumper == 1) {
            oi_setWheels(0, 0);
            return tape;
        }
    }
    oi_setWheels(0, 0); // stop
    return 0;
}


void turn_clockwise(oi_t *sensor, double degrees)
{
     oi_setWheels(-100, 100);
     double sum = 0;
     while (sum < degrees)
     {
         oi_update(sensor);
         sum += -sensor->angle;
     }
     oi_setWheels(0, 0); // stop
}

void turn_counter_clockwise(oi_t *sensor, double degrees)
{
     oi_setWheels(100, -100);
     double sum = 0;

     while (sum < degrees)
     {
         oi_update(sensor);
         sum += sensor->angle;
     }
     oi_setWheels(0, 0); // stop
}

void move_backward(oi_t *sensor, double centimeters)
{
    oi_setWheels(-100, -100);
    double sum = 0;

    while (sum < centimeters)
    {
        oi_update(sensor);
        sum -= sensor->distance;
    }
    oi_setWheels(0, 0); // stop
}
