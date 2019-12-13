typedef struct Object {
    int index; // The number of object you're on
    int startAngle; // the starting degree when the first edge of an object was found
    int endAngle; // the ending degree when you get past the object
    double width; // Width of the object.
    double distance; // distance in cm picked up by ir sensor and ping
    double middleValue; // middle of the object in degrees.
} Object;

/**
* This sweeps a servo motor 180 degrees and looks
* for objects, and then returns an array of those objects.
*/
Object *sweep(void);
