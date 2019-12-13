#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "servo.h"
#include "adc.h"
#include <math.h>
#include <string.h>
#include "time.h"
#include "object_scanner.h"
#include "open_interface.h"
#include "uart.h"

Object* sweep(void) {

    uart_sendStr("Scanning for objects...\r\r");

    static Object objArray[10]; // Array of all objects found

    lcd_clear();

    int i;
    // Reset the objArray since it's static and always in memory.
    for(i = 0; i < 10; i++) {
        Object obj = {};
        objArray[i] = obj;
    }

    int range = 50; // 43 CM
    int objectIndex = 0;
    double adc_distance = 0; // analog to digital distance.
    int currentDistanceIndex = 0; // Current distances in the current object.
    double distances[200]; // Distances to be averaged for each object.
    float degree = 0; // Current degrees

    // Move 180 degrees
    while(degree < 181){
        servo_move(degree);
        adc_distance = 110831 * pow(adc_read(), -1.173); // Bot 9
        //adc_distance = 127350 * pow(adc_read(), -1.291); // Bot 8
        // Others?  (pow(adc_read(), -1.083)*57496);//49869 * pow(adc_read(), -1.066); //

        // Detect Objects
        if(adc_distance <= range && objArray[objectIndex].startAngle == 0 && degree > 3) {
            Object obj = {};
            obj.startAngle = degree;
            obj.index = objectIndex;
            objArray[objectIndex] = obj;
        } else if (adc_distance > range && objArray[objectIndex].startAngle != 0) { // Stop detecting the object.
            Object obj = objArray[objectIndex];
            obj.endAngle = degree;
            obj.middleValue = ((obj.endAngle - obj.startAngle) / 2) + obj.startAngle;
            // Average all the distances for this object to get a more precise distance.
            int k;
            int minimumDistance = 0;
            for(k = 0; k < currentDistanceIndex; k++) {
                minimumDistance += distances[k];
            }
            obj.distance = minimumDistance / currentDistanceIndex;
            for(k = 0; k < 200; k++) {
                distances[k] = 0;
            }

            // Calculate the width of the object (Usually it's within 1cm of actual).
            obj.width = 2 * (minimumDistance / currentDistanceIndex) * tan(((obj.endAngle - obj.startAngle + 0.2) / 2) * M_PI / 200);
            objArray[objectIndex] = obj;
            objectIndex++;
            currentDistanceIndex = 0;
        }

        // Add to the distances if this object has been found.
        if (objArray[objectIndex].startAngle != 0) {
            distances[currentDistanceIndex] = adc_distance;
            currentDistanceIndex++;
        }

        // Print after finding each object.
        if(objectIndex - 1 != -1) {
            lcd_printf("Current Index: %d \nCurrent Distance: %dWidth: %d", objectIndex, (int) objArray[objectIndex - 1].distance, (int) objArray[objectIndex - 1].width);
        }

        degree += 1; // Move one degree to the left.
        timer_waitMillis(50);
    }

    // Complete the last pole.
    Object last = objArray[objectIndex];
    if(last.startAngle != 0 && last.endAngle == 0) {
        last.endAngle = degree;
        last.middleValue = ((last.endAngle - last.startAngle) / 2) + last.startAngle;
        int k;
        int minimumDistance = 0;
        for(k = 0; k < currentDistanceIndex; k++) {
            minimumDistance += distances[k];
        }
        last.distance = minimumDistance / currentDistanceIndex;
        last.width = 2 * (minimumDistance / currentDistanceIndex) * tan(((last.endAngle - last.startAngle + 0.2) / 2) * M_PI / 200);
        objArray[objectIndex] = last;
    }

    // Remove ghost objects.
    for(i = 0; i < 10; i++) {
        Object obj = {};
        // If the distance doesn't match up or the width is tiny.
        if(objArray[i].distance > range + 5 || objArray[i].width < 2) {
            objArray[i] = obj;
        } else {
//            char string[] = "Found Object!\rDistance: " + objArray[i].distance + "\rWidth: " + objArray[i].width + "\rAngle: " + objArray[i].middleValue + "\r";
            char string[50];
            snprintf(string, 50, "Found Object!\rDistance: %d\rWidth: %d\rAngle: %d\r\r", (int) objArray[i].distance, (int) objArray[i].width, (int) objArray[i].middleValue);
            uart_sendStr(string);
        }
    }
    return objArray;
}
