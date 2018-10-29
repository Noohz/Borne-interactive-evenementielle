/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : camera.cpp
Description : Définition des fonctions membres de la classe CCamera permettant de gérer la caméra
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.1
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 02/06/18 - Création
***************************************************************************************************/

#include <QProcess>
#include <QFileInfo>
#include "camera.h"

//-----------------------------------------------------------------------------------------------
//Methode : CCamera
//Rôle : Constructeur de la classe
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CCamera::CCamera()
{
}


//-----------------------------------------------------------------------------------------------
//Methode : prendrePhoto
//Rôle : Prendre une photo avec la caméra
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CCamera::prendrePhoto()
{
    QProcess l_processus; //Objet permettant de communiquer avec un programme externe
    l_processus.start("raspistill -w 1920 -h 1080 -n -t 1 -o photo.jpg"); //Prendre la photo
    l_processus.waitForFinished(); //Attendre que la commande soit finis

    //On vérifie que la photo existe
    if (QFileInfo::exists("photo.jpg"))
    {
        emit photoPrise(QImage("photo.jpg", "JPG")); //Envoyer le signal photoPrise() avec la photo prise
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : ~CCamera
//Rôle : Destructeur de la classe
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CCamera::~CCamera()
{
}
