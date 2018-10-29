/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : camera.h
Description : Déclaration de la classe CCamera permettant de gérer la caméra
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

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QCameraViewfinder>

class CCamera : public QObject
{
    Q_OBJECT

    //Attributs
private:
    QCamera *camera;
    QCameraImageCapture *cameraImageCapture;

    //Méthodes
public:
    CCamera(const QCameraInfo &p_cameraInfo, QCameraViewfinder *p_surface, QObject *p_parent = 0);
    ~CCamera();
    void prendrePhoto();

    //Slots
private slots:
    void envoyerPhoto(int p_requestId, const QImage& p_photo);
    void erreurCamera(QCamera::Error p_erreur);

    //Signal
signals:
    void photoPrise(QImage p_image);
};

#endif
