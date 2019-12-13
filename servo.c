#include "servo.h"

int servo_move(float degrees) {
    int value = 320000 - (7112 + (153.08 * degrees));
    TIMER1_TBMATCHR_R = value & 0xFFFF;
    TIMER1_TBPMR_R = (value >> 16) & 0xFF;
    return value;
}

void servo_init() {
    // Initialize Port B.
    SYSCTL_RCGCGPIO_R |= 0b10;
    // Set PB5 to use an alternative function.
    GPIO_PORTB_AFSEL_R |= 0b100000;
    // Set Port Control to bit 7 on PB5.
    GPIO_PORTB_PCTL_R |= 0x700000;
    // Set PB5 to out and enable it.
    GPIO_PORTB_DIR_R |= 0b100000;
    GPIO_PORTB_DEN_R |= 0b100000;
    // Start clock.
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
    // Disable the timer for configuration.
    TIMER1_CTL_R &= 0xFEFF;
    // Set time configuration to 16 bit.
    TIMER1_CFG_R |= TIMER_CFG_16_BIT;
    // Set timer to periodic mode
    TIMER1_TBMR_R |= 0xA;
    TIMER1_TBPR_R = 0x4;
    // set the first 16 bits for the timer
    TIMER1_TBILR_R = 320000 & 0xFFFF;
    TIMER1_TBPMR_R = 0x00;
    // Set the match value.
    TIMER1_TBMATCHR_R = 0x3E80;
    // Re-enable the timer.
    TIMER1_CTL_R |= 0x0100;
}
