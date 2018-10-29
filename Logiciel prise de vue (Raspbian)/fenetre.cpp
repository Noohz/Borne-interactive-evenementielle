/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : fenetre.cpp
Description : Définition des fonctions membres de la classe CFenetre permettant de gérer la fenêtre
Auteur : Loïc PETIT
Environnement : Windows
Version : 1.6
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 17/04/18 - Création
0.2 - 18/04/18 - Ajout partie paramétrage
0.3 - 20/04/18 - Ajout de la possibilité de tester la communication
0.4 - 23/04/18 - Ajout de la possibilité de se rendre dans le mode utilisateur
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
1.6 - 02/06/18 - Utilisation de la classe CCamera
************************************************************************************************/

#include <QFileInfo>
#include <QMessageBox>
#include "fenetre.h"
#include "ui_fenetre.h"


//-----------------------------------------------------------------------------------------------
//Methode : CFenetre
//Rôle : Constructeur de la classe
//Paramètre : p_parent : parent de la classe
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CFenetre::CFenetre(QWidget *p_parent) : QWidget(p_parent), ui(new Ui::CFenetre)
{
    ui->setupUi(this);

    //Partie paramétrage du logiciel dans un fichier de configuration de type .ini
    if (QFileInfo::exists("options.ini"))
    {
        settings = new QSettings("options.ini", QSettings::IniFormat, this);

        //Les attributs récupèrent les valeurs du fichier de configuration
        adresseIp = settings->value("Communication/adresse_ip").toString();
        port = settings->value("Communication/port").toString();
        dureeRetardateurPhoto = settings->value("Prise_de_vue/duree_retardateur_photo").toInt();
        dureeChoixOption = settings->value("Prise_de_vue/duree_choix_option").toInt();
        priseAutomatiquePhotos = settings->value("Prise_de_vue/prise_automatique_photos").toBool();

        //Les widgets prennent les valeurs des attributs
        ui->adresseIpLineEdit->setText(adresseIp);
        ui->portLineEdit->setText(port);
        ui->dureeRetardateurPhotoSpinBox->setValue(dureeRetardateurPhoto);
        ui->dureeChoixOptionSpinBox->setValue(dureeChoixOption);
        ui->priseAutomatiquePhotosCheckBox->setChecked(priseAutomatiquePhotos);
    }
    else
    {
        settings = new QSettings("options.ini", QSettings::IniFormat, this);

        //Les attributs récupèrent les valeurs des widgets
        adresseIp = ui->adresseIpLineEdit->text();
        port = ui->portLineEdit->text();
        dureeRetardateurPhoto = ui->dureeRetardateurPhotoSpinBox->value();
        dureeChoixOption = ui->dureeChoixOptionSpinBox->value();
        priseAutomatiquePhotos = ui->priseAutomatiquePhotosCheckBox->checkState();

        //Les valeurs sont placées dans le fichier de configuration
        settings->setValue("Communication/adresse_ip", adresseIp);
        settings->setValue("Communication/port", port);
        settings->setValue("Prise_de_vue/duree_retardateur_photo", dureeRetardateurPhoto);
        settings->setValue("Prise_de_vue/duree_choix_option", dureeChoixOption);
        settings->setValue("Prise_de_vue/prise_automatique_photos", priseAutomatiquePhotos);

        ui->stackedWidget->setCurrentIndex(1);
    }

    socket = new CSocket(this); //Objet socket

    //Si la connexion est établie alors le slot reussiteConnexion() est appelé
    connect(socket, SIGNAL(reussiteConnexion()), this, SLOT(reussiteConnexion()));
    //Si une erreur est détectée lors de la communication, le slot erreurCommunication() est appelé
    connect(socket, SIGNAL(erreurConnexion()), this, SLOT(erreurCommunication()));

    camera = new CCamera; //Objet caméra

    //Si une photo est prise alors le slot afficherPhoto() est appelé
    connect(camera, SIGNAL(photoPrise(QImage)), this, SLOT(afficherPhoto(QImage)));

    //Lorsque la page du stacked widget est changé, le slot initialisationDecompte() est appelé
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(initialisationDecompte()));

    etatDecompte = true; //Le décompte peut être arrêter

    //Lorsque la position du curseur dans la zone de texte est changée, le slot stopperDecompteChoixOption() est appelé
    connect(ui->adresseMailLineEdit, SIGNAL(textChanged(QString)), this, SLOT(stopperDecompteChoixOption()));
}


//-----------------------------------------------------------------------------------------------
//Methode : on_modeUtilisateurPushButton_clicked
//Rôle : Afficher la page utilisateur
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_modeUtilisateurPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2); //Vers la page utilisateur
}


//-----------------------------------------------------------------------------------------------
//Methode : on_modeParametragePushButton_clicked
//Rôle : Afficher la page paramétrage
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_modeParametragePushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); //Vers la page de paramétrage
}


