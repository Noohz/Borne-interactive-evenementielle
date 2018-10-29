/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : camera.h
Description : Déclaration de la classe CCamera permettant de gérer la caméra
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.1
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 02/06/18 - Création
***************************************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QImage>

class CCamera : public QObject
{
    Q_OBJECT

    //Attributs
private:

    //Méthodes
public:
    CCamera();
    ~CCamera();
    void prendrePhoto();

    //Signal
signals:
    void photoPrise(QImage p_image);
};

#endif
