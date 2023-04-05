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
    _delay_ms(10);
    robot_->moteur.changerDirection(Direction::AVANT);
    _delay_ms(50);
    robot_->moteur.changerDirection(Direction::GAUCHE);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(25);
}

void Controlleur::suivre(){
    int tour_restant = 3;
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

        if(distance < 35){
            trigger = true;
        }

        if(trigger){
            DEBUG_PRINT("HIT");
            tour_restant--;
        }

        if(tour_restant < 0){
            trigger = false;
            tour_restant = 3;
            if(robot_->moteur.getDirection() == Direction::DROITE){
                estDroite = false;
            }
            else{
                estDroite = true;
            }
        }

        if(distance < 4){
            while(true);
        }

    }

}