/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi
 * Description: Allume une led à la suite de la séquence de 3 appuis sur le boutton
*/

#define F_CPU 8000000UL // 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <librairie.h>


#define DDR_IN 0x00
#define DDR_OUT 0xff

char mots[46] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";

enum Etat
{
    TRANSMETTRE = 0,
    ROULER = 1
};

volatile Etat etatPresent;

volatile int gMinuterieExpiree = 0;

//ici car besoin de l'utiliser dans l'ISR du bouton
Bouton b;

void init()
{
    DDRD = DDR_IN;
    DDRB = DDR_OUT;
    DDRA = DDR_OUT;

    etatPresent = Etat::TRANSMETTRE;

}

//Interuption du bouton, si il est actif donc front montant changer etatPresent
ISR (INT0_vect) {
    switch (etatPresent)
    {
    case Etat::TRANSMETTRE:
        if(b.estActif()){
            etatPresent = Etat::ROULER;
        }
        break;
    
    case Etat::ROULER:
        if(b.estActif()){
            //le changement de etatPresent sera effectif apres la sequence des moteurs
            etatPresent = Etat::TRANSMETTRE;
        }
        break;
    }
}

//Interuption du timer1
ISR (TIMER1_COMPA_vect) {

    gMinuterieExpiree = 1;

}


int main()
{
    init();

    //Timer1 qui va permettre d'appeler la fonction attendre()
    Timer1 t1 = Timer1(&gMinuterieExpiree); //permet d'attendre en passant une variable en parametre, ce qui est impossible avec _delay_ms()
    int dureeAttendre = 5; 
    char c[] = "ekip";

    DEBUG_PRINT((c));
    DEBUG_PRINT((dureeAttendre));
    
    //Instance des moteurs du robot avec Timer0 + init() (PIN B3 B4)
    Moteur motr;

    //Instance de la classe Memoire
    Memoire24CXXX m;

    //init() du bouton (PIN D2)
    b.init();

    //Instance de la classe pour transmission RS232
    RS232 rs;
    rs.initialisationUART();

    //Instance de Led (PIN A0 A1)
    Led l = Led(EtatLed::OFF);
    
    for(int i = 0; i < 46; i++){
        m.ecriture(i, mots[i]);
        t1.attendre(dureeAttendre);
    }

    while(true){
        switch (etatPresent)
        {
        case Etat::ROULER:
            l.changerCouleur(EtatLed::VERT);
            motr.ajustementPWM(250,250);
            motr.changerDOCR0Airection(Direction::AVANT);
            t1.attendre(30000);

            motr.ajustementPWM(0,0);
            l.changerCouleurAmbre(t1, 5, 2, 1000);

            l.changerCouleur(EtatLed::ROUGE);
            motr.ajustemOCR0AntPWM(0,0);
            l.changerCouleur(EtatLed::OFF);

            for (int i = 0; i < 46; i++)
            {
                uint8_t temp;
                m.lecture(i, &temp);
                //rs.transmissionUART(temp);
                t1.attendre(dureeAttendre);
                
            }
            //rs.transmissionUART('\n');
            t1.attendre(dureeAttendre);
            break;
        }
    }
   
    return 0;
}
