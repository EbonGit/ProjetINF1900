/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Controlleur.h"

void Controlleur::detecter(){
    switch (etatDetection_)
    {
    case EtatDetection::ATTENDRE_1:
        //DEBUG_PRINT("ATTENDRE_1");
        // robot_->led.changerCouleur(EtatLed::ROUGE);
        // robot_->timer.attendre(10);
        // robot_->led.changerCouleur(EtatLed::VERT);
        // robot_->timer.attendre(5);
        robot_->led.changerCouleurAmbre(robot_->timer, 5, 3, 1);
        switch (bouton)
            {
            case TypeBouton::AUCUN:
                
                break;
            
            case TypeBouton::INTERUPT:
                robot_->led.changerCouleur(EtatLed::OFF);
                DEBUG_PRINT("INTERUPT");
                bouton = TypeBouton::AUCUN;
                etatDetection_ = EtatDetection::HAUT;
                break;
            
            case TypeBouton::EXTERNE:
                robot_->led.changerCouleur(EtatLed::OFF);
                DEBUG_PRINT("EXTERNE");
                bouton = TypeBouton::AUCUN;
                etatDetection_ = EtatDetection::DROIT;
                break;
            }
        break;
    
    case EtatDetection::HAUT:
        DEBUG_PRINT("HAUT");
        orientationN_ = 0 * HUITIEME;
        updateOrientation();
        robot_->led.changerCouleur(EtatLed::VERT);
        _delay_ms(2000);
        robot_->led.changerCouleur(EtatLed::OFF);
        etatDetection_ = EtatDetection::RECHERCHE;
        break;

    case EtatDetection::DROIT:
        DEBUG_PRINT("DROIT");
        orientationN_ = 2 * HUITIEME;
        updateOrientation();
        robot_->led.changerCouleur(EtatLed::ROUGE);
        _delay_ms(2000);
        robot_->led.changerCouleur(EtatLed::OFF);
        etatDetection_ = EtatDetection::RECHERCHE;
        break;

    case EtatDetection::RECHERCHE:
        certitude = 0;
        rechercher();
        updateOrientation();
        //rectification = 8;
        poteauBoussole = orientation_;
        etatDetection_ = EtatDetection::TROUVE;
        break;

    case EtatDetection::TROUVE:
        if(distance_ == 0){
            DEBUG_PRINT("PETIT");
        }
        if(distance_ == 1){
            DEBUG_PRINT("GRAND");
        }
        etatDetection_ = EtatDetection::AVANCER;
        break;
    //TODO si ne trouve pas
    case EtatDetection::NON_TROUVE:
        
        break;

    case EtatDetection::AVANCER:
        suivre(3, RANGE_GRAND, 15);
        etatDetection_ = EtatDetection::PROCHE;
        break;

    case EtatDetection::PROCHE:
        DEBUG_PRINT("PROCHE");
        afficherPoteau();
        enregistrerPoteau();
        etatDetection_ = EtatDetection::ATTENDRE_2;
        break;

    case EtatDetection::ATTENDRE_2:
        switch (bouton)
            {
            case TypeBouton::AUCUN:
                
                break;
            
            case TypeBouton::INTERUPT:
                DEBUG_PRINT("INTERUPT");
                bouton = TypeBouton::AUCUN;
                distance_ = -1;
                etatDetection_ = EtatDetection::HAUT;
                break;
            
            case TypeBouton::EXTERNE:
                DEBUG_PRINT("EXTERNE");
                bouton = TypeBouton::AUCUN;
                break;
            }
        break;
    }
    
}

void Controlleur::rechercher(){
    int remisZero = 0;
    if(orientation_ == Boussole::EST){
        robot_->moteur.changerDirection(Direction::GAUCHE);
        robot_->moteur.ajustementPWM(250,250);
        _delay_ms(100);
        robot_->moteur.ajustementPWM(100,100);
        while(true){
            remisZero++;
            //tournerGauche();
            float d = robot_->ir.distanceCm();
            if(d < RANGE_GRAND && d != 0){
                DEBUG_PRINT(d);
                DEBUG_PRINT("HIT");
                if(certitude == 0){
                    distance_ = 1;
                    if(d < RANGE_PETIT){
                        distance_ = 0;
                    }
                }
                
                break;
            }
            orientationN_--;
            if(remisZero > 20){
                certitude = 0;
            }
        }
        //updateOrientation();

    }
    else{
        robot_->moteur.changerDirection(Direction::DROITE);
        robot_->moteur.ajustementPWM(250,250);
        _delay_ms(100);
        robot_->moteur.ajustementPWM(100,100);
        while(true){
            remisZero++;
            //tournerDroite();
            float d = robot_->ir.distanceCm();
            if(d < RANGE_GRAND && d != 0){
                DEBUG_PRINT(d);
                DEBUG_PRINT("HIT");
                if(certitude == 0){
                    distance_ = 1;
                    if(d < RANGE_PETIT){
                        distance_ = 0;
                    }
                }
                break;
            }
            orientationN_++;
            if(remisZero > 20){
                certitude = 0;
            }
        }
         //updateOrientation();
    }
    robot_->moteur.changerDirection(Direction::AVANT);
    robot_->moteur.ajustementPWM(0,0);
    

    for (uint8_t i = 0; i < 8; i++)
    {
        tournerGauche();
    }
    certitude++;
    estCertain();


}

void Controlleur::afficherPoteau(){
    DEBUG_PRINT(distance_);
    switch (poteauBoussole)
    {
    case Boussole::NORD:
        DEBUG_PRINT("NORD");
        break;
    case Boussole::NORDEST:
        DEBUG_PRINT("NORDEST");
        break;
    case Boussole::EST:
        DEBUG_PRINT("EST");
        break;
    case Boussole::SUDEST:
        DEBUG_PRINT("SUDEST");
        break;
    case Boussole::SUD:
        DEBUG_PRINT("SUD");
        break;
    case Boussole::SUDOUEST:
        DEBUG_PRINT("SUDOUEST");
        break;
    case Boussole::OUEST:
        DEBUG_PRINT("OUEST");
        break;
    case Boussole::NORDOUEST:
        DEBUG_PRINT("NORDOUEST");
        break;
    }
}

void Controlleur::enregistrerPoteau(){
    robot_->memoire.ecriture(nombrePoteau, distance_);
    _delay_ms(25);
    robot_->memoire.ecriture(nombrePoteau + 1, orientationN_);
    _delay_ms(25);
    nombrePoteau = nombrePoteau + 2;
}

void Controlleur::estCertain(){
    bool trouve = true;
    for (uint8_t i = 0; i < 4; i++)
    {
        int distance = (int)robot_->ir.distanceCm();
        _delay_ms(50);
        if(distance > RANGE_GRAND){
            trouve = false;
        }
    }

    if(certitude > 3){
    }
    else{
        if(!trouve){
            rechercher();
        }

    }

}