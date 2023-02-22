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
#define DUREE 3000.0 // durée du fade = 3s


enum LedState
{
    RED = (1 << PA0),
    GREEN = (1 << PA1),
    OFF = 0x00
};



void init()
{
    DDRD = DDR_IN;
    DDRA = DDR_OUT;
}

bool debounce()
{

    if (PIND & 0x04)
    {
        _delay_ms(DEBOUNCE_TIME);
        if (PIND & 0x04)
        {
            return true;
        }
    }
    return false;
}

void delay(int16_t temps){
    for(int16_t j = 0; j < temps; j++){
        _delay_us(1);
    }
}

void AllumeLedPwm(float a, LedState couleur){
    PORTA = couleur;
    delay(a);
    PORTA = LedState::OFF;
    delay(B-a);
}

void fade(int8_t debut, int8_t fin, LedState couleur){
    float fadeState = debut*B;
    float pas = (fin-debut)/DUREE * B;
    for(int64_t i = 0; i < DUREE; i++){
        AllumeLedPwm(fadeState, couleur);
        fadeState+=pas;
    }
}



int main()
{

    init();

    while (true)
    {
        
        fade(1,0,LedState::RED);

        
    }

    return 0;
}
