/*
 *  BMP_180.c
 *
 *  Created: 9/12/2016 17:44:20
 *  Author: Dimitrios St
 *  Linker settings for sprintf: https://startingelectronics.org/articles/atmel-AVR-8-bit/print-float-atmel-studio-7/
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "LCD.h"
#include "DHT.h"
#include "BMP180.h"

int main(void)
{
	char conversion[10] = {'\0'}; //Fill the array with null characters
	int16_t temp = 0, dht_temp = 0; //Temperature and humidity variables

	double press = 0; //Save the pressure
	double dew_p = 0; //Save the dew point
	uint16_t humidity = 0; //Save the raw humidity reading

	//Initialize the components
	InitLCD(); //Initialize the LCD
	TWIInit(); //Initialize the I2C
	BMP180_Init(); //Initialize the BMP180 sensor
	DHT_Init(); //Initialize the DHT22
	_delay_ms(2000); //Delay for the initialization of the components
	
	//Write the fixed strings to the LCD once
	LCD_SetCursor(0, 1); //Set the cursor to the start of the screen and first row
	LCD_WriteStr("Tempr.: "); //Write the required string on the screen
	LCD_SetCursor(0, 2); //Set the cursor to the start of the screen and second row
	LCD_WriteStr("Dew P.: "); //Write the required string on the screen
	LCD_SetCursor(0, 3); //Set the cursor to the start of the screen and third row
	LCD_WriteStr("Rel Hu: "); //Write the required string on the screen
	LCD_SetCursor(0, 4); //Set the cursor to the start of the screen and fourth row
	LCD_WriteStr("Press.: "); //Write the required string on the screen
	
	
    while(1)
    {
		temp = BMP180_Get_Temp(); //Get the temperature reading from the BMP sensor and save it to temp
		press = BMP180_Sea_Level_Press(42); //Get the pressure reading, calibrated for the altitude
		DHT_GetMeteoData(&dht_temp, &humidity); //Get the data from the DHT sensor
		dew_p = ((243.04*(log(humidity/1000.0)+((1.7625*temp)/(243.04+(temp/10.0)))))/(17.625-log(humidity/1000.0)-((1.7625*temp)/(243.04+(temp/10.0))))); //Calculate the dew point for later use

		/* The following procedure of showing the data on the LCD is the same for all the data, so i will describe it here briefly.
		 * First we set the cursor at the position one space after the ':' character, in order to show the data.
		 * Then we clear the space from there up to the end of the screen, to reassure no junk characters are shown.
		 * After cleaning the cursor is reset and the printing process begins.
		 * To print the data we first convert the integer or double to ASCII, in order to make it to a printable format.
		 * For the above conversion we use the sprintf function from the stdio.h library.
		 * After the conversion we print the desired data.
		*/		

		//Show the temperature on the LCD
		LCD_SetCursor(8, 1);
		LCD_WriteStr("         ");
		LCD_SetCursor(8, 1);
		sprintf(conversion, "%.1f C", temp/10.0); //Divide by 10 because the read temperature is multiplied by 10
		LCD_WriteStr(conversion);
		
		//Show the calculated dew point on the LCD
		LCD_SetCursor(8, 2);
		LCD_WriteStr("         ");
		LCD_SetCursor(8, 2);
		sprintf(conversion, "%.1f C", dew_p);
		LCD_WriteStr(conversion);
		
		//Show the humidity
		LCD_SetCursor(8, 3);
		LCD_WriteStr("          ");
		LCD_SetCursor(8, 3);
		sprintf(conversion, "%.1f%%", humidity/10.0); //Divide by 10 because the read humidity from the sensor is multiplied by 10
		LCD_WriteStr(conversion);
		
		//Show the pressure
		LCD_SetCursor(8, 4);
		LCD_WriteStr("          ");
		LCD_SetCursor(8, 4);
		sprintf(conversion, "%.1f hPa", press);
		LCD_WriteStr(conversion);
		
		_delay_ms(3200); //Set your desired update time. Be careful not to set it below 2 seconds, because the DHT needs to rest for that time to make another measurment
    }
}