/*
 * Mini Project 2
 *
 *  Created on: May 4, 2018
 *      Author: IAA
 */

#include "lcd.h"
#include "adc.h"

uint8 tick=0;
ISR(INT1_vect)
{
	tick++;
	LCD_goToRowColumn(3,0);
	LCD_displayString("                ");
	TOGGLE_BIT(PORTB,PB0);
	TOGGLE_BIT(PORTB,PB1);
	if (tick == 1){
	LCD_goToRowColumn(3,0);
	LCD_displayString("clockwise     ");
	}
	if (tick == 2){
	LCD_goToRowColumn(3,0);
	LCD_displayString("Anti-clockwise  ");
	tick=0;
	}

}


void PWM_Timer0_Init(unsigned char set_duty_cycle){

	TCNT0 = 0; //initial timer value

	OCR0  = set_duty_cycle;

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}


void INT1_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	SET_BIT(DDRD,PD3);
	GICR  |= (1<<INT1);                 // Enable external interrupt pin INT0
	MCUCR |= (1<<ISC11) | (1<<ISC10);   // Trigger INT0 with the raising edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}



int main (void){

	uint16 adc_value;
	CLEAR_BIT(DDRA,PA0);
	SET_BIT(DDRB,PB0),
	SET_BIT(DDRB,PB1),
	SET_BIT(DDRB,PB3);

	DDRC = 0xff;
	SET_BIT(DDRD,PD0),
	SET_BIT(DDRD,PD1);
	SET_BIT(DDRD,PD2);

	//PWM_Timer0_Init(128);
	LCD_init();
	ADC_init();
	INT1_Init();
	LCD_clearScreen();

	LCD_displayString("ADC Value = ");
	LCD_goToRowColumn(1,0);
	LCD_displayString("MotorSpeed = ");
	LCD_goToRowColumn(2,0);
	LCD_displayString("Rot Direction: ");
	PORTB &= ~((1<<PB0) | (1<<PB1));
	SET_BIT(PORTB,PB0);

	LCD_goToRowColumn(3,0);
	LCD_displayString("clockwise");

	while (1)
	{

		LCD_goToRowColumn(0,12);
		adc_value = ADC_readChannel(0);
		LCD_intgerToString(adc_value);

		LCD_goToRowColumn(1,13);
		PWM_Timer0_Init(adc_value/4);
		LCD_intgerToString(adc_value/4);




	}
}


