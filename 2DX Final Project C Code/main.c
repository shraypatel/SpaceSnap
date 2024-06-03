/* 2DX3 Project Deliverable by: Shray Patel */

/*  Time of Flight for 2DX4 -- Studio W8-0
                Code written to support data collection from VL53L1X using the Ultra Light Driver.
                I2C methods written based upon MSP432E4 Reference Manual Chapter 19.
                Specific implementation was based upon format specified in VL53L1X.pdf pg19-21
                Code organized according to en.STSW-IMG009\Example\Src\main.c
                
                The VL53L1X is run with default firmware settings.


            Written by Tom Doyle
            Updated by  Hafez Mousavi Garmaroudi
            Last Update: March 17, 2020
						
						Last Update: March 03, 2022
						Updated by Hafez Mousavi
						__ the dev address can now be written in its original format. 
								Note: the functions  beginTxI2C and  beginRxI2C are modified in vl53l1_platform_2dx4.c file
								
						Modified March 16, 2023 
						by T. Doyle
							- minor modifications made to make compatible with new Keil IDE

*/
#include <stdint.h>
#include "PLL.h"
#include "SysTick.h"
#include "uart.h"
#include "onboardLEDs.h"
#include "tm4c1294ncpdt.h"
#include "VL53L1X_api.h"
#include "variables.h"
#include "I2C_init.h"
#include "spin_motor.h"
#include "onboard_button.h"
#include "measure.h"
#include "bus_speed_demo.h"

int main(void) {
	
	//Initialize Sequence
	PLL_Init();
	PortH_Init();
	SysTick_Init();
	onboardLEDs_Init();
	I2C_Init();
	UART_Init();
	PortJ_Init();
	PortJ_Interrupt_Init();
	PortM_Init();

	
	sensor_init(); //initialize the sensor 
	
	FlashAllLEDs();
	
	en_sensor(); //enable sensor sequence
	
	while(1)
	{
		//bus_demo(); // To Check bus speed uncomment this and comment measure(); 
		measure(); // if interrupt called, measure
	}
}

