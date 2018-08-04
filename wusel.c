#include <avr/io.h>	//avr input output 
#include <avr/interrupt.h> 	// intterupts instead of delay
#include <util/delay.h>

#define setBit(port, pin) port |= (1<<bit)
#define flipBit(port, pin) port ^= (1<< pin)

#define rightEye PC7
#define leftEye PD5

#define rightFrontLeg OCR1A
#define leftFrontLeg	OCR1B
#define rightBackLeg 	OCR1C
#define leftBackLeg   OCR3A


void blink();
void blinkTimes(int time);
void setup();
void hello(int times);
void spikesSetup();
void wakeUp();
void openEyes();
void getUp();

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
	
	sei();
	blink();
 	spikesSetup();
 
	while(1){

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

}

void spikesSetup(){
			DDRB = (1<<DDB7) | 	(1<<DDB6) |	 (1<<DDB5) | 	(1<< DDB4); // servo motor output enabled
			//DDC6 = backLeftServo
			//DDC7 = LEDright
			DDRC = (1<<DDC7) | (1<<DDC6); 
			//DDD5 = LEDleft
			DDRD = (1<<DDD5);	
	
					
	       // Timer1 [OK]: Mode 14 Fast PWM 0...ICR1, OC1x -> PWM CTRL
        TCCR1A = ((1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<COM1C1) | (0<<COM1C0) | (1<<WGM11) | (0<<WGM10));
        TCCR1B = ((0<<ICNC1) | (0<<ICES1) | (0<<PIN5) | (1<<WGM13) | (1<<WGM12)| (0<<CS12) | (1<<CS11) | (0<<CS10));
        
        //OCR1A  = 2000;     // SERVO_VR
        rightFrontLeg = 2000;
       // OCR1B  = 2000;     // SERVO_VL
        leftFrontLeg =2000;
       // OCR1C  = 2000;     // SERVO_HR
        rightBackLeg =2000;
        
        ICR1   = 40000; // Periode 	
        
        TCCR3A = ((1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<COM1C1) | (0<<COM1C0) | (1<<WGM11) | (0<<WGM10));
        TCCR3B = ((0<<ICNC1) | (0<<ICES1) | (0<<PIN5) | (1<<WGM13) | (1<<WGM12)| (0<<CS12) | (1<<CS11) | (0<<CS10));
        
       // OCR3A  = 2000;     // SERVO_HL
        leftBackLeg = 2000;
        
        ICR3   = 40000; // Periode 	
	}

void blink(){
/* blink init is timer init*/
	TCCR0B = (1<<CS02) | (1<<CS00);	//prescaler 1024
	TIMSK0 |= (1<<TOIE0);	//timer counter over flow is enabled
	TCNT0 = 0;	//timer starts at zero
	PORTD |= (1 << leftEye);

}
	void hello(int times){
		int counter=0;
		while(counter<times){
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
	/* every time the controller is started or reset
	set servos to zero position, 
	fade in the LED light eyes, 
	blink a few times,
	set front servos to push the front leggs to (90°?) standing position
	set back servos to push the rear up.*/
	void wakeUp(){	
	
	openEyes();
	blink(4);
	getUp();
	
	
	}
	
	void openEyes(){
	uint8_t counter = 0;
	//pwm implementation in order to understand PWM myself
	int idx = 0;
		
		uint8_t high = 0;
		uint8_t low = 60;
		int hiIdx = 0; 
		int loIdx = 0;
		
	while (idx < 20){
	
		hiIdx = 0; 
	  loIdx = 0;
		PORTC |= (1<< rightEye);
		PORTD |= (1<<leftEye); 
		
		while(hiIdx <= high){//how long on?
			hiIdx++;
		}
		PORTC |= (0<< rightEye);
		PORTD |= (0<<leftEye); 
			
		while(loIdx <= low){// how long off
			loIdx++;
		}
		
		if(counter%20==0){
			idx++;
			
			high =+3;
			low =-3;
		}
		counter++;
	}
		}
		void getUp(){
		// slowly set front legs to angle ?
		int counter=0;
		while(counter < 10){
		//2000+ 10; 
		rightFrontLeg= 2000 +(11*2);
		leftFrontLeg= 2000 +(11*2);
		counter++;
		}
		//pause
		// slowly set back legs to? 
		}
		/*interrupt service routine that makes the eyes blink twice per second. 
		(16 000 000 /1024)/ 256 = 60 hz. 60*30 so they togle every half second */
	ISR(TIMER0_OVF_vect) {
	static uint8_t overflow;
	overflow++;
	if(overflow>=30){
		PORTC ^= (1 << rightEye);
		PORTD ^= (1 << leftEye);
		overflow = 0;
		}
	}
