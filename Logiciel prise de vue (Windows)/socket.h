/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : socket.h
Description : Déclaration de la classe CSocket permettant de gérer la socket TCP
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.1
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 22/05/18 - Création
***************************************************************************************************/

#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QImage>
#include <QTcpSocket>

class CSocket : public QObject
{
    Q_OBJECT

    //Attributs
private:
    QTcpSocket *socket;
    QImage photo;
    QString mail;
    bool typeReussite;

    //Méthodes
public:
    CSocket(QObject *p_parent = 0);
    ~CSocket();
    void connexion(QString p_adresseIp, int p_port);
    void connexion(QString p_adresseIp, int p_port, QImage p_photo, QString p_mail);

    //Slots
private slots:
    void reussite();
    void erreur(QAbstractSocket::SocketError p_erreur);

    //Signaux
signals:
    void reussiteConnexion();
    void erreurConnexion();
};

#endif
