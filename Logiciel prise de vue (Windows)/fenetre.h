/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : fenetre.h
Description : Déclaration de la classe CFenetre permettant de gérer la fenêtre
Auteur : Loïc PETIT
Environnement : Windows
Version : 1.5
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 17/04/18 - Création
0.2 - 18/04/18 - Ajout partie paramétrage
0.3 - 20/04/18 - Ajout de la possibilité de tester la communication
0.4 - 23/04/18 - Ajout de la possibilité de se rendre dans le mode utilisateur (Utilisation de la classe CCamera)
0.5 - 30/04/18 - Ajout du retardateur et du décompte lors du choix après la prise d'une photo
0.6 - 02/05/18 - Ajout de la vérification de l'adresse mail
0.7 - 04/05/18 - Ajout des actions des boutons "envoyer", "supprimer" et "ok"
0.8 - 09/05/18 - Modification des actions des boutons "supprimer" et "envoyer"
0.9 - 09/05/18 - Ajout de la possibilité d'envoyer le mail mis dans la zone de texte et la photo
1.0 - 11/05/18 - Modifications diverses
1.1 - 14/05/18 - Modifications diverses
1.2 - 18/05/18 - Petites modifications
1.3 - 20/05/18 - Ajout d'un décompte lors du choix d'une option sur la photo prise
1.4 - 22/05/18 - Utilisation de la classe CSocket
1.5 - 23/05/18 - Utilisation de la classe CDecompte
***************************************************************************************************/

#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QSettings>
#include <QTimer>
#include "camera.h"
#include "socket.h"
#include "decompte.h"

namespace Ui
{
    class CFenetre;
}

class CFenetre : public QWidget
{
    Q_OBJECT

    //Attributs
private:
    Ui::CFenetre *ui;

    //Attributs pour le paramétrage
    QSettings *settings;
    QString adresseIp;
    QString port;
    int dureeRetardateurPhoto;
    int dureeChoixOption;
    bool priseAutomatiquePhotos;

    //Attribut pour la communication
    CSocket *socket;

    //Attribut pour la caméra
    CCamera *camera;
    QImage photo;

    //Attributs pour le retardateur
    bool etatDecompte;
    CDecompte* decompteRetardateur;
    CDecompte* decompteChoixOption;

    //Méthodes
public:
    explicit CFenetre(QWidget *p_parent = 0);
    ~CFenetre();

    //Slots
private slots:
    //Page d'accueil
    void on_modeUtilisateurPushButton_clicked();
    void on_modeParametragePushButton_clicked();

    //Page de paramétrage
    void on_sauvegarderPushButton_clicked();
    void on_annulerPushButton_clicked();
    void on_testerCommunicationPushButton_clicked();

    //Page de la prise de photo
    void on_prendrePhotoPushButton_clicked();

    //Page du résultat de la photo et des options
    void initialisationDecompte();
    void stopperDecompteChoixOption();
    void afficherPhoto(QImage p_photo);
    void on_envoyerPushButton_clicked();
    void envoyer();
    void on_supprimerPushButton_clicked();

    //Page de l'état de la photo
    void on_okPushButton_clicked();

    //Autres
    void reussiteConnexion();
    void erreurCommunication();
    void finDecompteRetardateur();
    void finDecompteChoixOption();
};

#endif
