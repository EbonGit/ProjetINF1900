
#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <librairie.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff

volatile int gMinuterieExpiree = 0;


void init(){
    cli();
    DDRD = DDR_OUT;
    DDRB = DDR_OUT;
    DDRA = DDR_OUT;
    sei();
}


ISR(TIMER1_COMPA_vect){
    gMinuterieExpiree = 1;
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


int main(){

    init();

    Memoire24CXXX m;

    Led l = Led(EtatLed::OFF);

    Timer1 t = Timer1(&gMinuterieExpiree);

    Moteur motr;

    SoundManager sm;

    int index = -1;

    uint8_t nOctets1;
    uint8_t nOctets2;

    m.lecture(++index, &nOctets1);
    m.lecture(++index, &nOctets2);

    int debut_boucle = -1;
    int boucle_index = 0;

    uint16_t nOctets = (((uint16_t)nOctets1 << 8) | nOctets2) - 2;
    DEBUG_PRINT(nOctets);

    bool actif = false;


    while(index != nOctets)
    {
        uint8_t opcode;
        uint8_t arg;

        m.lecture(++index, &opcode);

        if(opcode != 0xFF){
            m.lecture(++index, &arg);
        }

        if(opcode == 0x01){
            actif = true;
        }

        if(!actif){
            continue;
        }

        switch (opcode)
        {
        case 0x01:
            DEBUG_PRINT("DEBUT");
            break;
        
        case 0x02:
            DEBUG_PRINT("ATTENDRE");
            DEBUG_PRINT(arg);
            for (int i = 0; i < arg; i++)
            {
                t.attendre(195); //attendre 25ms
            }
            break;

        case 0x44:
            DEBUG_PRINT("ALLUMER LED");
            DEBUG_PRINT(arg);

            if (arg == 1){
                l.changerCouleur(EtatLed::VERT);
            }
            if (arg == 2){
                l.changerCouleur(EtatLed::ROUGE);
            }
            break;
        
        case 0x45:
            DEBUG_PRINT("ETEINDRE LED");
            l.changerCouleur(EtatLed::OFF);
            break;
        
        case 0x48:
            DEBUG_PRINT("JOUER SON");
            DEBUG_PRINT(arg);
            sm.ajusterSon(arg);
            break;

        case 0x09:
            DEBUG_PRINT("STOP SON");
            sm.stopSon();
            break;

        case 0x60:
            DEBUG_PRINT("STOP MOTEUR");
            motr.ajustementPWM(0,0);
            break;
        
        case 0x61:
            DEBUG_PRINT("STOP MOTEUR");
            motr.ajustementPWM(0,0);
            break;

        case 0x62:
            DEBUG_PRINT("AVANCER");
            DEBUG_PRINT(arg);
            motr.ajustementPWM(arg, arg);
            motr.changerDirection(Direction::AVANT);
            break;

        case 0x63:
            DEBUG_PRINT("RECULER");
            DEBUG_PRINT(arg);
            motr.ajustementPWM(arg, arg);
            motr.changerDirection(Direction::ARRIERE);
            break;

        case 0x64:
            DEBUG_PRINT("TOURNER DROITE");
            motr.ajustementPWM(150,150);
            motr.changerDirection(Direction::DROITE);
            t.attendre(9000);
            motr.ajustementPWM(0,0);
            break;

        case 0x65:
            DEBUG_PRINT("TOURNER GAUCHE");
            motr.ajustementPWM(150,150);
            motr.changerDirection(Direction::GAUCHE);
            t.attendre(8000);
            motr.ajustementPWM(0,0);
            break;

        case 0xC0:
            DEBUG_PRINT("DEBUT BOUCLE");
            DEBUG_PRINT(arg);
            debut_boucle = index;
            DEBUG_PRINT(index);
            boucle_index = arg;
            break;

        case 0xC1:
            DEBUG_PRINT("FIN BOUCLE");
            if(boucle_index > 1){
                index = debut_boucle;
            }
            boucle_index--;
            
            break;
        
        case 0xFF:
            DEBUG_PRINT("FIN");
            actif = false;
            break;
        
        }
    }
    

    return 0;
}