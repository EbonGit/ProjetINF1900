/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: Définition de l'ensemble des classes de la librairie pour le robot du cours INF1900 (voir dossier pour plus de détail)
 */

#include "librairie.h"

//constructeur du robot
Robot::Robot(volatile int* v){
    //gMinuterieExpiree_ = v;
    setVolatile(v);
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


