/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: Définition de l'ensemble des classes de la librairie pour le robot du cours INF1900 (voir dossier pour plus de détail)
 */

#include "librairie.h"

//constructeur du robot
Robot::Robot(volatile int* v){
    timer.setVolatile(v);
    led.changerCouleur(EtatLed::OFF);
    sequenceDebut();
    rs232.initialisationUART();
}

//séquence arbitraire joué lors de l'allumage du robot
void Robot::sequenceDebut(){
    son.ajusterSon(64);
    led.changerCouleur(EtatLed::ROUGE);
    timer.attendre(1000);
    son.ajusterSon(56);
    led.changerCouleur(EtatLed::VERT);
    timer.attendre(1000);
    son.stopSon();
    led.changerCouleur(EtatLed::OFF);
    timer.attendre(10000);
}


