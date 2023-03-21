
/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "main.h"

volatile int gMinuterieExpiree = 0;


void init(){
    cli();
    DDRD = DDR_OUT;
    DDRB = DDR_OUT;
    DDRA = DDR_OUT;
    PORTD = 0 << PD6; //mise a zero du pin D6 pour le buzzer
    sei();
}

//interruption pour la fonction attendre de la librairie
ISR(TIMER1_COMPA_vect){
    gMinuterieExpiree = 1;
}

//constructeur
GestionSon::GestionSon(){
    ajusterSon(82); // soundOff
}

int GestionSon::calculerTop(int index){
    return (int)round(F_CPU / (freq[index - 45] * DIV_CLOCK)); //calcul pour la fréquence
}

void GestionSon::ajusterSon(int a){

    int top = calculerTop(a);
   
    OCR2A = top;
    OCR2B = top;

    TCCR2A = (1 << WGM21 | 0 << WGM20 | 0 << WGM22 | 0 << COM2A1 | 1 << COM2A0| 1 << COM2B1 | 0 << COM2B0);

    TCCR2B = (1 << CS22 | 1 << CS20); // clock
}

void GestionSon::stopSon(){
    ajusterSon(82);
}


bool Programme::estFini(){
    if(index_ == int(nOctets_)){
        return true;
    }
    return false;
}

//constructeur du robot
Robot::Robot(volatile int* v){
    gMinuterieExpiree_ = v;
    changerCouleur(EtatLed::OFF);
    sequenceDebut();
}

//séquence arbitraire joué lors de l'allumage du robot
void Robot::sequenceDebut(){
    ajusterSon(64);
    changerCouleur(EtatLed::ROUGE);
    attendre(1000);
    ajusterSon(56);
    changerCouleur(EtatLed::VERT);
    attendre(1000);
    stopSon();
    changerCouleur(EtatLed::OFF);
    attendre(10000);
}

void Robot::lireNombreOctets(){
    uint8_t nOctets1;
    uint8_t nOctets2;

    lecture(getIndexpp(), &nOctets1);
    lecture(getIndexpp(), &nOctets2);

    nOctets_ = (((uint16_t)nOctets1 << 8) | nOctets2) - 2; //calcul du nombre d'octets
    DEBUG_PRINT(nOctets_);
}

int main(){

    init();

    Robot robot = Robot(&gMinuterieExpiree);

    robot.lireNombreOctets();

    int debut_boucle = -1;
    int boucle_index = 0;

    while(!robot.Programme::estFini())
    {
        uint8_t opcode;
        uint8_t arg;

        robot.lecture(robot.getIndexpp(), &opcode);

        if(opcode != 0xFF){
            robot.lecture(robot.getIndexpp(), &arg);
        }

        if(opcode == 0x01){
            robot.setActif(true);
        }

        if(!robot.getActif()){
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
                robot.attendre(MS25); //attendre 25ms
            }
            break;

        case 0x44:
            DEBUG_PRINT("ALLUMER LED");
            DEBUG_PRINT(arg);

            if (arg == 1){
                robot.changerCouleur(EtatLed::VERT);
            }
            if (arg == 2){
                robot.changerCouleur(EtatLed::ROUGE);
            }
            break;
        
        case 0x45:
            DEBUG_PRINT("ETEINDRE LED");
            robot.changerCouleur(EtatLed::OFF);
            break;
        
        case 0x48:
            DEBUG_PRINT("JOUER SON");
            DEBUG_PRINT(arg);
            robot.ajusterSon(arg);
            break;

        case 0x09:
            DEBUG_PRINT("STOP SON");
            robot.stopSon();
            break;

        case 0x60:
            DEBUG_PRINT("STOP MOTEUR");
            robot.ajustementPWM(0,0);
            break;
        
        case 0x61:
            DEBUG_PRINT("STOP MOTEUR");
            robot.ajustementPWM(0,0);
            break;

        case 0x62:
            DEBUG_PRINT("AVANCER");
            DEBUG_PRINT(arg);
            robot.ajustementPWM(arg, arg);
            robot.changerDirection(Direction::AVANT);
            break;

        case 0x63:
            DEBUG_PRINT("RECULER");
            DEBUG_PRINT(arg);
            robot.ajustementPWM(arg, arg);
            robot.changerDirection(Direction::ARRIERE);
            break;

        case 0x64:
            DEBUG_PRINT("TOURNER DROITE");
            robot.ajustementPWM(250,250);
            robot.changerDirection(Direction::DROITE);
            robot.attendre(TEMPS_90DEG);
            robot.ajustementPWM(0,0);
            break;

        case 0x65:
            DEBUG_PRINT("TOURNER GAUCHE");
            robot.ajustementPWM(250,250);
            robot.changerDirection(Direction::GAUCHE);
            robot.attendre(TEMPS_90DEG);
            robot.ajustementPWM(0,0);
            break;

        case 0xC0:
            DEBUG_PRINT("DEBUT BOUCLE");
            DEBUG_PRINT(arg);
            debut_boucle = robot.getIndex();
            DEBUG_PRINT(robot.getIndex());
            boucle_index = arg;
            break;

        case 0xC1:
            DEBUG_PRINT("FIN BOUCLE");
            if(boucle_index > 1){
                robot.setIndex(debut_boucle);
            }
            boucle_index--;
            
            break;
        
        case 0xFF:
            DEBUG_PRINT("FIN");
            robot.setActif(false);
            break;
        
        }
    }
    

    return 0;
}