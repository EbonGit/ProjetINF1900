
#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <librairie.h>
#include <stdio.h>
#include <math.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff

#define VREF 4.3

void InitADC()
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC  
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);     
}
uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F); 
    //single conversion mode
    ADCSRA |= (1<<ADSC);
    // wait until ADC conversion is complete
    while( ADCSRA & (1<<ADSC) );
   return ADC;
}

class SoundManager{
public:
    SoundManager();
    float freq[38] = {110, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.0, 196.0, 207.65,
                    220.0, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.0,
                    415.30, 440, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99,
                    783.99, 830.61, 880.0, 0};

    int longueurMusic = 9;
    // D4 D4 D5 A4 AB4 G4 F4 D4 F4 G4
    int music[10] = {62, 62, 74, 69, 70, 67, 65, 62, 65, 67};

    int calculerTop(int);
    void ajusterSon(int);
    void stopSon();
};

SoundManager::SoundManager(){
    ajusterSon(82); // soundOff
}

int SoundManager::calculerTop(int index){
    return (int)round(F_CPU / freq[index - 45]);
}

void SoundManager::ajusterSon(int a){

    int top = calculerTop(a);
   
    OCR2A = top; //
    OCR2B = top;

    TCCR2A = (1 << WGM21 | 0 << WGM20 | 0 << WGM22 | 0 << COM2A1 | 1 << COM2A0| 1 << COM2B1 | 0 << COM2B0);

    TCCR2B = (1 << CS22 | 1 << CS20); // clock
}

void SoundManager::stopSon(){
    ajusterSon(82);
}

void init(){
    cli();
    DDRD = DDR_OUT;
    DDRB = DDR_OUT;
    DDRA = DDR_IN; //IN/OUT
    DDRC = DDR_IN;
    sei();
    DEBUG_PRINT("debut");
}

float map(int X, float A, float B, float C, float D){
    return (X-A)/(B-A) * (D-C) + C;
}

int main(){

    init();
    InitADC();

    SoundManager sm;

    can c = can();

    Moteur m;
    m.changerDirection(Direction::DROITE);

    int temps_tour_restant = 3;
    bool trigger = false;
    

    while(true){
        uint16_t val_ = (c.lecture(PORTA & 0x00));
        double val = (double)VREF/1024*val_;

        double distance = (29.988 * pow(val, -1.173))/2.54;
        
        DEBUG_PRINT((int)distance);

       
        m.ajustementPWM(255,255);
        _delay_ms(20);
        Direction d = m.getDirection();
        m.changerDirection(Direction::AVANT);
        _delay_ms(20);
        m.changerDirection(d);
        m.ajustementPWM(0,0);
        _delay_ms(50);

        if(distance < 20){
            trigger = true;
            sm.ajusterSon(55);
            _delay_ms(10);
        }

        if(trigger){
            temps_tour_restant--;
        }

        if(temps_tour_restant < 0){
            trigger = false;
            temps_tour_restant = 3;
            if(m.getDirection() == Direction::DROITE){
                m.changerDirection(Direction::GAUCHE);
            }
            else{
                m.changerDirection(Direction::DROITE);
            }
        }

        if(distance < 5){
            while(true);
        }

        sm.stopSon();


    }

    

    return 0;
}