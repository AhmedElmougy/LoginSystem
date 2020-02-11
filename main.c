/*
 * File        : main.c
 * Description : This file includes login application using LCD and keypad for Atmega32
 *				 you need to enter a 4 digit password in order to proceed to system 
 *				 the correct password is 1234 when logged in LCD will display logged in and a green led will be on 
 *				 and a motor that control a door will be on for 5 seconds 
 * Author      : Ahmed Elmogy Embedded SW Engineer 
 * Date        : 10/2/2020
 * Git account : https://github.com/AhmedElmougy
 * mail        : ahmed.m.elmogy@gmil.com 
 */ 


#include "LSTD_Types.h"
#include "LUTILS.h"
#include "LDELAY_Interface.h"
#include "MDIO_Interface.h"
#include "HKPD_Interface.h"
#include "HLCD_Interface.h"
#include "HLED_Interface.h"
#include "HDCMOTOR_Interface.h"

/*global variables*/

/*character*/
volatile u8 character=' ';
/*Data*/
volatile u8 Data=' ';
/*password array*/
volatile u8 PassWord[4];
/*number of characters entered*/
u8 PasswordCounter = 0;


int main(void)
{
	HDCMOTOR_voidInitializeMotor();
	/*Initialize logged in indicator LED at PA0 */
	HLED_u8InitOneLed(MDIO_PORTA,MDIO_PA0);
	/*Initialize Not logged in indicator LED at PA0 */
	HLED_u8InitOneLed(MDIO_PORTA,MDIO_PA1);
    /*Initialize LCD*/
	HLCD_voidInitializeLCD(); 
	/*Initialize Keypad*/
	HKPD_voidInitializeKeypad();      
	/*hide cursor and blinking*/
	HLCD_voidCursorDisplayOnOff(HLCD_OFF);
	HLCD_voidCursorBlinkOnOff(HLCD_OFF);
	/*display welcome on first line*/
	HLCD_voidDisplayString("Welcome:",HLCD_STRING);
	HLCD_voidSelectLine(HLCD_LINE2);
	
	
	while (1) 
    {
		/*set not logged in LED on*/
		HLED_u8LedOnOff(MDIO_PORTA,MDIO_PA1,MDIO_HIGH);
		/*set logged in LED off*/
		HLED_u8LedOnOff(MDIO_PORTA,MDIO_PA0,MDIO_LOW);
		/*read inputed char*/
		character = HKPD_u8GetPressedKey();
		
		/*if a key was pressed*/
		if(character!='?')
		{
			/*if number was pressed*/
			if((character>=0)&&(character<10))
			{
				Data = character + '0';	
			} 
			/*if character was pressed*/
			else
			{
				Data = character;
			
			}/*end of pressed key type check*/
			
			/*display pressed key*/
			HLCD_voidDisplayChar(Data);
			/*save password digit in array*/
			PassWord[PasswordCounter] = character;
			/*wait 500 ms*/
			LDelay_voidDelay_ms(500);
			
			/*counter for number of digits entered*/
			PasswordCounter++;
			/*if entered password digits equal 4*/
			if(PasswordCounter > 3)
			{
				/*if password is correct (1234)*/
				if((PassWord[0]==1)&&(PassWord[1]==2)&&(PassWord[2]==3)&&(PassWord[3]==4))
				{
					/*display logged in*/
					HLCD_voidClearLCD();
					HLCD_voidSelectLine(HLCD_LINE2);
					HLCD_voidDisplayString("Logged in",HLCD_STRING);
					LDelay_voidDelay_ms(3000);
					/*display welcome*/
					HLCD_voidClearLCD();
					HLCD_voidSelectLine(HLCD_LINE1);
					HLCD_voidDisplayString("Welcome",HLCD_STRING);
					HLCD_voidSelectLine(HLCD_LINE2);
					
					
					/*open logged in led*/
					HLED_u8LedOnOff(MDIO_PORTA,MDIO_PA0,MDIO_HIGH);
					/*set Not logged in led off*/
					HLED_u8LedOnOff(MDIO_PORTA,MDIO_PA1,MDIO_LOW);

					/*Move motor right to open door*/
					HDCMOTOR_voidTurnMotorRight(HDCMOTOR_MOTOR1);
					LDelay_voidDelay_ms(5000);
					/*Move motor left to close door again*/
					HDCMOTOR_voidTurnMotorLeft(HDCMOTOR_MOTOR1);
					LDelay_voidDelay_ms(5000);
					/*Stop Motor*/
					HDCMOTOR_voidStopMotor(HDCMOTOR_MOTOR1);
									            
				}
				else /*if password is not correct*/
				{
					/*display wrong pass*/
					HLCD_voidClearLCD();
					HLCD_voidSelectLine(2);
					HLCD_voidDisplayString("Wrong pass",HLCD_STRING);
					LDelay_voidDelay_ms(2000);
					/*return to first page*/
					HLCD_voidClearLCD();
					HLCD_voidSelectLine(HLCD_LINE1);
					HLCD_voidDisplayString("Enter pass",HLCD_STRING);
					HLCD_voidSelectLine(HLCD_LINE2);
					
				}/*end of password is correct check*/
				
				PasswordCounter = 0; /*start from zero again*/
			
			}/*end of number of password digits entered check*/
		
		}/*end of there a key pressed check*/
	
	}/*end of infinite loop*/
	
}/*end of main()*/
