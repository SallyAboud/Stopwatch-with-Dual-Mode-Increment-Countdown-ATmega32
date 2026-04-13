/*
 * project2.c
 *
 *  Created on: Sep 9, 2024
 *      Author: SoO
 */

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

#define countUp 1
#define countDown 0

unsigned char count=countUp;

unsigned char sec=0;
unsigned char min=0;
unsigned char hrs=0;

void timer1_Init_CTC(void){
	TCCR1A=(1<<FOC1A);//not PWM mode

	//WGM12->CTC mode, (CS10 & CS12)-> prescaler=1024
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);
	TIMSK|=(1<<OCIE1A);//timer1A output compare match interrupt enable

	//Ttimer=1 Ftimer=1 clock=16Mhz prescaler=1024
	//Ftimer=16Mhz/(1024(1+top))=1 top=15625
	OCR1A=15625;
	//timer initial vaue=0;
	TCNT1=0;
}

ISR(TIMER1_COMPA_vect){

		if(count){//if it is counting up count=1
			sec++;
			if(sec>59){
				sec=0;
				min++;
			}
			if(min>59){
				min=0;
				hrs++;
			}
			if(hrs>23){
				hrs=0;
			}
		}

		else{//if it is counting down count=0

			if(sec!=0){
				sec--;
			}
			else if(min!=0){//if minutes not equal 0 and seconds=0
				min--;
				sec=59;
			}
			else if(hrs!=0){//if  hours not equal 0 and minutes and seconds=0
				hrs--;
				min=59;
				sec=59;
			}
		}

}

void timer(){
	unsigned char sec1=sec%10;//ones of seconds
	unsigned char sec10=sec/10;//tens of seconds
	unsigned char min1=min%10;//ones of minutes
	unsigned char min10=min/10;//tens of minutes
	unsigned char hrs1=hrs%10;//ones of hours
	unsigned char hrs10=hrs/10;//tens of hours


	//display ones of seconds
	//disable all the 7segments except the one displaying the ones of the seconds
	PORTA&=~(1<<PA1)&~(1<<PA2)&~(1<<PA3)&~(1<<PA4)&~(1<<PA5);
	PORTA|=(1<<PA0);
	PORTC=(PORTC&0XF0)|(sec1&0x0F);
	_delay_ms(2);

	//display tens of seconds
	//disable all the 7segments except the one displaying the tens of the seconds
	PORTA&=~(1<<PA0)&~(1<<PA2)&~(1<<PA3)&~(1<<PA4)&~(1<<PA5);
	PORTA|=(1<<PA1);
	PORTC=(PORTC&0XF0)|(sec10&0x0F);
	_delay_ms(2);



	//display ones of minutes
	//disable all the 7segments except the one displaying the ones of the minutes
	PORTA&=~(1<<PA1)&~(1<<PA0)&~(1<<PA3)&~(1<<PA4)&~(1<<PA5);
	PORTA|=(1<<PA2);
	PORTC=(PORTC&0XF0)|(min1&0x0F);
	_delay_ms(2);

	//display tens of minutes
	//disable all the 7segments except the one displaying the tens of the minutes
	PORTA&=~(1<<PA1)&~(1<<PA2)&~(1<<PA0)&~(1<<PA4)&~(1<<PA5);
	PORTA|=(1<<PA3);
	PORTC=(PORTC&0XF0)|(min10&0x0F);
	_delay_ms(2);



	//display ones of hours
	//disable all the 7segments except the one displaying the ones of the hours
	PORTA&=~(1<<PA1)&~(1<<PA2)&~(1<<PA3)&~(1<<PA0)&~(1<<PA5);
	PORTA|=(1<<PA4);
	PORTC=(PORTC&0XF0)|(hrs1&0x0F);
	_delay_ms(2);

	//display tens of hours
	//disable all the 7segments except the one displaying the tens of the hours
	PORTA&=~(1<<PA1)&~(1<<PA2)&~(1<<PA0)&~(1<<PA4)&~(1<<PA3);
	PORTA|=(1<<PA5);
	PORTC=(PORTC&0XF0)|(hrs10&0x0F);
	_delay_ms(2);

}


void INT0_Init(){

	DDRD&=~(1<<PD2);//PD2 is an input

	//on falling edge INT0 generates an interrupt request
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);

	GICR|=(1<<INT0);//enable INT0

}

ISR (INT0_vect){
	//reset by making hours, minutes and seconds=0
	sec=0;
	min=0;
	hrs=0;
}

void INT1_Init(){
	DDRD&=~(1<<PD3);//PD3 is an input
	MCUCR|=(1<<ISC11)|(1<<ISC10);//on rising edge INT1 generates an interrupt request
	GICR|=(1<<INT1);//enable INT1


}

ISR (INT1_vect){

	TIMSK&=~(1<<OCIE1A);//(timer1A output compare match interrupt disable)

}

