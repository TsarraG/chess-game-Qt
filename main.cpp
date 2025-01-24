/**
* Programme qui lance la fenetre
* \file Echecs.cpp
* \date 3 mai 2024
* Cree le 8 avril 2024
*/

#include "EchecsFenetre.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	InterfaceVueQt::FenetreJeu echecsFenetre;


	echecsFenetre.show();
	return app.exec();
}
