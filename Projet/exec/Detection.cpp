/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */
#include "Controlleur.h"

void Controlleur::detecter(){
    switch (etatDetection_)
    {
    case EtatDetection::ATTENDRE_1:
        DEBUG_PRINT("ATTENDRE_1");
        switch (bouton)
            {
            case TypeBouton::AUCUN:
                
                break;
            
            case TypeBouton::INTERUPT:
                DEBUG_PRINT("INTERUPT");
                bouton = TypeBouton::AUCUN;
                etatDetection_ = EtatDetection::HAUT;
                break;
            
            case TypeBouton::EXTERNE:
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
        etatDetection_ = EtatDetection::RECHERCHE;
        break;

    case EtatDetection::DROIT:
        DEBUG_PRINT("DROIT");
        orientationN_ = 2 * HUITIEME;
        updateOrientation();
        etatDetection_ = EtatDetection::RECHERCHE;
        break;

    case EtatDetection::RECHERCHE:
        rechercher();
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
        tournerGauche(); //pour se positionner
        suivre(3, RANGE_GRAND, 6);
        etatDetection_ = EtatDetection::PROCHE;
        break;

    case EtatDetection::PROCHE:
        DEBUG_PRINT("PROCHE");
        afficherPoteau();
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
    if(orientation_ == Boussole::EST){
        while(true){
            tournerGauche();
            float d = robot_->ir.distanceInch();
            if(d < RANGE_GRAND){
                DEBUG_PRINT("HIT");
                distance_ = 1;
                if(d < RANGE_PETIT){
                    distance_ = 0;
                }
                break;
            }
        }
    }
    else{
        while(true){
            tournerDroite();
            float d = robot_->ir.distanceInch();
            if(d < RANGE_GRAND){
                DEBUG_PRINT("HIT");
                distance_ = 1;
                if(d < RANGE_PETIT){
                    distance_ = 0;
                }
                break;
            }
        }
    }


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