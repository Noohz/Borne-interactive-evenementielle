#ifndef SERVEUR_H
#define SERVEUR_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class Serveur;
}

class Serveur : public QWidget
{
    Q_OBJECT

public:
    explicit Serveur(QWidget *parent = 0);
    ~Serveur();

private:
    Ui::Serveur *ui;

    QTcpServer *serveur;
    QTcpSocket *client;
    quint32 tailleMessage;
    int nombrephoto;

private slots:
    void nouvelleConnexion();
    void deconnexionClient();
    void donneesRecues();
};

#endif // SERVEUR_H
