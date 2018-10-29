/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : socket.cpp
Description : Définition des fonctions membres de la classe CSocket permettant de gérer la socket TCP
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.1
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 22/05/18 - Création
***************************************************************************************************/

#include <QMessageBox>
#include "socket.h"

//-----------------------------------------------------------------------------------------------
//Methode : CSocket
//Rôle : Constructeur de la classe
//Paramètre : p_parent : parent de la classe
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CSocket::CSocket(QObject *p_parent)
{
    socket = new QTcpSocket(p_parent);

    //Si la connexion est établie alors le slot reussiteConnexion() est appelé
    connect(socket, SIGNAL(connected()), this, SLOT(reussite()));
    //Si une erreur est détectée lors de la communication, le slot erreurCommunication() est appelé
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(erreur(QAbstractSocket::SocketError)));
}


//-----------------------------------------------------------------------------------------------
//Methode : connexion
//Rôle : Connexion à l'adresse IP et au port (test de la communication)
//Paramètre : p_adresseIp : adresse IP
//            p_port : port
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CSocket::connexion(QString p_adresseIp, int p_port)
{
    typeReussite = true; //Test de la communication
    socket->abort(); //On désactive les connexions précédentes s'il y en a
    socket->connectToHost(p_adresseIp, p_port); //Connexion à l'adresse IP et au port
}


//-----------------------------------------------------------------------------------------------
//Methode : connexion
//Rôle : Connexion à l'adresse IP et au port (envoi d'une photo et d'un mail)
//Paramètre : p_adresseIp : adresse IP
//            p_port : port
//            p_photo : photo à envoyer
//            p_mail : mail à envoyer
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CSocket::connexion(QString p_adresseIp, int p_port, QImage p_photo, QString p_mail)
{
    typeReussite = false; //Envoi d'une photo et d'un mail
    photo = p_photo;
    mail = p_mail;
    socket->abort(); //On désactive les connexions précédentes s'il y en a
    socket->connectToHost(p_adresseIp, p_port); //Connexion à l'adresse IP et au port
}


//-----------------------------------------------------------------------------------------------
//Methode : reussite
//Rôle : Effectuer des actions après la réussite de la connexion
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CSocket::reussite()
{
    if (typeReussite == true)
    {
        QMessageBox::information(NULL,"Connexion","Réussite de la connexion",QMessageBox::Ok);
    }
    else
    {
        //Préparation du paquet
        QByteArray l_paquet;
        QDataStream out(&l_paquet, QIODevice::WriteOnly);

        out << (quint32) 0; //On écrit 0 au début du paquet pour réserver la place pour écrire la taille
        out << QPixmap::fromImage(photo); // On ajoute la photo
        out << mail; // On ajoute l'adresse mail
        out.device()->seek(0); //On se replace au début du paquet
        out << (quint32) (l_paquet.size() - sizeof(quint32)); //On écrase le 0 qu'on avait réservé par la longueur du message

        socket->write(l_paquet); //Envoi du paquet
    }
    emit reussiteConnexion(); //Le signal reussiteConnexion() est émis
    socket->disconnectFromHost(); //Déconnexion
}


//-----------------------------------------------------------------------------------------------
//Methode : erreur
//Rôle : Affiche un message différent selon l'erreur qui est indiquée
//Paramètre : p_erreur : erreur
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CSocket::erreur(QAbstractSocket::SocketError p_erreur)
{
    switch(p_erreur)
    {
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::warning(NULL,"Erreur de communication","Le serveur n'a pas pu être trouvé. Vérifiez l'adresse IP et le port.",QMessageBox::Ok);
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::warning(NULL,"Erreur de communication","Le serveur a refusé la connexion. Vérifiez si le logiciel du système de diffusion a bien été lancé. Vérifiez aussi l'adresse IP et le port.",QMessageBox::Ok);
        break;
        case QAbstractSocket::RemoteHostClosedError:
            QMessageBox::warning(NULL,"Erreur de communication","Le serveur a coupé la connexion",QMessageBox::Ok);
            break;
        default:
            QMessageBox::warning(NULL,"Erreur de communication","Erreur : " + socket->errorString(),QMessageBox::Ok);
    }
    emit erreurConnexion(); //Le signal erreurConnexion() est émis
}


//-----------------------------------------------------------------------------------------------
//Methode : ~CSocket
//Rôle : Destructeur de la classe
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CSocket::~CSocket()
{
    delete socket;
}
