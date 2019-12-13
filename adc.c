#include "adc.h"
#include "math.h"
#include "open_interface.h"
#include "movement.h"

void adc_init(void) {
    // Enable clock on Port B.
    SYSCTL_RCGCGPIO_R |= 0x2;
    // Enable clock on ADC0.
    SYSCTL_RCGCADC_R |= 0x1;
    // Turn on alternative functions on PB1
    GPIO_PORTB_AFSEL_R |= 0x10;
    // Set PB1 to output.
    GPIO_PORTB_DIR_R |= 0x10;
    GPIO_PORTB_DEN_R &= 0xEF;
    GPIO_PORTB_AMSEL_R |= 0x10;
    GPIO_PORTB_ADCCTL_R = 0;

    // Disable ADC for configuration.
    ADC0_ACTSS_R &= 0xFFFE;
    // Set event to default
    ADC0_EMUX_R &= 0xFFF0;
    // Set bits 1 and 3 to be sampled
    ADC0_SSMUX0_R |= 0xA;
    ADC0_SSCTL0_R = 0x0;
    ADC0_SSCTL0_R |= 0x6;
    // Average 64 samples.
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    // Re-enable the ADC.
    ADC0_ACTSS_R |= 0x1;
}

int adc_read(void) {
    // Start Sampling.
    ADC0_PSSI_R |= 0x1;
    while ((ADC0_RIS_R & 0x1) == 0) {}
    // End Sampling.
    ADC0_ISC_R |= 0x1;
    return ADC0_SSFIFO0_R;
}
