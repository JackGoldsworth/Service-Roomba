/*
 * @author Aryan Joshi and Conner Makoben
 *@date 9/11/19
 *
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

 /**
 * Detects the tape on the ground for boundaries.
 */
int detect_tape(oi_t* sensor);

/**
* Detects where a cliff is, and avoids it.
*/
int detect_cliff(oi_t* sensor);

/**
* Detects when a object is bumped.
*/
int detect_bumper(oi_t* sensor);

/**
 * move forward a certain amount of centimeters. If an object
 * is hit with the left or right bumper, stop the wheels and return
 * the current distance moved. Otherwise return the total distance moved.
 */
int move_forward(oi_t* sensor, double centimeters);

/**
 * Turns the block clockwise a certain degrees.
 */
void turn_clockwise(oi_t* sensor, double degrees);

/**
 * Turns the block counter clockwise a certain degrees.
 */
void turn_counter_clockwise(oi_t* sensor, double degrees);

/**
 * Moves the bot backwards by a certain amount of centimeters.
 */
void move_backward(oi_t* sensor, double centimeters);
