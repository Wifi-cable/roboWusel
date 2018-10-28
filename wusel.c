#include <avr/io.h>	//avr input output
#include <avr/interrupt.h> 	// intterupts instead of delay
#include <util/delay.h>

#define setBit(port, pin) port |= (1<<bit)
#define flipBit(port, pin) port ^= (1<< pin)
#define true 1
#define leftwisker (PIND &(1 << PD6))
#define rightwisker (PIND &(1 << PD4))

#define rightEye PC7
#define leftEye PD5
 /*int sinArray[] = {3000, 3105, 3208, 3309, 3407, 3500, 3588, 3669, 3743, 3809, 3866, 3914, 3951, 3978, 3995, 4000, 3995, 3978, 3951,
	  3914, 3866, 3809, 3743, 3669, 3588, 3500, 3407, 3309, 3208, 3105, 3000, 2895, 2792, 2691, 2593,2500, 2412, 2331, 2257,
		2191, 2134, 2086, 2049, 2022, 2005, 2000, 2005, 2022, 2049, 2086, 2134, 2191, 2257, 2331, 2412, 2500, 2593, 2691, 2792, 2895};
 int cosinArray[] ={4000, 3995, 3978, 3951, 3914, 3866, 3809, 3743, 3669, 3588, 3500, 3407, 3309, 3208, 3105, 3000, 2895, 2792, 2691,
	  2593, 2500, 2412, 2331, 2257, 2191, 2134, 2086, 2049, 2022, 2005, 2000, 2005, 2022, 2049, 2086, 2134, 2191, 2257, 2331, 2412, 2500,
		2593, 2691, 2792, 2895, 3000, 3105, 3208, 3309, 3407, 3500, 3588, 3669, 3743, 3809, 3866, 3914, 3951, 3978, 3995};
		*/
		int sinArray [] = {500, 517, 535, 552, 570, 587, 604, 621, 638, 655, 671, 687, 703, 719, 735, 750, 765, 780,
			 794, 808, 821, 835, 847, 860, 872, 883, 894, 905, 915, 924, 933, 941, 949, 957, 964, 970, 976, 981, 985,
			  989, 992, 995, 997, 999, 1000, 1000, 1000, 999, 997, 995, 992, 989, 985, 981, 976, 970, 964, 957, 949,
				941, 933, 924, 915, 905, 894, 883, 872, 860, 847, 835, 821, 808, 794, 780, 765, 750, 735, 719, 703, 687,
			  671, 655, 638, 621, 604, 587, 570, 552, 535, 517, 500, 483, 465, 448, 430, 413, 396, 379, 362, 345,
			  329, 313, 297, 281, 265, 250, 235, 220, 206, 192, 179, 165, 153, 140, 128, 117, 106, 95, 85, 76, 67,
				59, 51, 43, 36, 30, 24, 19, 15, 11, 8, 5, 3, 1, 0, 0, 0, 1, 3, 5, 8, 11, 15, 19, 24, 30, 36, 43, 51, 59,
				67, 76, 85, 95, 106, 117, 128, 140, 153, 165, 179, 192, 206, 220, 235, 250, 265, 281, 297, 313, 329, 345,
				362, 379, 396, 413, 430, 448, 465, 483};

		int cosinArray[] ={1000, 1000, 999, 997, 995, 992, 989, 985, 981, 976, 970, 964, 957, 949, 941, 933, 924, 915, 905,
			 894, 883, 872, 860, 847, 835, 821, 808, 794, 780, 765, 750, 735, 719, 703, 687, 671, 655, 638, 621, 604, 587,
			 570, 552, 535, 517, 500, 483, 465, 448, 430, 413, 396, 379, 362, 345, 329, 313, 297, 281, 265, 250, 235, 220, 206,
			 192, 179, 165, 153, 140, 128, 117, 106, 95, 85, 76, 67, 59, 51, 43, 36, 30, 24, 19, 15, 11, 8, 5, 3, 1, 0, 0, 0,
			 1, 3, 5, 8, 11, 15, 19, 24, 30, 36, 43, 51, 59, 67, 76, 85, 95, 106, 117, 128, 140, 153, 165, 179, 192, 206, 220,
			 235, 250, 265, 281, 297, 313, 329, 345, 362, 379, 396, 413, 430, 448, 465, 483, 500, 517, 535, 552, 570, 587, 604,
			  621, 638, 655, 671, 687, 703, 719, 735, 750, 765, 780, 794, 808, 821, 835, 847, 860, 872, 883, 894, 905, 915,
				924, 933, 941, 949, 957, 964, 970, 976, 981, 985, 989, 992, 995, 997, 999, 1000};

struct legg{
	uint16_t zeroPosition;
	int finalPosition;
	int singleStep;
	};
volatile uint8_t flag =0;
int mode;
volatile int roboWalk;
volatile uint8_t delay = 0;
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
void walk(int far);
void walk1(int step);
void walk2(int step);
void walk3(int step);
void sleep(int time);
void rightWalk(int steps);
void timerWalk( int step, int speed);
void stop();


