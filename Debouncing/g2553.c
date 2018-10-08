#include <msp430.h> 


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1REN |= BIT3; //pull up resistor on button
    P1OUT |= BIT3; //pulls up resistor
    P1DIR |= BIT0; //set

    P1IE |= BIT3; //enable interrupt on button
    P1IES |= BIT3; //interrupts on negative edge
    P1IFG &= ~BIT3; //clear interrupt flag

__bis_SR_register(LPM0_bits + GIE); //enter interrupt mode and low power mode
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    P1IE &= ~BIT3;              //disable interrupt
        WDTCTL = WDT_MDLY_32;   //Watchdog timer enabled
        IE1 |= WDTIE;           //enable WDT interrupt
        P1IFG &=~BIT3;          //clear flag
    }


#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    IE1 &= ~WDTIE;              //disable WDT
    IFG1 &= ~WDTIFG;            //clears flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog

    if (!(P1IN & BIT3))         //switch LED
    {
        P1OUT ^= BIT0;
    }
    P1IE |= BIT3;               //enable button interrupt

}
