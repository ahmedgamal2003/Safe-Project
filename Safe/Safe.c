/*
 * Safe.c
 *
 * Created: 9/9/2024 4:07:57 PM
 *  Author: AHMED GAMAL
 */ 


#define F_CPU 8000000UL
#include <util/delay.h>
#include "LCD.h"
#include "keypad_driver.h"
#include "EEPROM_driver.h"

#define EEPROM_STATUS 0x20
#define EEPROM_LOCATION1 0x21
#define EEPROM_LOCATION2 0x22
#define EEPROM_LOCATION3 0x23
#define EEPROM_LOCATION4 0x24
#define MAX_TRY 3

char arr[4];
int main(void)
{
	L:LCD_vInit();
	keypad_vInit();
	
	char flag=0,i,val=NOTPRESSED,try=MAX_TRY;
    if (EEPROM_read(EEPROM_STATUS)==NOTPRESSED)
    {
		LCD_vSend_string("Set pass:");
		for (i=0;i<=3;i++)
		{
			do 
			{
				val=keypad_u8check_press();
			} while (val==NOTPRESSED);
			LCD_vSend_char(val);
			_delay_ms(500);
			LCD_movecursor(1,10+i);
			LCD_vSend_char('*');
			_delay_ms(500);
			EEPROM_write(EEPROM_LOCATION1+i,val);
		}
		EEPROM_write(EEPROM_STATUS,0x00);
    }
	
L2:	while(flag==0)
	{
		LCD_clearscreen();
		arr[0]=arr[1]=arr[2]=arr[3]=NOTPRESSED;
		LCD_vSend_string("Check pass:");
		for (i=0;i<=3;i++)
		{
			do 
			{
				arr[i]=keypad_u8check_press();
				if (arr[i]=='A')
				{
					_delay_ms(200);
					
					LCD_clearscreen();
					LCD_vSend_string("old pass:");
					arr[0]=arr[1]=arr[2]=arr[3]=NOTPRESSED;
					for(i=0;i<=3;i++)
					{
						do 
						{
							arr[i]=keypad_u8check_press();
						} while (arr[i]==NOTPRESSED);
						LCD_vSend_char(arr[i]);
						_delay_ms(500);
						LCD_movecursor(1,10+i);
						LCD_vSend_char('*');
						_delay_ms(500);
					}
					if (EEPROM_read(EEPROM_LOCATION1)==arr[0] && EEPROM_read(EEPROM_LOCATION2)==arr[1] && EEPROM_read(EEPROM_LOCATION3)==arr[2] && EEPROM_read(EEPROM_LOCATION4)==arr[3])
					{
						LCD_clearscreen();
						LCD_vSend_string("Right password");
						_delay_ms(500);
						EEPROM_write(EEPROM_STATUS,NOTPRESSED);
						goto L;
					}
					else
					{
						LCD_clearscreen();
						LCD_vSend_string("Wrong password");
						_delay_ms(500);
						goto L2;
					}
					
				}
			} while (arr[i]==NOTPRESSED);
			
			LCD_vSend_char(arr[i]);
			_delay_ms(500);
			LCD_movecursor(1,12+i);
			LCD_vSend_char('*');
			_delay_ms(500);
		}
		
		if (EEPROM_read(EEPROM_LOCATION1)==arr[0] && EEPROM_read(EEPROM_LOCATION2)==arr[1] && EEPROM_read(EEPROM_LOCATION3)==arr[2] && EEPROM_read(EEPROM_LOCATION4)==arr[3])
		{
			LCD_clearscreen();
			LCD_vSend_string("Right password");
			LCD_movecursor(2,1);
			LCD_vSend_string("Safe opened");
			flag=1;
		}
		else
		{
			try--;
			if (try>0)
			{
				LCD_clearscreen();
				LCD_vSend_string("Wrong password");
				LCD_movecursor(2,1);
				LCD_vSend_string("Tries left:");
				LCD_vSend_char(try+48);
				_delay_ms(1000);
				
			}
			else
			{
				LCD_clearscreen();
				LCD_vSend_string("Wrong password");
				LCD_movecursor(2,1);
				LCD_vSend_string("Safe closed");
				flag=1;
			}
		}
	}	
		
	
}	