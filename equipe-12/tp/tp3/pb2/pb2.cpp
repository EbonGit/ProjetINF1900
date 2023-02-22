/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Attenue une DEL progressivement en 3 secondes grace à un signal PWM
 * Broche IO: 
 *  
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>


#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20

#define B 1000 // frequence 1 kHz
#define DUREE 2000.0 // durée du fade = 3s

float vitesses[5]  = {1.0, 0.75, 0.5, 0.25, 0.0};

void init();
void delay(int16_t);
void Pwm(float*);

int main()
{

    init();

    while (true)
    {
        for(float x : vitesses){
            for (int i = 0; i < DUREE; i++)
            {
                float temp = x*B;
                Pwm(&temp);
            }
            
        }
        
    }

    return 0;
}

void init()
{
    DDRD = DDR_IN;
    DDRA = DDR_OUT;
}

void delay(int16_t temps){
    for(int16_t j = 0; j < temps; j++){
        _delay_us(1);
    }
}

void Pwm(float* a){
    PORTA = (1 << PA0);
    delay(*a);
    PORTA = 0x00;
    delay(B-*a);
}
