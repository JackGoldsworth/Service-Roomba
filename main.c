#include "open_interface.h"
#include "adc.h"
#include "cyBot_uart.h"
#include "movement.h"
#include "servo.h"
#include "timer.h"
#include "lcd.h"
#include "object_scanner.h"
#include <math.h>
#include "uart.h"

#define TRUE 1
#define FALSE 0

/**
* This is the main function that runs in an infinite loop
* until the finish zone is found.
*/
void run(oi_t *sensor);

/**
* This function calls the sweep function and then
* parses the information to find out how many small pillars
* were found.
*/
int scanObjects(oi_t *sensor, int searchingZone);

/**
* This function plays the song when the zone is found.
*/
void playSong(void);

int totalObjects = 0;

int main(void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();
    uart_init();
    adc_init();
    servo_init();
    timer3B_init();
    run(sensor_data);
    oi_free(sensor_data);
    return 0;
}

void run(oi_t *sensor) {
    int timesThrough = 0;
    int foundIssues = 0;
    int issueFlag = 0;
    while (1) {

        // Look for pillars and other objects.
        int objectsFound = scanObjects(sensor, FALSE);
        if (objectsFound > 1 && objectsFound < 4) {
            // Turn around and look for two more.
            turn_clockwise(sensor, 172);
            objectsFound += scanObjects(sensor, TRUE);
            // If we find more then three in total, we found the zone.
            if (objectsFound > 3) {
                uart_sendStr("You're service dog brought it's owner to the target zone!\r\r");
                lcd_printf("Target Zone!");
                // Play song
                playSong();
                turn_counter_clockwise(sensor, 340);
                break;
            } else {
                turn_counter_clockwise(sensor, 170);
            }
        }

        // If it finds zero objects, go farther than if it does.
        if (totalObjects == 0) {
            move_forward(sensor, 90);
        } else {
            move_forward(sensor, 50);
        }
        // If we run into a wall twice in a row, do a 180 and get out of there.
        if (timesThrough > 2 && foundIssues > 1) {
            turn_counter_clockwise(sensor, 170);
            timesThrough = 0;
            foundIssues = 0;
        }
        timer_waitMillis(70);
    }
}

int scanObjects(oi_t *sensor, int searchingZone) {
    servo_move(0);
    totalObjects = 0;
    Object *objects = sweep();
    int pillarCount = 0; // Count the small pillars to see if we can find the safe zone.
    int pillarAngle = 0; // Angle we need to move to, to be in between the pillars.
    int pillarDistance = 0; // Distance to the pillars.
    int i;
    for (i = 0; i < 10; i++) {
        Object curObj = objects[i];
        // Count the pillars.
        if (curObj.width > 2 && curObj.width < 9) {
            pillarCount++;
            pillarAngle += objects[i].middleValue;
            pillarDistance += objects[i].distance;
            lcd_printf("Pillar Count: %d \nDistance: %d\n Angle: %d", pillarCount, pillarDistance, pillarAngle);
            timer_waitMillis(2000);
        }
        // Count the total objects found.
        if (curObj.width > 0) {
            totalObjects++;
        }
        int direction = (int) (90 - curObj.middleValue);

        // Dodge objects that are in front.
        if (direction < 40 && direction > -40 && curObj.width > 2 && curObj.distance < 30) {
            move_backward(sensor, 30);
            if (direction < 0) {
                turn_clockwise(sensor, 30);
            } else {
                turn_counter_clockwise(sensor, 30);
            }
        }
    }

    return pillarCount;
}

void playSong(void) {
    unsigned char mario1NumNotes = 49;
    unsigned char mario1Notes[49] = {48, 60, 45, 57, 46, 58, 0, 48, 60, 45, 57, 46, 58, 0, 41, 53, 38, 50, 39, 51, 0,
                                     41, 53, 38, 50, 39, 51, 0, 51, 50, 49, 48, 51, 50, 44, 43, 49, 48, 54, 53, 52, 58,
                                     57, 56, 51, 47, 46, 45, 44};
    unsigned char mario1Duration[49] = {12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12,
                                        62, 12, 12, 12, 12, 12, 12, 48, 8, 8, 8, 24, 24, 24, 24, 24, 24, 8, 8, 8, 8, 8,
                                        8, 16, 16, 16, 16, 16, 16};
    oi_loadSong(0, mario1NumNotes, mario1Notes, mario1Duration);
    oi_play_song(0);
}
