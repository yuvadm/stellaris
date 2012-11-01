#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

int main(void) {
    unsigned long ulPeriod, dutyCycle;

    // 40 MHz system clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|
        SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    ulPeriod = 100000;
    dutyCycle = 25000;

    // Turn off LEDs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

    // Configure PB6 as T0CCP0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB6_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);

    // Configure timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod -1);
    TimerMatchSet(TIMER0_BASE, TIMER_A, dutyCycle); // PWM
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) { // The following code varies the duty cycle over time
        TimerMatchSet(TIMER0_BASE, TIMER_A, dutyCycle++);

        if(dutyCycle >= ulPeriod - 1)
            dutyCycle = 0;

        SysCtlDelay(50000);
    }
}
