#include <avr/io.h>	//avr input output 
#include <avr/interrupt.h> 	// intterupts instead of delay
#include <util/delay.h>

#define setBit(port, pin) port |= (1<<bit)
#define flipBit(port, pin) port ^= (1<< pin)

#define rightEye PC7
#define leftEye PD5

struct legg{
	uint16_t zeroPosition;
	int finalPosition;
	int singleStep;
	uint8_t legID; //1,2,3,4
	};

int mode;
uint8_t delay;
// as seen from robot
struct legg rightFront;	//yellow servo mirrored
struct legg leftFront;
struct legg rightBack;	//mirrored
struct legg leftBack;

void timerInit();
void blinkTimes(int time);
void setup();
void hello(int times);
void spikesSetup();
void wakeUp();
void setLegPos(struct legg* thatLeg , int oneStep, int finalPos);
void walk();
void sleep(int time);

void main(void){

	int leftWisker;
	int rightWisker;
	int distanceSensor;
	int infraredSensor;
	int topLightSensor;
	int bottomLightSensor;
	


	
	rightFront.zeroPosition = 4000;

	
	leftFront.zeroPosition = 2000;

	
	rightBack.zeroPosition = 4000;

	
	leftBack.zeroPosition = 2000;

	
	sei();
	timerInit();
 	spikesSetup();


/*	while(1){
		blinkTimes(2);
		_delay_ms(2000);
		setLegPos(&leftFront, 10, 1);
		_delay_ms(2000);
		setLegPos(&leftFront, -10, 160);
		_delay_ms(2000);
		setLegPos(&leftFront, 10, 1);
		_delay_ms(2000);
		setLegPos(&leftFront, -10, 160);
	}
*/
	while(1){
		wakeUp();

		_delay_ms(1000);
		sleep(2000);


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
        
        ICR1   = 40000; // Periode 	
        
        TCCR3A = ((1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<COM1C1) | (0<<COM1C0) | (1<<WGM11) | (0<<WGM10));
        TCCR3B = ((0<<ICNC1) | (0<<ICES1) | (0<<PIN5) | (1<<WGM13) | (1<<WGM12)| (0<<CS12) | (1<<CS11) | (0<<CS10));
        // put all legs in zero position
      OCR1A = rightFront.zeroPosition;
	  OCR1B	= leftFront.zeroPosition;
	  OCR1C	= rightBack.zeroPosition;
	  OCR3A = leftBack.zeroPosition;

        
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
	void setLegPos(struct legg* thatLeg , int oneStep, int finalPos){
		thatLeg-> finalPosition = (finalPos*11);
		thatLeg-> singleStep = (oneStep*11);
	}
	
	void wakeUp(){
		blinkTimes(3);
		_delay_ms(50);
		setLegPos(&rightFront, 1, 90);
		setLegPos(&leftFront, 1, 90);
		_delay_ms(550);
		setLegPos(&rightBack, 2, 90);
		setLegPos(&leftBack, 2, 90);
		_delay_ms(250);
		PORTC = (1 << rightEye);
		PORTD = (1 << leftEye);
		blinkTimes(6);
		PORTC = (1 << rightEye);
		PORTD = (1 << leftEye);
	}

	void sleep(int time){
		int cnt;
		setLegPos(&rightBack, -2, 0);
		setLegPos(&leftBack, -2,  181);
		_delay_ms(1000);
		setLegPos(&rightFront, -2, 0);
		setLegPos(&leftFront, -2, 181);
		_delay_ms(100);
		PORTC = (0 << rightEye);
		PORTD = (0 << leftEye);
		for(cnt = 0; cnt<time; cnt++){
		_delay_ms(1);
		}

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
	// 1000 Aufrufe / Sec
	static uint8_t ms250;
	static uint8_t ms500;
	uint8_t ms10;
	int currentPos1 = OCR1A;
	int currentPos2 = OCR1B;
	int currentPos3	= OCR1C;
	int currentPos4 = OCR3A;
	

	if((ms250 % 10)== 0 ){

			// 4000 to 2000 (invers)
			if((rightFront.finalPosition>=0)&&(rightFront.finalPosition<=2000)){// if not out of rage

				if(rightFront.singleStep>0)	{//move up
					if(rightFront.finalPosition > (currentPos1 - 2000)){
						OCR1A =  OCR1A + rightFront.singleStep;
					}
				}

				else{// move down
					if(rightFront.finalPosition < (currentPos1 - 2000)){
						OCR1A =  OCR1A + rightFront.singleStep;
					}
				}
			}
			//repeat different leg. added instead of subtract, because the legg is not mirrored
			if((leftFront.finalPosition>=0)&&(leftFront.finalPosition<=2000)){// if not out of rage

				if(leftFront.singleStep>0)	{//move up
					if(leftFront.finalPosition < (currentPos2 - leftFront.zeroPosition)){
						OCR1B =  OCR1B - leftFront.singleStep;
					}
			}

			else{// move down
				if(leftFront.finalPosition > (currentPos2  - leftFront.zeroPosition)){
				OCR1B =  OCR1B - leftFront.singleStep;
				}
			}
		}
			//mirrored leg
			if((rightBack.finalPosition>=0)&&(rightBack.finalPosition<=2000)){// if not out of rage

							if(rightBack.singleStep>0)	{//move up
								if(rightBack.finalPosition > (currentPos3- 2000)){
									OCR1C =  OCR1C +rightBack.singleStep;
								}
						}

						else{// move down
							if(rightBack.finalPosition < (currentPos3- 2000)){
							OCR1C =  OCR1C + rightBack.singleStep;
							}
						}
					}

		if((leftBack.finalPosition>0)&&(leftBack.finalPosition<2000)){// if not out of rage

				if(leftBack.singleStep>0)	{//move up
					if(leftBack.finalPosition < (currentPos4 - leftBack.zeroPosition)){
						OCR3A =  OCR3A -leftBack.singleStep;
					}
			}

			else{// move down
				if(leftBack.finalPosition > (currentPos4 - leftBack.zeroPosition)){
				OCR3A =  OCR3A - leftBack.singleStep;
				}
			}
		}
	}	//end %10

	ms250++;
	if(ms250 >=250){
		ms500++;
		ms250=0; 
		
	}
	
	}