void INT2_Init(){
	DDRD&=~(1<<PB2);//PB2 is an input
	MCUCR&=~(1<<ISC2);//on falling edge INT0 generates an interrupt request
	GICR|=(1<<INT2);//enable INT2

}

ISR (INT2_vect){

	 TIMSK|=(1<<OCIE1A);//(timer1A output compare match interrupt enable)

}

int main(){

	SREG|=(1<<7);//enable global interrupts
	DDRC|=0X0F;//make first 4 pins in PORTC as outputs
	DDRA|=0X3F;//make first 6 pins in PORTA as outputs
	PORTA|=0X3F;//enable the first 6 pins in PORTA
	PORTC|=0X00;//initialize the 7 segment to 0
	DDRB&=0X00;//make all pins in PORTB as inputs
	PORTB|=0XFF;//make all pins in PORTB internal pull up
	DDRD|=(1<<0)|(1<<4)|(1<<5);//make pin 0,4 and 5 as outputs
	PORTD&=~(1<<0)&~(1<<4)&~(1<<5);//clear pin 0,4 and 5
	PORTD|=(1<<PD2);//make PD2 (INT0) internal pull up
	timer1_Init_CTC();
	INT0_Init();
	INT1_Init();
	INT2_Init();
	while(1){
		timer();


		//hrs increment
		if(!(PINB&(1<<1))){ //check if the push button at PB1 is pressed
			_delay_ms(30);
			if(!(PINB&(1<<1))){ //second check due to switch de-bouncing
				if(hrs==23){
					hrs=0;
				}
				else{
					hrs++;
				}
				while(!(PINB&(1<<1))){//if the button is not released it won't exit the while loop
					timer();
				}
			}
		}

		//hrs decrement
		else if(!(PINB&(1<<0))){ //check if the push button at PB0 is pressed
			_delay_ms(30);
			if(!(PINB&(1<<0))){ //second check due to switch de-bouncing
				if(hrs!=0){
					hrs--;
				}
				while(!(PINB&(1<<0))){//if the button is not released it won't exit the while loop
					timer();
				}
			}
		}


		//minutes increment
		if(!(PINB&(1<<4))){ //check if the push button at PB4 is pressed
			_delay_ms(30);
			if(!(PINB&(1<<4))){ //second check due to switch de-bouncing
				if(min==59){
					min=0;
					hrs++;
				}
				else{
					min++;
				}
				while(!(PINB&(1<<4))){//if the button is not released it won't exit the while loop
					timer();
				}
			}
		}

		//minutes decrement
		else if(!(PINB&(1<<3))){//check if the push button at PB3 is pressed
			_delay_ms(30);
			if(!(PINB&(1<<3))){ //second check due to switch de-bouncing
				if(min!=0){
					min--;
				}
				else if(hrs!=0){
					hrs--;
					min=59;
				}
				while(!(PINB&(1<<3))){//if the button is not released it won't exit the while loop
					timer();
				}
			}
		}


		//seconds increment
		if(!(PINB&(1<<6))){ //check if the push button at PB6 is pressed
			_delay_ms(30);
			if(!(PINB&(1<<6))){ //second check due to switch de-bouncing
				if(sec==59){
					sec=0;
					min++;
					if(min>59){
						min=0;
						if(hrs!=23){
							hrs++;
						}
						else{
							hrs=0;
						}
					}
				}
				else{
					sec++;
				}
				while(!(PINB&(1<<6))){//if the button is not released it won't exit the while loop
					timer();
				}
			}
		}

		//seconds decrement
		else if(!(PINB&(1<<5))){ //check if the push button at PB5 is pressed
			_delay_ms(30);
			if(!(PINB&(1<<5))){ //second check due to switch de-bouncing
				if(sec!=0){
					sec--;
				}
				else if(min!=0){//if seconds =0 and minutes does not =0
					min--;
					sec=59;
				}
				else if(hrs!=0){//if seconds and minutes =0 and hours does not =0
					hrs--;
					min=59;
					sec=59;
				}
				while(!(PINB&(1<<5))){//if the button is not released it won't exit the while loop
					timer();
				}
			}
		}


		//count down
		if(!(PINB&(1<<7))){ //check if the push button at PB7 is pressed
			_delay_ms(30);
			if(!(PINB&(1<<7))){ //second check due to switch de-bouncing
				count=countDown;
				PORTD|=(1<<5);//count down led is on (yellow led)
				if(hrs==0 && min==0 && sec==0){
					PORTD|=(1<<0);//buzzer is on when timer count down reaches 0
				}
				else{
					PORTD&=~(1<<0);//buzzer is off
				}
				PORTD&=~(1<<4);//count up led is off (red led)
			}
		}

		//count up
		else if(PINB&(1<<7)){ //check if the push button at PB7 is released
			_delay_ms(30);
			if(PINB&(1<<7)){ //second check due to switch de-bouncing
				count=countUp;
				PORTD&=~(1<<5)&~(1<<0);//count down led(yellow) and buzzer are off
				PORTD|=(1<<4);//count up led is on (red led)
			}
		}
	}
 }

