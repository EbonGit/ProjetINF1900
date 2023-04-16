/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Controlleur.h"

bool rebond(bool valActive, int PIN){
    if(valActive){
        if ((PIND & PIN)) 
        {
            _delay_ms(TEMPS_REBONDS);
            if ((PIND & PIN))
            {
                return true;
            }
        }
    }
    else{
        if (!(PIND & PIN)) 
        {
            _delay_ms(TEMPS_REBONDS);
            if (!(PIND & PIN))
            {
                return true;
            }
        }
    }
    
    return false;
}

ISR (INT0_vect) {
    if(rebond(true, 0x04)){
        bouton = TypeBouton::INTERUPT;
    }
}

ISR (INT1_vect) {
    if(rebond(false, 0x08)){
        bouton = TypeBouton::EXTERNE;
    }
}

Controlleur::Controlleur(Robot* r){
    robot_ = r;
}

void Controlleur::virerDroite(){
    robot_->moteur.changerDirection(Direction::DROITE);
    robot_->moteur.ajustementPWM(255,255);
    _delay_ms(10);
    robot_->moteur.changerDirection(Direction::AVANT);
    _delay_ms(40);
    robot_->moteur.changerDirection(Direction::DROITE);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(10);
}

void Controlleur::virerGauche(){
    robot_->moteur.changerDirection(Direction::GAUCHE);
    robot_->moteur.ajustementPWM(255,255);
    _delay_ms(10);
    robot_->moteur.changerDirection(Direction::AVANT);
    _delay_ms(40);
    robot_->moteur.changerDirection(Direction::GAUCHE);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(10);
}

void Controlleur::suivre(int tour_restant_, int distance_active, int distance_stop){

    int tour_restant = tour_restant_;
    //int perdu_ = 0;
    bool trigger = false;
    bool estDroite = true;

    while(true){
        int distance = (int)robot_->ir.distanceCm();
        //DEBUG_PRINT(distance);

        if(estDroite){
            virerDroite();

        }
        else{
            virerGauche();

        }

        if(distance < distance_active && distance != 0){
            trigger = true;
            DEBUG_PRINT(distance);
        }

        if(trigger){
            DEBUG_PRINT("HIT");
            tour_restant--;
        }

        if(tour_restant < 0){
            trigger = false;
            tour_restant = tour_restant_;
            estDroite = !estDroite;
        }

        if(distance < distance_stop && distance != 0){
            DEBUG_PRINT("PROCHE");
            break;
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
    
    //orientationN_++;
    //updateOrientation();
}

void Controlleur::tournerGauche(){
    robot_->moteur.changerDirection(Direction::GAUCHE);
    robot_->moteur.ajustementPWM(255,255);
    _delay_ms(PERIODE_TOURNER + DELTA_TOURNER);
    robot_->moteur.changerDirection(Direction::AVANT);
    robot_->moteur.ajustementPWM(0,0);
    _delay_ms(PERIODE_TOURNER + DELTA_TOURNER);

    //orientationN_--;
    //updateOrientation();
}

void Controlleur::updateOrientation(){
    if(orientationN_ == (HUITIEME*4 + 1)){
        orientationN_ = (-HUITIEME*4) + 1;
    }
    if(orientationN_ == (-HUITIEME*4)){
        orientationN_ = (HUITIEME*4);
    }
    float normalise = (orientationN_ + (HUITIEME * 4.0f)) / (8.0f * HUITIEME);

    if (normalise > 0.4375f && normalise <= 0.5625f) {
        orientation_ = Boussole::NORD;
        DEBUG_PRINT("NORD");
    }
    else if (normalise >= 0.5625f && normalise <= 0.6875f) {
        orientation_ = Boussole::NORDEST;
        DEBUG_PRINT("NORDEST");
    }
    else if (normalise >= 0.6875f && normalise <= 0.8125f) {
        orientation_ = Boussole::EST;
        DEBUG_PRINT("EST");
    }
    else if (normalise >= 0.8125f && normalise <= 0.9375f) {
        orientation_ = Boussole::SUDEST;
        DEBUG_PRINT("SUDEST");
    }
    else if (normalise >= 0.9375f || normalise <= 0.0625f) {
        orientation_ = Boussole::SUD;
        DEBUG_PRINT("SUD");
    }
    else if (normalise >= 0.0625f && normalise <= 0.1875f) {
        orientation_ = Boussole::SUDOUEST;
        DEBUG_PRINT("SUDOUEST");
    }
    else if (normalise >= 0.1875f && normalise <= 0.3125f) {
        orientation_ = Boussole::OUEST;
        DEBUG_PRINT("OUEST");
    }
    else if (normalise >= 0.3125f && normalise <= 0.4375f) {
        orientation_ = Boussole::NORDOUEST;
        DEBUG_PRINT("NORDOUEST");
    }
    else{
        DEBUG_PRINT("mauvais angle");
    }
    
}

void Controlleur::demarrer(){
    etat_ = EtatRobot::INIT;
    while(true){

        switch (etat_)
        {
        case EtatRobot::INIT:
            
            switch (bouton)
            {
            case TypeBouton::AUCUN:
                DEBUG_PRINT("AUCUN");
                break;
            
            case TypeBouton::INTERUPT:
                DEBUG_PRINT("INTERUPT");
                bouton = TypeBouton::AUCUN;
                etat_ = EtatRobot::DETECTION;
                for (uint8_t i = 0; i < 20; i++)
                {
                    robot_->memoire.ecriture(i, fin);
                    _delay_ms(25);
                }
                break;
            
            case TypeBouton::EXTERNE:
                DEBUG_PRINT("EXTERNE");
                bouton = TypeBouton::AUCUN;
                etat_ = EtatRobot::TRANSMISSION;
                break;
            }
            //_delay_ms(200);

            break;

        case EtatRobot::DETECTION:
            //DEBUG_PRINT("DETECTION");
            detecter();
            break;

        case EtatRobot::TRANSMISSION:
            //DEBUG_PRINT("TRANSMISSION");
            transmettre();
            break;
        
        }

    }


}