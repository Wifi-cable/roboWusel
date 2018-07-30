#include <avr/io.h>	//avr input output 
#include <avr/interrupt.h> 	// intterupts instead of delay
#include <util/delay.h>
//#include <time.h>


void blink();
void blinkTimes(int time);
void setup();
void hello(int times);
void main(void){

	uint8_t prescaler;
	uint8_t devisor;
	float dutyCycle;
	
	int leftWisker;
	int rightWisker;
	int distanceSensor;
	int infraredSensor;
	int topLightSensor;
	int bottomLightSensor;
	int gyro; 
	setup();
	
	while(1){
	// code to runn in loop.
	blinkTimes(50);
	hello(20);
	}

}
void setup(){
	// set up input, output pins 
	
/*DDB7 = backRightServo
	DDB6 = frontLeftServo 
	DDB5 = frontRightServo 
	DDB4 = distanceSensorOn/off */
	DDRB = (1<<DDB7) | 	(1<<DDB6) |	 (1<<DDB5) | 	(1<< DDB4); // servo motor output enabled
	//DDC6 = backLeftServo
	//DDC7 = LEDright
	DDRC = (1<<DDC7) | (1<<DDC6); 
	//DDD5 = LEDleft
	DDRD = (1<<DDD5);	
	
	//servo setup, fast PWM or mode 14 for all hardware PWM pins

	TCCR1A = (1<< COM1A1)| (1<< WGM11); 
	TCCR1B = (1<< WGM12) | (1<<WGM13) ;
	
	TCCR3A = (1<< WGM31) | (1<<COM3A1);
	TCCR3B = (1<< WGM32) | (1<<WGM33) ;
	ICR1 = 19999;
	/*19999 may not be correct, port may also be wrong
	set a comapre register that compares the counter to the value when the controller needs to
	 write a zero in the servo port
	 TODO
	-servo needs 50 herz frequency
	-find the appropriate frequency of controller after the fuses have been set 
	-find prescaler if neccary */

}

void blink(){

/* turn the led lights on and off. test to see if the bits are enabled
	 and the flashing of the controller works*/
	 while(1){ 
	 	_delay_ms(300);
	 	PORTD = (1<< PD5);
	 	PORTC= (1<< PC7);
	 	_delay_ms(200);
	 	PORTC= (0<< PC7);
	 	PORTD= (0<<PD5);
	 }
}
	void hello(int times){
		int counter=0;
		while(counter<times){
			OCR1A= ICR1 - 2000; 
			_delay_ms(250);
			OCR1A= ICR1 + 2000; 
			_delay_ms(250);			
			counter++;
		}
	}
	void blinkTimes(int time){
	int counter;
	for(counter=0; counter<=time; counter++){
		_delay_ms(300);
	 	PORTD = (1<< PD5);
	 	PORTC= (1<< PC7);
	 	_delay_ms(200);
	 	PORTC= (0<< PC7);
	 	PORTD= (0<<PD5);
	
	}
	}