void main(void){

	int leftWisker;
	int rightWisker;
	int distanceSensor;
	int infraredSensor;
	int topLightSensor;
	int bottomLightSensor;

//default positions or zeroPosition is mirrored
	rightFront.zeroPosition = 4000;
	leftFront.zeroPosition = 2000;
	rightBack.zeroPosition = 4000;
	leftBack.zeroPosition = 2000;



	timerInit();
 	spikesSetup();
	sei();

	wakeUp();
	_delay_ms(500);
	hello(3);

	while(1){
		timerWalk( 1, 1);

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
		setLegPos(&rightFront, -1, 5);// leg down
		setLegPos(&leftFront, -1, 170);	// leg down
		_delay_ms(2000);
		setLegPos(&rightBack, -1, 5); // leg down
		setLegPos(&leftBack, -1, 170);// leg down
		_delay_ms(250);
		PORTC = (1 << rightEye);
		PORTD = (1 << leftEye);
		blinkTimes(10);
		PORTC = (0 << rightEye);
		PORTD = (0 << leftEye);
	}

	void sleep(int time){
		int cnt;

		setLegPos(&rightBack, 2, 140);// leg up
		setLegPos(&leftBack, 2,  20);	//up
		_delay_ms(1000);
		setLegPos(&rightFront, 2, 140);	// up
		setLegPos(&leftFront, 2, 20);	// up
		_delay_ms(300);
		PORTC = (1 << rightEye);
		PORTD = (1 << leftEye);
		for(cnt = 0; cnt<time; cnt++){
		_delay_ms(1);
		}
	}

	void walk(int step){
		while (step){

	setLegPos(&rightFront, 3, 120);	// up
	_delay_ms(50);
	setLegPos(&rightBack, 4, 140);// leg up
	_delay_ms(150);
	setLegPos(&rightFront, -3, 20);// leg down
	_delay_ms(50);
	setLegPos(&rightBack, -4,0); // leg down
	//right side moved now left
	_delay_ms(150);
	setLegPos(&leftFront, 3, 60);	// up
	_delay_ms(50);
	setLegPos(&leftBack, 4,  80);	//up
	_delay_ms(150);
	setLegPos(&leftFront, -3, 160);	// leg down
	_delay_ms(50);
	setLegPos(&leftBack, -4, 180);// leg down
	_delay_ms(150);

	step--;
		}
	}
	void walk2(int step){	// sucks
		while (step){


	setLegPos(&leftFront, 7, 40);	// up
	_delay_ms(100);
	setLegPos(&rightBack, 7, 140);// leg up
	_delay_ms(100);
	setLegPos(&leftFront, -7, 75);	// leg down
	_delay_ms(100);
	setLegPos(&rightBack, -7,5); // leg down
	_delay_ms(100);
	setLegPos(&rightFront, 7, 140);	// up
	_delay_ms(100);
	setLegPos(&leftBack, 7, 40);	//up
	_delay_ms(100);	//next step
	setLegPos(&rightFront, -7, 5);// leg down
	_delay_ms(100);
	setLegPos(&leftBack, -7, 175);// leg down
	_delay_ms(100);

	step--;
		}
	}
	void walk3(int step){
		while (step){


	setLegPos(&leftFront, 5, 40);	// up
	_delay_ms(150);
	setLegPos(&rightBack, 5, 120);// leg up
	_delay_ms(150);
	setLegPos(&leftFront, -5, 180);	// leg down
	_delay_ms(150);
	setLegPos(&rightBack, -5, 0); // leg down
	_delay_ms(150);
	setLegPos(&rightFront, 5, 140);	// up
	_delay_ms(150);
	setLegPos(&leftBack, 5,  60);	//up
	_delay_ms(150);	//next step
	setLegPos(&rightFront, -5, 0);// leg down
	_delay_ms(150);
	setLegPos(&leftBack, -5, 180);// leg down
	_delay_ms(150);

	step--;
		}
	}
	void rightWalk(int steps){	// does NOT work this way
		while (steps){
			setLegPos(&rightFront, 4, 160);	// up
			_delay_ms(60);
			setLegPos(&rightBack, 4, 160);// leg up
			_delay_ms(160);
			setLegPos(&rightFront, -4, 0);// leg down
			_delay_ms(60);
			setLegPos(&rightBack, -4,0); // leg down
			//right side moved now left
			_delay_ms(160);
			setLegPos(&leftFront, 3, 60);	// up
			_delay_ms(50);
			setLegPos(&leftBack, 3,  60);	//up
			_delay_ms(150);
			setLegPos(&leftFront, -3, 160);	// leg down
			_delay_ms(50);
			setLegPos(&leftBack, -3, 180);// leg down
			_delay_ms(150);
	steps--;
		}
		return;
	}
	/* make a different walk method that  uses both legs as -- one line,  than move them continously.  but how to move all 4 at once?
	 * do math to set them all, do not set per set per reglular set leg method.  have delay as a parameter. */


	void hello(int times){
		int counter = 0;
		while(counter < times){

			setLegPos(&rightFront, -5, 20);
			_delay_ms(250);
			setLegPos(&rightFront, 5, 180);
			_delay_ms(250);
			counter++;
		}
		setLegPos(&rightFront, -3, 10);
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
	//delay = 1;	//too fast
		if (leftwisker|| rightwisker){
			flag = 1;
		}
		// 4000 to 2000 (invers)
		if((rightFront.finalPosition>=0)&&(rightFront.finalPosition<=2000)){// if not out of rage

			if(rightFront.singleStep>0)	{//move up
				if(rightFront.finalPosition > (currentPos1 - 2000)){
					OCR1A =  OCR1A + rightFront.singleStep;
				}
			}

			else{// move up
				if(rightFront.finalPosition < (currentPos1 - 2000)){
					OCR1A =  OCR1A + rightFront.singleStep;
				}
			}
		}
		//repeat different leg. added instead of subtract, because the legg is not mirrored
		if((leftFront.finalPosition>=0)&&(leftFront.finalPosition<=2000)){// if not out of rage

			if(leftFront.singleStep>0)	{//move down
				if(leftFront.finalPosition < (currentPos2 - leftFront.zeroPosition)){
					OCR1B =  OCR1B - leftFront.singleStep;
				}
		}

		else{// move up
			if(leftFront.finalPosition >
				(currentPos2  - leftFront.zeroPosition)){
			OCR1B =  OCR1B - leftFront.singleStep;
			}
		}
	}
		//mirrored leg
		if((rightBack.finalPosition>=0)&&(rightBack.finalPosition<=2000)){// if not out of rage

						if(rightBack.singleStep>0)	{//move down
							if(rightBack.finalPosition > (currentPos3- 2000)){
								OCR1C =  OCR1C +rightBack.singleStep;
							}
					}

					else{// move up
						if(rightBack.finalPosition < (currentPos3- 2000)){
						OCR1C =  OCR1C + rightBack.singleStep;
						}
					}
				}

	if((leftBack.finalPosition>=0)&&(leftBack.finalPosition<=2000)){// if not out of rage

			if(leftBack.singleStep>0)	{//move leg down
				if(leftBack.finalPosition < (currentPos4 - leftBack.zeroPosition)){
					OCR3A =  OCR3A -leftBack.singleStep;
				}
		}

		else{// move leg up
			if(leftBack.finalPosition > (currentPos4 - leftBack.zeroPosition)){
			OCR3A =  OCR3A - leftBack.singleStep;
			}
		}
	}
}	//end %10
if((ms250 % 25) ==0){
	delay=1;
}

ms250++;
if(ms250 >=250){
	ms500++;
	ms250=0;
}
}

	void walk1(int step){
		while (step){

	setLegPos(&leftBack, 5,  40);	//up
	_delay_ms(150);
	setLegPos(&rightFront, 5, 140);	// up
	_delay_ms(150);
	setLegPos(&leftBack, -5, 175);// leg down
	_delay_ms(150);
	setLegPos(&rightFront, -5, 5);// leg down
	_delay_ms(150);	//next step

	setLegPos(&rightBack, 5, 140);// leg up
	_delay_ms(150);
	setLegPos(&leftFront, 5, 40);	// up
	_delay_ms(150);
	setLegPos(&rightBack, -5, 5); // leg down
	_delay_ms(150);
	setLegPos(&leftFront, -5, 175);	// leg down
	_delay_ms(150);

	step--;
		}
	}

