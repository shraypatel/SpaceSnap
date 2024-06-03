#include "tm4c1294ncpdt.h"
#include <stdint.h>
#include "SysTick.h"
#include "spin_motor.h"
#include "variables.h"

int delay = 50000;
char motorSteps[] = {0b00000011, 0b00000110, 0b00001100, 0b00001001};
void PortH_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R7) == 0){};	
	GPIO_PORTH_DIR_R |= 0xF;												
	GPIO_PORTH_AFSEL_R &= ~0xF;		 								
	GPIO_PORTH_DEN_R |= 0xF;																												
	GPIO_PORTH_AMSEL_R &= ~0xF;		 								
	return;
}

void spin_motor(int step, int dir)
{
	for(int i=0; i < step; i++) 
	{
		if(dir == 1) // If motor dir is CW
		{
			for(int j=0; j < 4; j++)
			{
				GPIO_PORTH_DATA_R = motorSteps[j]; // Spin motor each step in the motorSteps Array
				SysTick_Wait(delay);
			}
		}
		else if(dir == 0) // If motor dir is CCW
		{
			for(int j=3; j >=0; j--) // Spin motor each step in reverse order from the motorSteps array
			{
				GPIO_PORTH_DATA_R = motorSteps[j];
				SysTick_Wait(delay);
			}
		}
	}
	
	
}