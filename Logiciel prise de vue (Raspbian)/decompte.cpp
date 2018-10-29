/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : decompte.cpp
Description : Définition des fonctions membres de la classe CDecompte permettant de gérer le décompte
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.1
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 23/05/18 - Création
***************************************************************************************************/

#include "decompte.h"

//-----------------------------------------------------------------------------------------------
//Methode : CDecompte
//Rôle : Constructeur de la classe
//Paramètre : p_label : label où s'affiche le décompte
//            p_temps : temps du décompte
//            p_parent : parent de la classe
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CDecompte::CDecompte(QLabel* p_label, int p_temps, QObject *p_parent)
{
    label = p_label;
    temps = p_temps;

    timerDecompte = new QTimer(p_parent);
    timerDecompte->setInterval(1000); //Définit une intervalle de 1s

    typeDecompte = true;

    //Lorsque timerDecompte envoie le signal timeout(), le slot decompte() est appelé
    connect(timerDecompte, SIGNAL(timeout()), this, SLOT(decompte()));

    timerDecompte->start(); //Le timer commence
    decompte();
}


//-----------------------------------------------------------------------------------------------
//Methode : CDecompte
//Rôle : Constructeur de la classe
//Paramètre : p_bouton : label où s'affiche le décompte
//            p_temps : temps du décompte
//            p_parent : parent de la classe
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CDecompte::CDecompte(QPushButton* p_bouton, int p_temps, QObject *p_parent)
{
    bouton = p_bouton;
    temps = p_temps;

    timerDecompte = new QTimer(p_parent);
    timerDecompte->setInterval(1000); //Définit une intervalle de 1s

    typeDecompte = false;

    //Lorsque timerDecompte envoie le signal timeout(), le slot decompte() est appelé
    connect(timerDecompte, SIGNAL(timeout()), this, SLOT(decompte()));

    timerDecompte->start(); //Le timer commence
    decompte();
}


//-----------------------------------------------------------------------------------------------
//Methode : decompte
//Rôle : Affichage du décompte
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CDecompte::decompte()
{
    if (temps >= 0)
    {
        //Actions en fonction du type de décompte (label ou bouton)
        if (typeDecompte == true)
        {
            label->setText(QString::number(temps)); //Changement du texte en fonction de la variable temps
            temps--;
        }
        else
        {
            bouton->setText(QString::number(temps)); //Changement du texte en fonction de la variable temps
            temps--;
        }
    }
    else
    {
        //Actions à la fin du décompte en fonction du type de décompte (label ou bouton)
        if (typeDecompte == true)
        {
            timerDecompte->stop(); //Le timer se stop
            emit finDecompteLabel(); //On emet le signal finDecompteLabel()
        }
        else
        {
            timerDecompte->stop(); //Le timer se stop
            emit finDecompteBouton(); //On emet le signal finDecompteBouton()
        }
    }
}


//-----------------------------------------------------------------------------------------------
//Methode : stopperDecompte
//Rôle : Arrêter le décompte
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
void CDecompte::stopperDecompte()
{
    timerDecompte->stop();
}


//-----------------------------------------------------------------------------------------------
//Methode : ~CDecompte
//Rôle : Destructeur de la classe
//Paramètre : Aucun
//Retour : Aucun
//-----------------------------------------------------------------------------------------------
CDecompte::~CDecompte()
{
    //Suppression de l'objet QTimer
    delete timerDecompte;
}
