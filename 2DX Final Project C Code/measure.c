#include <stdint.h>
#include "PLL.h"
#include "SysTick.h"
#include "uart.h"
#include "onboardLEDs.h"
#include "tm4c1294ncpdt.h"
#include "VL53L1X_api.h"
#include "I2C_init.h"
#include "variables.h"
#include "spin_motor.h"
#include "bus_speed_demo.h"

uint16_t dev = 0x29; // address of the ToF sensor as an I2C slave peripheral
int status = 0;
int measurements[SAMPLE_SIZE];
uint16_t Distance;
uint8_t RangeStatus;
uint8_t dataReady;
uint8_t sensor_status = 0; 

void sensor_init(void)
{
	while (sensor_status == 0)
	{
			status = VL53L1X_BootState(dev, &sensor_status);
			SysTick_Wait10ms(100);
	}
}

void en_sensor(void)
{
		status = VL53L1X_ClearInterrupt(dev); /* clear interrupt has to be called to enable next interrupt*/
    status = VL53L1X_SensorInit(dev);
    Status_Check("SensorInit", status);

    status = VL53L1X_StartRanging(dev); /* This function has to be called to enable the ranging */
    Status_Check("StartRanging", status);
}

void print_measure(void)
{
	for (int i = 0; i < SAMPLE_SIZE; i++)
	{ // print values to uart one by one from array
			sprintf(printf_buffer, "%u,", measurements[i]);
			signal = UART_InChar(); // wait for acknowledgement from PC
			if (signal == 0x31)
			{
					UART_printf(printf_buffer); // send the measurement
			}
	}
}

void measure(void)
{
	
	if (spin_status)
	{
		for (int t = 0; t < TOTAL_MEASURES; t++)
		{
				for (int i = 0; i < SAMPLE_SIZE; i++) // Take a measurment every step of the motor for TOTAL_MEASURES times
				{ 
					GPIO_PORTN_DATA_R |= 0b010;
						while (dataReady == 0)
						{
								status = VL53L1X_CheckForDataReady(dev, &dataReady);
								FlashLED2(1);
						
							VL53L1_WaitMs(dev, 2);
						}

						dataReady = 0;
						status = VL53L1X_GetRangeStatus(dev, &RangeStatus); // ensure sensor status is good
						status = VL53L1X_GetDistance(dev, &Distance);       // get the distance from the sensor
						FlashLED2(1); //Flash LED 2 

						measurements[i] = Distance;              // add distance measure to the distance array 
						status = VL53L1X_ClearInterrupt(dev); //Clear interrupt to enable next one 

						
						spin_motor(512 / SAMPLE_SIZE, CW);
				}
				
				print_measure();
				GPIO_PORTN_DATA_R &= 0b101;
				spin_motor(512, CCW); //Once the frame is done, reset to home
				spin_status = 0;

		}
	}
}

