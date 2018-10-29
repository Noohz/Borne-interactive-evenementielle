/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : camera.cpp
Description : Définition des fonctions membres de la classe CCamera permettant de gérer la caméra
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.4
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 23/04/18 - Création
0.2 - 11/05/18 - Ajout de message d'erreur pour la caméra
0.3 - 14/05/18 - Modifications diverses
0.4 - 18/05/18 - Petites modifications
***************************************************************************************************/

#include <QCoreApplication>
#include <QMessageBox>
#include "camera.h"

//-----------------------------------------------------------------------------------------------
//Methode : CCamera
//Rôle : Constructeur de la classe
//Paramètre : p_cameraInfo : caméra à utiliser
//            p_surface : endroit où s'affiche le flux vidéo de la caméra
//            p_parent : parent de la classe
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CCamera::CCamera(const QCameraInfo &p_cameraInfo, QCameraViewfinder *p_surface, QObject *p_parent) : camera(new QCamera(p_cameraInfo, p_parent))
{
    camera->setViewfinder(p_surface); //On définit l'endroit où le flux vidéo de la caméra s'affiche

    //Si une erreur avec la caméra est détecté
    connect(camera, SIGNAL(error(QCamera::Error)), this, SLOT(erreurCamera(QCamera::Error)));

    cameraImageCapture = new QCameraImageCapture(camera, p_parent); //Objet permettant la prise de photo

    //Si une photo est prise alors le slot envoyerPhoto() est appelé
    connect(cameraImageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(envoyerPhoto(int,QImage)));

    camera->start(); //Démarre la caméra
}


//-----------------------------------------------------------------------------------------------
//Methode : prendrePhoto
//Rôle : Prendre une photo avec la caméra
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CCamera::prendrePhoto()
{
    cameraImageCapture->capture(QCoreApplication::applicationDirPath() + "/photo.jpg"); //Prendre une photo
}


//-----------------------------------------------------------------------------------------------
//Methode : envoyerPhoto
//Rôle : Envoyer le signal photoPrise() avec la photo prise
//Paramètre : p_requestId
//            p_photo : photo prise
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CCamera::envoyerPhoto(int p_requestId, const QImage& p_photo)
{
    Q_UNUSED(p_requestId);

    emit photoPrise(p_photo); //Envoyer le signal photoPrise() avec la photo prise
}


//-----------------------------------------------------------------------------------------------
//Methode : erreurCamera
//Rôle : Affiche un message différent selon l'erreur qui est indiquée
//Paramètre : p_erreur : erreur
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CCamera::erreurCamera(QCamera::Error p_erreur)
{
    switch (p_erreur)
    {
        case QCamera::NoError:
        {
            break;
        }
        case QCamera::CameraError:
        {
            QMessageBox::warning(NULL, "Erreur", "Une erreur est survenue avec la caméra");
            break;
        }
        case QCamera::InvalidRequestError:
        {
            QMessageBox::warning(NULL, "Erreur", "Le système ne supporte pas cette fonctionnalité");
            break;
        }
        case QCamera::ServiceMissingError:
        {
            QMessageBox::warning(NULL, "Erreur", "Aucune caméra détectée");
            break;
        }
        case QCamera::NotSupportedFeatureError:
        {
            QMessageBox::warning(NULL, "Erreur", "Caméra non supportée");
            break;
        }
    };
}


//-----------------------------------------------------------------------------------------------
//Methode : ~CCamera
//Rôle : Destructeur de la classe
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CCamera::~CCamera()
{
	//Suppression des objets crées dynamiquement
    delete camera;
    delete cameraImageCapture;
}
