#include <avr/io.h>	//avr input output 
#include <avr/interrupt.h> 	// intterupts instead of delay
#include <delay.h>


void blink();

void main(void){
	DDRB = (1<<DDB7) | (1<<DDB6) | (1<<DDB5) ; // 11100000  servo motor output enabled
	DDRC = (1<<DDC7) | (1<<DDC6); //servo and one LED
	DDRD = (1<<DDD5);	// set LED as output
	
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
	
	while(1){
	
	
	}

}
void blink(){
	/* turn the led lights on and off. test to see if the bits are enabled
	 and the flashing of the controller works*/
}