/* this method uses the huge global arrays that translate sin wave and cosin wave into integer large
integer values and a timer to walk forward or backward.
*/
	void timerWalk( int forward, int speed){

		static uint8_t sinPos; // arraypointer for sinus array
		static uint8_t cosPos; // arraypointer for cosinArray
		if(delay){
			if(forward){// move forward
				OCR1A = (sinArray[sinPos]) + 2000;
				OCR1B = (sinArray[sinPos]) + 3000;
				OCR1C = (cosinArray[cosPos]) + 2000;
				OCR3A = (cosinArray[cosPos]) + 3000;

				sinPos = (sinPos -speed);
				if (sinPos>181) {
					sinPos = 179;
				}

				cosPos = (cosPos -speed);
				if (cosPos>181) {
					cosPos = 179;
				}
			}

			else{	// backwards
				OCR1A= (sinArray[sinPos]) + 2000;
				OCR1B= (sinArray[sinPos]) + 3000;
				OCR1C= cosinArray[cosPos] + 2000;
				OCR3A= cosinArray[cosPos] + 3000;

				sinPos= (sinPos +speed)%180;
				cosPos= (cosPos +speed)%180;

			}
			//move
			delay = 0;
		}
	}// end timerWalk


	void stop(){
		if (flag){
			blinkTimes(4);
			walk3(5);
			flag = 0;
		}
		else{
			walk(1);
		}
	}
	/*
	 * let ISR check  every 10 ms if a button was pressed.
	 * if so, set a flag.
	 * after each step check for flag
	 * if flag, walk backwards set flag to zero.
	 * */
