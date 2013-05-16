#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"

void Timer0IntHandlerC()
{
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  // TIMER0_BASE 0x40030000
  // TIMER_TIMA_TIMEOUT 0x00000001
  // TIMER_O_ICR 0x00000024

  // update this reg 0x40030024 = 0x00000001

  __asm__("nop");
}

void main()
{
  /* run at 40 mhz */
  SysCtlClockSet(SYSCTL_SYSDIV_5 |
                 SYSCTL_USE_PLL |
                 SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_16MHZ);
  
  /* enable led */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
  
  /* enable vga rgb ping */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);   // r
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);   // g
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);   // b
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4);   // i
  
  /* enable vga sync pins */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);   // vsync
  GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1);   // hsync
  
  /* enable & configure timer + interrupt */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  IntMasterEnable();
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER0_BASE,
               TIMER_A,
               1056);     /* 26.4us (1056 cycles @ 40 mhz) */
  IntEnable(INT_TIMER0A);
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  TimerEnable(TIMER0_BASE, TIMER_A);

  
  while (1)
  {
    /* blink led */
    int i = 0xffff;

    
while(i--);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0xff);
    SysCtlDelay(SysCtlClockGet() / 100);
while(i--);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    SysCtlDelay(SysCtlClockGet() / 100);
  }
}
