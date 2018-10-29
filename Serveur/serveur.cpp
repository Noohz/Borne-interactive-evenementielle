#include <QPixmap>
#include "serveur.h"
#include "ui_serveur.h"

Serveur::Serveur(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serveur)
{
    ui->setupUi(this);

    serveur = new QTcpServer(this);

    /*Le serveur démarre en écoutant toutes les adresses et sur le port 50000, avec des conditions,
     * on peut faire en sorte qu'il n'y ai qu'un seul client (a voir dans la méthode nouvelleConnexion())*/
    if(serveur->listen(QHostAddress::Any, 50000))
    {
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion())); //Attente d'une connexion
        ui->etatServeurLabel->setText("En attente d'une connexion");
    }
    else
    {
        ui->etatServeurLabel->setText(serveur->errorString()); //Le serveur n'a pas pu démarrer
    }

    tailleMessage = 0;
    nombrephoto = 0;
}

void Serveur::nouvelleConnexion()
{
    ui->etatServeurLabel->setText("Il y a une connexion");
    client = serveur->nextPendingConnection(); //on récupère le client (socket)

    connect(client, SIGNAL(readyRead()), this, SLOT(donneesRecues())); //Attente de donnée envoyé par client
    connect(client, SIGNAL(disconnected()), this, SLOT(deconnexionClient())); //Attente déconnexion du client
}

void Serveur::deconnexionClient()
{
    ui->etatServeurLabel->setText("En attente d'une connexion");
}

void Serveur::donneesRecues()
{
    ui->etatServeurLabel->setText("Reception en cours...");

    //On récupère le client à l'origine de l'envoi de données
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) //Si on ne le trouve pas, on abandonne
    {
        return;
    }

    QDataStream in(socket); //Flux de donnée qui va nous permettre de lire ce que contient la socket

    if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint32)) // On n'a pas reçu la taille du message en entier
        {
            return;
        }
        in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
    }

    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
    {
        return;
    }

    //Récupération de l'image
    QPixmap image;
    in >> image;

    //Récupération de l'adresse mail
    QString mail;
    in >> mail;

    //Sauvegarde de l'image
    image.save("test_" + QString::number(nombrephoto) + ".jpg", "JPG");

    //On affiche le mail sur l'IHM
    ui->adressemail->setText(mail);

    ui->etatServeurLabel->setText("Image reçue");

    //On incrémente le nombre de photo
    nombrephoto++;
    //On remet à 0 la taille du message pour une prochaine réception de données
    tailleMessage = 0;
}

Serveur::~Serveur()
{
    delete ui;
}
