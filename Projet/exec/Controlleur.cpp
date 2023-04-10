/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Controlleur.h"

Controlleur::Controlleur(Robot* r){
    robot_ = r;
}

void Controlleur::virerDroite(){
    robot_->moteur.changerDirection(Direction::DROITE);
    robot_->moteur.ajustementPWM(255,255);
    _delay_ms(10);
    robot_->moteur.changerDirection(Direction::AVANT);
    _delay_ms(50);
    robot_->moteur.changerDirection(Direction::DROITE);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(25);
}

void Controlleur::virerGauche(){
    robot_->moteur.changerDirection(Direction::GAUCHE);
    robot_->moteur.ajustementPWM(255,255);
    _delay_ms(20);
    robot_->moteur.changerDirection(Direction::AVANT);
    _delay_ms(50);
    robot_->moteur.changerDirection(Direction::GAUCHE);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(25);
}

void Controlleur::suivre(int tour_restant_, int distance_active, int distance_stop){

    int tour_restant = tour_restant_;
    bool trigger = false;
    bool estDroite = true;

    while(true){
        int distance = robot_->ir.distanceInch();
        DEBUG_PRINT(distance);

        if(estDroite){
            virerDroite();
        }
        else{
            virerGauche();
        }

        if(distance < distance_active){
            trigger = true;
        }

        if(trigger){
            DEBUG_PRINT("HIT");
            tour_restant--;
        }

        if(tour_restant < 0){
            trigger = false;
            tour_restant = tour_restant_;
            if(robot_->moteur.getDirection() == Direction::DROITE){
                estDroite = false;
            }
            else{
                estDroite = true;
            }
        }

        if(distance < distance_stop){
            while(true);
        }

    }

}

void Controlleur::tournerDroite(){
    robot_->moteur.changerDirection(Direction::DROITE);
    robot_->moteur.ajustementPWM(255,255);
    _delay_ms(PERIODE_TOURNER);
    robot_->moteur.changerDirection(Direction::AVANT);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(PERIODE_TOURNER);
}

void Controlleur::tournerGauche(){
    robot_->moteur.changerDirection(Direction::GAUCHE);
    robot_->moteur.ajustementPWM(255,255);
    _delay_ms(PERIODE_TOURNER + DELTA_TOURNER);
    robot_->moteur.changerDirection(Direction::AVANT);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(PERIODE_TOURNER + DELTA_TOURNER);
}

void Controlleur::demarrer(){
    etat_ = EtatRobot::INIT;
    bouton_ = TypeBouton::AUCUN;

    while(true){

        switch (etat_)
        {
        case EtatRobot::INIT:
            DEBUG_PRINT("INIT");
            _delay_ms(50);
            break;

        case EtatRobot::DETECTION:
            DEBUG_PRINT("DETECTION");
            detecter();
            break;

        case EtatRobot::TRANSMISSION:
            DEBUG_PRINT("TRANSMISSION");
            transmettre();
            break;
        
        }

    }


}