//-----------------------------------------------------------------------------------------------
//Methode : on_sauvegarderPushButton_clicked
//Rôle : Sauvegarder le paramétrage
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_sauvegarderPushButton_clicked()
{
    if (settings->isWritable() == true)
    {
        ui->stackedWidget->setCurrentIndex(0); //Vers la page d'accueil

        //Les valeurs des widgets sont placées dans le fichier de configuration
        settings->setValue("Communication/adresse_ip", ui->adresseIpLineEdit->text());
        settings->setValue("Communication/port", ui->portLineEdit->text());
        settings->setValue("Prise_de_vue/duree_retardateur_photo", ui->dureeRetardateurPhotoSpinBox->value());
        settings->setValue("Prise_de_vue/duree_choix_option", ui->dureeChoixOptionSpinBox->value());
        settings->setValue("Prise_de_vue/prise_automatique_photos", ui->priseAutomatiquePhotosCheckBox->isChecked());

        //Les attributs récupèrent les valeurs dans le fichier de configuration
        adresseIp = settings->value("Communication/adresse_ip").toString();
        port = settings->value("Communication/port").toString();
        dureeRetardateurPhoto = settings->value("Prise_de_vue/duree_retardateur_photo").toInt();
        dureeChoixOption = settings->value("Prise_de_vue/duree_choix_option").toInt();
        priseAutomatiquePhotos = settings->value("Prise_de_vue/prise_automatique_photos").toBool();

        QMessageBox::information(this, "Modification effectuées", "Les modifications ont bien été effectuées", QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Impossible de sauvegarder les modifications (fichier en lecture seule)", QMessageBox::Ok);
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : on_annulerPushButton_clicked
//Rôle : Annuler le paramétrage
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_annulerPushButton_clicked()
{
    QMessageBox::StandardButton l_reponse;

    l_reponse = QMessageBox::question(this, "Annuler", "Voulez vous annuler les modifications apportées ?", QMessageBox::Yes|QMessageBox::No);

    if (l_reponse == QMessageBox::Yes)
    {
        ui->stackedWidget->setCurrentIndex(0); //Vers la page d'accueil

        //Les valeurs des widgets sont replacées en fonction des attributs
        ui->adresseIpLineEdit->setText(adresseIp);
        ui->portLineEdit->setText(port);
        ui->dureeRetardateurPhotoSpinBox->setValue(dureeRetardateurPhoto);
        ui->dureeChoixOptionSpinBox->setValue(dureeChoixOption);
        ui->priseAutomatiquePhotosCheckBox->setChecked(priseAutomatiquePhotos);
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : on_testerCommunicationPushButton_clicked
//Rôle : Tester la communication à l'adresse IP et au port
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_testerCommunicationPushButton_clicked()
{
    //On désactive les boutons
    ui->testerCommunicationPushButton->setEnabled(false);
    ui->sauvegarderPushButton->setEnabled(false);
    ui->annulerPushButton->setEnabled(false);

    //Connexion à l'adresse IP et au port
    socket->connexion(ui->adresseIpLineEdit->text(), ui->portLineEdit->text().toInt());
}


//-----------------------------------------------------------------------------------------------
//Methode : reussiteConnexion
//Rôle : Effectuer des actions après la réussite de la connexion
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::reussiteConnexion()
{
    //Si on se connecte pour l'envoi de la photo
    if (ui->stackedWidget->currentIndex() == 3)
    {
        //On vérifie que la photo existe
        if (QFileInfo::exists("photo.jpg"))
        {
            QFile::remove("photo.jpg"); //Suppression de photo.jpg
        }

        ui->etatPhotoLabel->setText("L'image a été envoyée");
        ui->stackedWidget->setCurrentIndex(4); //Vers la page d'état de la photo

        //On réactive les boutons
        ui->envoyerPushButton->setEnabled(true);
        ui->supprimerPushButton->setEnabled(true);
        ui->adresseMailLineEdit->setEnabled(true);
    }
    //Si on se connecte lors du test de communication
    else if (ui->stackedWidget->currentIndex() == 1)
    {
        //On réactive les boutons
        ui->testerCommunicationPushButton->setEnabled(true);
        ui->sauvegarderPushButton->setEnabled(true);
        ui->annulerPushButton->setEnabled(true);
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : erreurCommunication
//Rôle : Actions lors d'une erreur de communication
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::erreurCommunication()
{
    //Changement de l'état des boutons
    if (ui->testerCommunicationPushButton->isEnabled() == false)
    {
        //On réactive les boutons
        ui->testerCommunicationPushButton->setEnabled(true);
        ui->sauvegarderPushButton->setEnabled(true);
        ui->annulerPushButton->setEnabled(true);
    }
    if (ui->envoyerPushButton->isEnabled() == false)
    {
        //On réactive les boutons
        ui->envoyerPushButton->setEnabled(true);
        ui->supprimerPushButton->setEnabled(true);
        ui->adresseMailLineEdit->setEnabled(true);
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : on_prendrePhotoPushButton_clicked
//Rôle : Prendre en photo et initialiser le décompte
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_prendrePhotoPushButton_clicked()
{
    ui->prendrePhotoPushButton->setEnabled(false);
    decompteRetardateur = new CDecompte(ui->prendrePhotoPushButton, dureeRetardateurPhoto, this);
    connect(decompteRetardateur, SIGNAL(finDecompteBouton()), this, SLOT(finDecompteRetardateur()));
}


//-----------------------------------------------------------------------------------------------
//Methode : initialisationDecompte
//Rôle : Initialiser le décompte pour le choix d'une option
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::initialisationDecompte()
{
    if(ui->stackedWidget->currentIndex() == 3)
    {
        decompteChoixOption = new CDecompte(ui->decompteChoixOptionLabel, dureeChoixOption, this);
        connect(decompteChoixOption, SIGNAL(finDecompteLabel()), this, SLOT(finDecompteChoixOption()));
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : stopperDecompteChoixOption
//Rôle : Arrêter le décompte du choix d'une option
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::stopperDecompteChoixOption()
{
   if (etatDecompte == true)
   {
       decompteChoixOption->stopperDecompte();
       delete decompteChoixOption;
       ui->decompteChoixOptionLabel->setText("");
       etatDecompte = false;
   }
}


//-----------------------------------------------------------------------------------------------
//Methode : afficherPhoto
//Rôle : Afficher la photo prise avec les options
//Paramètre : p_photo : photo prise
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::afficherPhoto(QImage p_photo)
{
    photo = p_photo;
    ui->photoLabel->setPixmap(QPixmap::fromImage(photo.scaled(764, 380, Qt::KeepAspectRatio))); //On applique la photo au QLabel
    ui->photoLabel->setAlignment(Qt::AlignCenter);

    ui->stackedWidget->setCurrentIndex(3); //Vers la page de choix d'une option pour la photo
}


//-----------------------------------------------------------------------------------------------
//Methode : on_envoyerPushButton_clicked
//Rôle : Actions lors de l'appui sur le bouton "envoyer"
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_envoyerPushButton_clicked()
{
    stopperDecompteChoixOption(); //Arrêt du décompte
    envoyer();
}


//-----------------------------------------------------------------------------------------------
//Methode : envoyer
//Rôle : Envoyer la photo prise et le mail
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::envoyer()
{
    //Définition des caractères pouvant être utiliser dans une adresse mail
    QRegularExpression l_formatMail("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");

    //On vérifie si l'adresse mail est correcte
    if(l_formatMail.match(ui->adresseMailLineEdit->text()).hasMatch() || ui->adresseMailLineEdit->text() == "")
    {
        //On désactive les boutons
        ui->envoyerPushButton->setEnabled(false);
        ui->supprimerPushButton->setEnabled(false);
        ui->adresseMailLineEdit->setEnabled(false);

        socket->connexion(adresseIp, port.toInt(), photo, ui->adresseMailLineEdit->text());
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Adresse mail non valide");
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : on_supprimerPushButton_clicked
//Rôle : Supprimer la photo prise
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_supprimerPushButton_clicked()
{
    stopperDecompteChoixOption(); //Arrêt du décompte

    QMessageBox::StandardButton l_reponse;

    //Message de confirmation
    l_reponse = QMessageBox::question(this, "Supprimer", "Voulez vous supprimer la photo ?", QMessageBox::Yes|QMessageBox::No);

    if (l_reponse == QMessageBox::Yes)
    {
        //On vérifie que la photo existe
        if (QFileInfo::exists("photo.jpg"))
        {
            QFile::remove("photo.jpg"); //Suppression de photo.jpg
        }

        ui->etatPhotoLabel->setText("La photo a été supprimée");

        ui->stackedWidget->setCurrentIndex(4); //Vers la page d'état de la photo
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : on_okPushButton_clicked
//Rôle : Retourner à la page permettant la prise de photo
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::on_okPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2); //Vers la page de la prise de photo
    ui->adresseMailLineEdit->setText(""); //On vide la zone de texte
    if(etatDecompte == false)
    {
        etatDecompte = true;
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : finDecompteRetardateur
//Rôle : Actions à la fin du décompte pour le retardateur
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::finDecompteRetardateur()
{
    camera->prendrePhoto(); //Prendre une photo
    ui->prendrePhotoPushButton->setText("Prendre en photo");
    ui->prendrePhotoPushButton->setEnabled(true);
    delete decompteRetardateur;
}


//-----------------------------------------------------------------------------------------------
//Methode : finDecompteChoixOption
//Rôle : Actions à la fin du décompte pour le le choix d'une option
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CFenetre::finDecompteChoixOption()
{
    envoyer(); //Envoi de la photo et du mail
    ui->decompteChoixOptionLabel->setText("");
	etatDecompte = false;
    delete decompteChoixOption;
}


//-----------------------------------------------------------------------------------------------
//Methode : ~CFenetre
//Rôle : Destructeur de la classe
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CFenetre::~CFenetre()
{
	//Suppression des objets crées dynamiquement
    delete camera;
    delete settings;
    delete socket;
    delete ui;
}
