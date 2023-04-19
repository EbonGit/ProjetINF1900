/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */

#include "led.h"


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