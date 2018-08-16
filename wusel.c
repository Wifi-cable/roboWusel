#include <avr/io.h>	//avr input output 
#include <avr/interrupt.h> 	// intterupts instead of delay
#include <util/delay.h>

#define setBit(port, pin) port |= (1<<bit)
#define flipBit(port, pin) port ^= (1<< pin)

#define rightEye PC7
#define leftEye PD5

#define rightFrontLeg   OCR1A
#define leftFrontLeg	OCR1B
#define rightBackLeg 	OCR1C
#define leftBackLeg     OCR3A

struct legg{
	int finalPosition;
	int singleStep;
	uint8_t legID; //1,2,3,4
	};

int mode;
uint8_t delay;
void timerInit();
void blinkTimes(int time);
void setup();
void hello(int times);
void spikesSetup();
void wakeUp();
void setLegPos(struct legg* thatLeg , int oneStep, int finalPos);
void walk();


void main(void){

	int leftWisker;
	int rightWisker;
	int distanceSensor;
	int infraredSensor;
	int topLightSensor;
	int bottomLightSensor;
	

	// as seen from robot
	struct legg rightFront;	//yellow servo
	struct legg leftFront;
	struct legg rightBack;
	struct legg leftBack; 
	
	rightFront.angel = 4000;
	rightFront.legID = 1; 
	
	leftFront.angel = 2000;
	leftFront.legID =2;
	
	rightBack.angel = 4000;
	rightBack.legID = 3;
	
	leftBack.angel = 2000;
	leftBack.legID = 4;
	
	sei();
	timerInit();
 	spikesSetup();
 
	while(1){
	blinkTimes(5);
	_delay_ms(500);

	}

}


void spikesSetup(){
			DDRB = (1<<DDB7) | 	(1<<DDB6) |	 (1<<DDB5) | 	(1<< DDB4); // servo motor output enabled
			//DDC6 = backLeftServo
			//DDC7 = LEDright
			DDRC = (1<<DDC7) | (1<<DDC6); 
			//DDD5 = LEDleft
			DDRD = (1<<DDD5);	
					
        TCCR1A = ((1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<COM1C1) | (0<<COM1C0) | (1<<WGM11) | (0<<WGM10));
        TCCR1B = ((0<<ICNC1) | (0<<ICES1) | (0<<PIN5) | (1<<WGM13) | (1<<WGM12)| (0<<CS12) | (1<<CS11) | (0<<CS10));
     /*   
        rightFrontLeg = 4000;
        leftFrontLeg =2000;
        rightBackLeg =4000;	*/
        
        ICR1   = 40000; // Periode 	
        
        TCCR3A = ((1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<COM1C1) | (0<<COM1C0) | (1<<WGM11) | (0<<WGM10));
        TCCR3B = ((0<<ICNC1) | (0<<ICES1) | (0<<PIN5) | (1<<WGM13) | (1<<WGM12)| (0<<CS12) | (1<<CS11) | (0<<CS10));
        
        leftBackLeg = 2000;
        
        ICR3   = 40000; // Periode 	
	}

void timerInit(){

	TCCR0B = (1<<CS01) | (1<<CS00) | (1<<WGM02);	//prescaler 64
	TIMSK0 |= (1<<TOIE0);	//timer counter over flow is enabled
	TCCR0A |= (1<<WGM01)| (1<<WGM00);
	OCR0A = 249; 
	TCNT0 = 0;	//timer starts at zero
	PORTD |= (1 << leftEye);

}
/*setter for the legs, that takes a position to increment the leg position,
 * and a goal or final position the leg should be in. this makes up the position and speed of movement.
 * if the leg should move backward set a negative final position and increment.
 * */
	setLegPos(struct legg* thatLeg , int oneStep, int finalPos){
		thatLeg-> finalPosition = (finalPos*11);
		thatLeg-> singleStep = (oneStep*11);
	}
	
	
	void walk(){
	//set legs to walk straight 1,4,2,3 ? 
	}

	void hello(int times){
		int counter = 0;
		while(counter < times){
			OCR1A = 2000; 
			_delay_ms(250);
			OCR1A = 2000+ 1000; 
			_delay_ms(250);			
			counter++;
		}
	}
	
	void blinkTimes(int time){
	int counter = 0;
	while( counter<time){
		_delay_ms(250);
		PORTC ^= (1 << rightEye);
		PORTD ^= (1 << leftEye);
		counter++;
	}
	}





		/*interrupt service routine that makes the eyes blink twice per second. 
		(16 000 000 /1024)/ 256 = 60 hz. 60*30 so they togle every half second */
	ISR(TIMER0_OVF_vect) {
	static uint8_t ms250;
	static uint8_t ms500;
	uint8_t ms10;
	static uint8_t on = 0; 
	
	ms250++;
	if((ms250 % 100)== 0 ){
		ms10++;

	}
	int zeroPos;//leg is high in the air
	//put in ISR
	if((legId == 1)||(legId == 3) ){	//servos and therefore leggs are mirrored on robot
		zeroPos = 4000;
		ocr= zeroPos - (singleStep * 11);
	}
	else{
		zeroPos = 2000;
		ocr= zeroPos + (singleStep * 11);
	}

	/*if(ms500 >= speed){	// does not work, because it would set the legs many times afer a delay. must resett 
	the counter after each time. use differnet counter variable?
		set servos
	}	*/
	if(ms250 >=250){
		ms500++;
		ms250=0; 
		
	}
	


	}
