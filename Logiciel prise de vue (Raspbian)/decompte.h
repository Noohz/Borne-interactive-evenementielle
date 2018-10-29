/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : decompte.h
Description : Déclaration de la classe CDecompte permettant de gérer le décompte
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.1
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 23/05/18 - Création
***************************************************************************************************/

#ifndef DECOMPTE_H
#define DECOMPTE_H

#include <QObject>
#include <QTimer>
#include <QPushButton>
#include <QLabel>

class CDecompte : public QObject
{
    Q_OBJECT

    //Attributs
private:
    QTimer *timerDecompte;
    int temps;
    bool typeDecompte;
    QPushButton* bouton;
    QLabel* label;

    //Méthodes
public:
    CDecompte(QLabel* p_label, int p_temps, QObject *p_parent = 0);
    CDecompte(QPushButton* p_bouton, int p_temps, QObject *p_parent = 0);
    ~CDecompte();
    void stopperDecompte();

    //Slots
private slots:
    void decompte();

    //Signaux
signals:
    void finDecompteLabel();
    void finDecompteBouton();
};

#endif
