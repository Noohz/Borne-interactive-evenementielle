/**************************************************************************************************
Nom du projet : LogicielPriseDeVue
Nom du fichier : main.cpp
Auteur : Loïc PETIT
Environnement : Windows
Version : 0.2
---------------------------------------------------------------------------------------------------
Historique :
0.1 - 23/04/18 - Création
0.2 - 22/05/18 - Ajout de la traduction des boutons Yes/No
***************************************************************************************************/

#include <QApplication>
#include <QTranslator>
#include "fenetre.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Cette partie permet de traduire certains boutons (yes, no)
    QTranslator translator;
    translator.load(QString("qt_fr"), QCoreApplication::applicationDirPath() + QString("/translations"));
    app.installTranslator(&translator);

    CFenetre fenetre;
    fenetre.show();

    return app.exec();
}
