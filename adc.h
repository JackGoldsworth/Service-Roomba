
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"


/**
* Initializes the adc.
*/
void adc_init(void);

/**
* Retrieves the information for the adc.
*/
int adc_read(void);
