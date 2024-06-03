#include <stdint.h>
#include "PLL.h"
#include "SysTick.h"
#include "tm4c1294ncpdt.h"

void PortM_Init(void)
{
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;                 // Activate the clock for Port M
	while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R11) == 0){};      // Allow time for clock to stabilize
	GPIO_PORTM_DIR_R = 0b00000001;       								      // Enable PM0 as output
  GPIO_PORTM_DEN_R = 0b00000001;														// Enable PM0 as digital pins														
	return;
}
void PortN_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x010;        								// make PN1 out (PN1 built-in LED1)
  GPIO_PORTN_DEN_R |= 0x010;        								// enable digital I/O on PN0
}

void bus_demo(void) //This function simply toggles a GPIO pin to check the Bus speed using AD2
{
	GPIO_PORTM_DATA_R ^= 0x01;
	SysTick_Wait(300000);
	GPIO_PORTM_DATA_R ^= 0x01;
	SysTick_Wait(300000);
}

