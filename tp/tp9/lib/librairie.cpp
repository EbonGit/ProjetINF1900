/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: Définition de l'ensemble des classes de la librairie pour le robot du cours INF1900 (voir dossier pour plus de détail)
 */

#include "librairie.h"


Bouton::Bouton(int masque){
    masque_ = masque;
}

void Bouton::init(){
    cli ();
    EIMSK |= (1 << INT0);
    EICRA |= (1 << INT0);
    sei ();

    peutChangerEtat_ = true;
}

bool Bouton::debounce()
{

    if (PIND & masque_)
    {
        _delay_ms(DEBOUNCE_TIME);
        if (PIND & masque_)
        {
            return true;
        }
    }
    return false;
}

bool Bouton::estActif(){
    if (debounce() && peutChangerEtat_)
    {
        peutChangerEtat_ = false;
        return true;
    }
    if (!(PIND & masque_))
    {
        peutChangerEtat_ = true;
    }
    return false;
}

Timer1::Timer1(volatile int* gMinuterieExpiree){
    gMinuterieExpiree_ = gMinuterieExpiree;
}

void Timer1::partirMinuterie(int duree){
    *gMinuterieExpiree_ = 0;

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0;

    OCR1A = duree;

    TCCR1A = (1 << COM1A0); //00100000

    TCCR1B = (1 << CSN12 | 1 << CS10);

    TCCR1C = 0;

    TIMSK1 = (1 << ICIE1 | 1 << OCIE1A);
}

void Timer1::attendre(int duree){
    partirMinuterie(duree);
    while(*gMinuterieExpiree_ == 0);
}

void RS232::initialisationUART (void)
{
    // 2400 bauds. Nous vous donnons la valeur des deux

    // premiers registres pour vous éviter des complications.

    UBRR0H = 0;

    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0

    UCSR0A = 0;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Format des trames: 8 bits, 1 stop bits, sans parité

    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

}

void RS232::transmissionUART(uint8_t donnee)
{
    while(!(UCSR0A & (1 << UDRE0)));
    donnee_ = donnee;
    UDR0 = donnee_;
}

uint8_t RS232::recevoirUART( void )
{
    while ( !(UCSR0A & (1<<RXC0)) );
    return UDR0;
}


// constructeur: initialisation du convertisseur
can::can()
{
   // ADC mux : reference analogique externe
   //           ajuste a droite
   ADMUX  = ( 0 << REFS1 ) | ( 0 << REFS0 ) | ( 0 << ADLAR ) | ( 0 << MUX4 ) |
            ( 0 << MUX3 ) | ( 0 << MUX2 ) | ( 0 << MUX1) | ( 0 << MUX0 ) ;

   // ADC Status and Control Register A : 
   //  Active le convertisseur mais sans demarrer de conversion pour l'instant.
   //  Pas de declanchement automatique et aucune interruption suivant
   //  la fin d'une convertion. Division de l'horloge par 64 (il faudra
   //  donc 13 cycles du convertisseur * 64 pour terminer une conversion
   //  soit 832 cycles d'horloge soit 104 micro-secondes
   ADCSRA = ( 1 << ADEN ) | ( 0 << ADSC )  | ( 0 << ADATE ) | ( 0 << ADIF ) |
            ( 0 << ADIE ) | ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 0 << ADPS0 ) ;
}

// Destructeur:  Arreter le convertisseur pour sauver sur la consommation.
can::~can()
{
   // rendre le convertisseur inactif.
   ADCSRA = 0 << ADEN ;
}

// Faire une conversion et aller retourner le resultat sur 16 bits
// dont seulement les 10 de poids faibles sont significatifs.
uint16_t
can::lecture( uint8_t pos)
{
   uint16_t adcVal;

   // Garder les bits de ADMUX intacts, sauf les bit permettant 
   // la selection de l'entree
   ADMUX  &=  ~(( 1 << MUX4 ) | ( 1 << MUX3 ) | 
                ( 1 << MUX2 ) | ( 1 << MUX1)  | ( 1 << MUX0 ));

   // selectionner l'entree voulue
   ADMUX |= ((pos & 0x07) << MUX0) ;

   // demarrer la conversion
   ADCSRA |= ( 1 << ADSC );

   // Attendre la fin de la conversion soit 13 cycles du convertisseur.
   while( ! (ADCSRA & ( 1 << ADIF ) ) );

   // important: remettre le bit d'indication de fin de cycle a zero 
   // pour la prochaine conversion ce qui se fait en l'ajustant a un.
   ADCSRA |= (1 << ADIF);

   // Aller chercher le resultat sur 16 bits.
   adcVal =   ADCL ;
   adcVal +=  ADCH << 8;

   // resultat sur 16 bits
   return adcVal;
}

//void Moteur::init(){}

void Moteur::ajustementPWM(int a, int b)
{
    vitesse_a_ = a;
    vitesse_b_ = b;

    //TODO: changer pour Timer0 

    OCR0A = a;

    OCR0B = b;


    // division d'horloge par 8 - implique une fréquence de PWM fixe

    TCCR0A = (1 << WGM0 | 1 << COM0A1 | 0 << COM0A0 | 1 << COM0B1 | 0 << COM0B0);

    TCCR0B = (1 << CS00);

}

void Moteur::changerDirection(Direction d)
{
    direction_ = d;
    PORTB = d;
}

Direction Moteur::getDirection(void)
{
    return direction_;
}

Led::Led(EtatLed l){
    couleur_ = l;
    PORTA = l;
}

void Led::changerCouleur(EtatLed l){
    couleur_ = l;
    PORTA = l;
}

/**
 * Changer la couleur de la led pour un ratio entre vert et rouge.
 *
 * @param t Timer1 pour utiliser realiser le ratio avec attendre().
 * @param t_vert duree pendant lequel la led est verte.
 * @param t_rouge duree pendant lequel la led est rouge.
 * @param n nombre de fois que la boucle tourne (mettre a 1 pour pouvoir continuer a faire de la scrutation dans une boucle).
 */
void Led::changerCouleurAmbre(Timer1 t, int t_vert, int t_rouge, int n){
    for (int i = 0; i < n; i++)
    {
        PORTA = EtatLed::VERT;
        t.attendre(t_vert);

        PORTA = EtatLed::ROUGE;
        t.attendre(t_rouge);
    }   
}

EtatLed Led::recupererCouleur(){
    return couleur_;
}


