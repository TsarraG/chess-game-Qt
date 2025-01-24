/**
* Classe qui regroupe la vue du jeu
* \file Echecs.cpp
* \date 3 mai 2024
* Cree le 8 avril 2024
*/

#include "EchecsFenetre.hpp"
#include "PieceItems.hpp"
#pragma warning(push, 0)
#include <QLabel>
#include <QApplication>
#include <QComboBox>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QGraphicsProxyWidget>
#include <QGraphicsEllipseItem>
#include <QPoint>
#include <QGraphicsScene>
#include <QPushButton>
#include <QObject>
#include <QListWidget>
#include <QHoverEvent>
#include <QMouseEvent>
#pragma pop()
#include <iostream>
#include <memory>
#include <type_traits>

using namespace std;

InterfaceVueQt::FenetreJeu::FenetreJeu(QWidget* parent) : QMainWindow(parent), sceneGraphique_(new QGraphicsScene), casesBoutons_(new QButtonGroup), jeuEchecs_(new ModeleEchec::Echiquier())
{
	configurer();
	jeuEchecs_->listePieceItemAjoutee_.resize(8);
	for (int i = 0; i < 8; ++i) {
		jeuEchecs_->listePieceItemAjoutee_[i].resize(8);
	}

	jeuEchecs_->listePiecesAjoutee_.resize(8);
	for (int i = 0; i < 8; ++i) {
		jeuEchecs_->listePiecesAjoutee_[i].resize(8);
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			jeuEchecs_->listePieceItemAjoutee_[i][j] = nullptr;
			jeuEchecs_->listePiecesAjoutee_[i][j] = nullptr;
		}
	}
}

InterfaceVueQt::FenetreJeu::~FenetreJeu() {
	delete sceneGraphique_;
	delete casesBoutons_;
	delete jeuEchecs_;
}

void InterfaceVueQt::FenetreJeu::afficherListeMouvementValidePiece(vector<ModeleEchec::Position>& listeMouvement) {
	for (auto& pos : listeMouvement) {
		int positionX = pos.obtenirX() * DIMENSION_CASE + 25;
		int positionY = pos.obtenirY() * DIMENSION_CASE + 25;
		QGraphicsEllipseItem* cercle 
			= new QGraphicsEllipseItem(QRectF(positionX, positionY, 35, 35));
		QColor color(238, 125, 148, 128);
		QBrush brush1(color);
		cercle->setBrush(brush1);
		sceneGraphique_->addItem(cercle);
		listeRondMouvement_.push_back(cercle);
	}

}

void InterfaceVueQt::FenetreJeu::configurer() {
	
	configurerInterface();
	configurerPlateau();
	configurerChoixPosition();

	emit nouvellePartieDebutee();
	
	connect(jeuEchecs_, SIGNAL(pieceCree(ModeleEchec::Piece*)), 
		this, SLOT(ajouterPieceEchiquier(ModeleEchec::Piece*)));
	connect(jeuEchecs_, SIGNAL(pieceMangee(ModeleEchec::Piece *, int, int)), 
		this, SLOT(enleverPieces(ModeleEchec::Piece*, int, int)));
	connect(jeuEchecs_, SIGNAL(pieceDeplacee(QGraphicsPixmapItem*, 
		ModeleEchec::Piece*, ModeleEchec::Position)), this, 
		SLOT(deplacerPositionPiece(QGraphicsPixmapItem*, ModeleEchec::Piece*, ModeleEchec::Position)));


	configurerPieces();
}

void InterfaceVueQt::FenetreJeu::configurerInterface() {

	setWindowTitle("ECHECS");
	setWindowIcon(QIcon("piecesChess/b_knight.png"));

	QWidget* widgetPrincipal = new QWidget(this);
	setCentralWidget(widgetPrincipal);

	widgetPrincipal->setMinimumSize(1200, 1000);
	showMaximized();

	QVBoxLayout* layoutPrincipal = new QVBoxLayout(widgetPrincipal);
	QGraphicsView* view = new QGraphicsView(sceneGraphique_);
	layoutPrincipal->addWidget(view);

}

void InterfaceVueQt::FenetreJeu::configurerPlateau() {
	for (int i = 0; i < NOMBRE_LIGNE; i++) {
		for (int j = 0; j < NOMBRE_COLONNE; j++) {
			QPushButton* caseBouton = new QPushButton();
			casesBoutons_->addButton(caseBouton, i * NOMBRE_LIGNE + j);

			caseBouton->setProperty("colonne", i);
			caseBouton->setProperty("ligne", j);

			QObject::connect(caseBouton, SIGNAL(pressed()), this, SLOT(appuyerCase()));

			QRect caseJeuRect = QRect(j * DIMENSION_CASE, i * DIMENSION_CASE, 
				DIMENSION_CASE, DIMENSION_CASE);
			caseBouton->setGeometry(caseJeuRect);

			QPalette palette = caseBouton->palette();
			if ((i + j) % 2 == 0) {
				palette.setColor(QPalette::Button, QColor(229, 204, 255)); 
			}
			else {
				palette.setColor(QPalette::Button, QColor(79, 50, 95)); 
			}
			caseBouton->setAutoFillBackground(true);
			caseBouton->setPalette(palette);
			caseBouton->update();

			sceneGraphique_->addWidget(caseBouton);
		}
	}
}

void InterfaceVueQt::FenetreJeu::effacerPartie() {
	couleurJoueurActuel = ModeleEchec::Couleur::BLANC;
	texteHaut_->setDefaultTextColor(Qt::gray);
	encadrementTexteN_->setPen(QPen(Qt::gray, 5));
	texteBas_->setDefaultTextColor(QColor("#4F325F"));
	encadrementTexteB_->setPen(QPen(Qt::white, 5));
	estBlancJouer = true;
	estPrisePiece = false;
	effacerCercleMouvementsPossibles();
	listeMouvementActuel_.clear();
	netoyerPlateau();
}

void InterfaceVueQt::FenetreJeu::configurerPieces() {

	if (position_ == "Position par defaut") {
		effacerPartie();
		changerPositionParDefaut();
	}

	else if (position_ == "Position Fou-Tour-Roi") {
		effacerPartie();
		changerPositionFouTourRoi();
	}

	else if (position_ == "Partie Mat de l'idiot") {
		effacerPartie();
		changerPositionMatIdiot();
	}
}

void InterfaceVueQt::FenetreJeu::configurerChoixPosition() {
	QRect boitePieces = QRect(DIMENSION_CASE * NOMBRE_LIGNE + ESPACE_PLATEAU_RECTANGLE, 
		0, LARGEUR_RECTANGLE, HAUTEUR_RECTANGLE);
	sceneGraphique_->addRect(boitePieces, QPen(QColor(229, 204, 255)), QBrush(QColor(229, 204, 255)));

	QGraphicsTextItem* texteHaut = new QGraphicsTextItem("JOUER");
	texteHaut->setDefaultTextColor(Qt::gray);
	QFont font1("Verdana", 20, QFont::Bold);
	texteHaut->setFont(font1);
	QPointF positionTexteHaut(DIMENSION_CASE * NOMBRE_LIGNE + ESPACE_PLATEAU_RECTANGLE + 120, 100);
	texteHaut->setPos(positionTexteHaut);
	sceneGraphique_->addItem(texteHaut);
	texteHaut_ = texteHaut;

	QRectF boiteTexteN = texteHaut->boundingRect();
	QGraphicsRectItem* encadrementTexteN = new QGraphicsRectItem(boiteTexteN);
	encadrementTexteN->setRect(boiteTexteN);
	encadrementTexteN->setPen(QPen(Qt::gray, 5));
	encadrementTexteN->setPos(positionTexteHaut); 
	sceneGraphique_->addItem(encadrementTexteN);
	encadrementTexteN->setRect(boiteTexteN.adjusted(-10, -10, 10, 10));
	encadrementTexteN_ = encadrementTexteN;

	QListWidget* listeWidget = new QListWidget();
	listeWidget->setStyleSheet("background-color: #4F325F;");
	QFont font3("Verdana", 12); 
	listeWidget->setFont(font3);
	listeWidget->addItem("Position par defaut");
	listeWidget->addItem("Position Fou-Tour-Roi");
	listeWidget->addItem("Partie Mat de l'idiot");

	int listeX = DIMENSION_CASE * NOMBRE_LIGNE + ESPACE_PLATEAU_RECTANGLE + 30;
	int listeY = 200;

	listeWidget->setGeometry(listeX, listeY, LARGEUR_RECTANGLE - 55, 90);
	sceneGraphique_->addWidget(listeWidget);

	connect(listeWidget, &QListWidget::currentItemChanged, 
		this, &InterfaceVueQt::FenetreJeu::changerPositionDepart);

	QPushButton* boutonBoite = new QPushButton("Nouvelle Partie");
	boutonBoite->setStyleSheet("background-color: #4F325F; color: white;");
	QFont font("Verdana", 12);
	boutonBoite->setFont(font);

	int boutonX = DIMENSION_CASE * NOMBRE_LIGNE + ESPACE_PLATEAU_RECTANGLE + 30;
	int boutonY = 350;

	boutonBoite->setGeometry(boutonX, boutonY, LARGEUR_RECTANGLE - 55, 85);
	sceneGraphique_->addWidget(boutonBoite);


	QGraphicsTextItem * texteBas = new QGraphicsTextItem("JOUER");
	texteBas->setDefaultTextColor(QColor("#4F325F"));
	QFont font2("Verdana", 20, QFont::Bold);
	texteBas->setFont(font2);
	QPointF positionTexteBas(DIMENSION_CASE * NOMBRE_LIGNE + ESPACE_PLATEAU_RECTANGLE + 120, 500);
	texteBas->setPos(positionTexteBas);
	sceneGraphique_->addItem(texteBas);
	texteBas_ = texteBas;

	QRectF boiteTexteB = texteBas->boundingRect();
	QGraphicsRectItem* encadrementTexteB = new QGraphicsRectItem(boiteTexteB);
	encadrementTexteB->setRect(boiteTexteB);
	encadrementTexteB->setPen(QPen(Qt::white, 5));
	encadrementTexteB->setPos(positionTexteBas); 
	sceneGraphique_->addItem(encadrementTexteB);
	encadrementTexteB->setRect(boiteTexteB.adjusted(-10, -10, 10, 10));
	encadrementTexteB_ = encadrementTexteB;

	connect(boutonBoite, &QPushButton::clicked, this, &InterfaceVueQt::FenetreJeu::configurerPieces);

	this->activateWindow();

}

void InterfaceVueQt::FenetreJeu::deplacerPositionPiece(QGraphicsPixmapItem* pieceGraphique,
	ModeleEchec::Piece* piece, ModeleEchec::Position position) {
	jeuEchecs_->listePiecesAjoutee_[position.obtenirY()][position.obtenirX()] = piece;
	jeuEchecs_->listePieceItemAjoutee_[position.obtenirY()][position.obtenirX()] = pieceGraphique;
	piece->changerPosition(position);
	int largeur = piece->obtenirPositionX();
	int hauteur = piece->obtenirPositionY();
	pieceGraphique->setPos(largeur * DIMENSION_CASE + 5, hauteur * DIMENSION_CASE + 5);
}

void InterfaceVueQt::FenetreJeu::appuyerCase() {
	QPushButton* boutonPresse = qobject_cast<QPushButton*>(sender());
	int ligne = boutonPresse->property("ligne").toInt();
	int colonne = boutonPresse->property("colonne").toInt();

	QMessageBox messageBoxN;
	messageBoxN.setText("NOIR EST EN ECHEC!!!");

	QMessageBox messageBoxB;
	messageBoxB.setText("BLANC EST EN ECHEC!!!");

	if (!(jeuEchecs_->estEchec(couleurJoueurActuel))) {
		interpreterClique(ligne, colonne);
	}
	else
	{
		if (couleurJoueurActuel == ModeleEchec::Couleur::BLANC)
			messageBoxB.exec();
		else
			messageBoxN.exec();
		interpreterClique(ligne, colonne);
	}
}

void InterfaceVueQt::FenetreJeu::selectionnerPiece(int ligne, int colonne) {
	piece = jeuEchecs_->listePiecesAjoutee_[colonne][ligne];
	pieceItem = jeuEchecs_->listePieceItemAjoutee_[colonne][ligne];
	ancienLigne = ligne;
	ancienColonne = colonne;
	estPrisePiece = true;
	listeMouvementPossible_ = jeuEchecs_->trouverListeMouvementsValidePosition(ModeleEchec::Position(ligne, colonne), true);
	for (const auto& position1 : listeMouvementPossible_) {
		if (!(jeuEchecs_->estEnEchecApresDeplacement(ModeleEchec::Position(ligne, colonne), position1, couleurJoueurActuel))) {
			listeMouvementActuel_.push_back(position1);
		}
	}
	afficherListeMouvementValidePiece(listeMouvementActuel_);
}

void InterfaceVueQt::FenetreJeu::deposerPieceVide(int ligne, int colonne) {
	jeuEchecs_->listePiecesAjoutee_[ancienColonne][ancienLigne] = nullptr;
	jeuEchecs_->listePieceItemAjoutee_[ancienColonne][ancienLigne] = nullptr;
	jeuEchecs_->deplacerPiece(pieceItem, piece, ModeleEchec::Position(ligne, colonne));
	estPrisePiece = false;
	effacerCercleMouvementsPossibles();
	listeMouvementActuel_.clear();
}

void InterfaceVueQt::FenetreJeu::interpreterClique(int ligne, int colonne) {
	if (!estPrisePiece) {
		if ((jeuEchecs_->listePiecesAjoutee_[colonne][ligne] != nullptr)) {
			if (estBlancJouer && 
				jeuEchecs_->trouverPieceSurCase(ModeleEchec::Position(ligne, colonne))->obtenirCouleur() == ModeleEchec::Couleur::BLANC ||
				!estBlancJouer && 
				jeuEchecs_->trouverPieceSurCase(ModeleEchec::Position(ligne, colonne))->obtenirCouleur() == ModeleEchec::Couleur::NOIR) {
				selectionnerPiece(ligne, colonne);

			}
		}
	}
	else {
		if ((jeuEchecs_->listePiecesAjoutee_[colonne][ligne] == nullptr)) {
			if (jeuEchecs_->estPositionDansListe(ModeleEchec::Position(ligne, colonne), listeMouvementActuel_)) {
				deposerPieceVide(ligne, colonne);
				estBlancJouer = !estBlancJouer;
				if (estBlancJouer) {
					couleurJoueurActuel = ModeleEchec::Couleur::BLANC;

					texteHaut_->setDefaultTextColor(Qt::gray);
					encadrementTexteN_->setPen(QPen(Qt::gray, 5));

					texteBas_->setDefaultTextColor(QColor("#4F325F"));
					encadrementTexteB_->setPen(QPen(Qt::white, 5));
				}
				else {
					couleurJoueurActuel = ModeleEchec::Couleur::NOIR;

					texteBas_->setDefaultTextColor(Qt::gray);
					encadrementTexteB_->setPen(QPen(Qt::gray, 5));

					texteHaut_->setDefaultTextColor(QColor("#4F325F"));
					encadrementTexteN_->setPen(QPen(Qt::black, 5));
				}
			}
		}
		else {
			if (piece->obtenirCouleur() == jeuEchecs_->listePiecesAjoutee_[colonne][ligne]->obtenirCouleur()) {
				effacerCercleMouvementsPossibles();
				listeMouvementActuel_.clear();
				if (estBlancJouer && jeuEchecs_->trouverPieceSurCase(ModeleEchec::Position(ligne, colonne))->obtenirCouleur() 
					== ModeleEchec::Couleur::BLANC ||
					!estBlancJouer && jeuEchecs_->trouverPieceSurCase(ModeleEchec::Position(ligne, colonne))->obtenirCouleur()
					== ModeleEchec::Couleur::NOIR) {
					selectionnerPiece(ligne, colonne);
				}
			}
			else {
				if (jeuEchecs_->estPositionDansListe(ModeleEchec::Position(ligne, colonne), listeMouvementActuel_) && 
					jeuEchecs_->trouverPieceSurCase(ModeleEchec::Position(ligne, colonne))->obtenirType() 
					!= ModeleEchec::TypePiece::ROI) {
					jeuEchecs_->supprimerPiece(jeuEchecs_->listePiecesAjoutee_[colonne][ligne], colonne, ligne);
					deposerPieceVide(ligne, colonne);
					estBlancJouer = !estBlancJouer;
					if (estBlancJouer) {
						couleurJoueurActuel = ModeleEchec::Couleur::BLANC;

						texteHaut_->setDefaultTextColor(Qt::gray);
						encadrementTexteN_->setPen(QPen(Qt::gray, 5));

						texteBas_->setDefaultTextColor(QColor("#4F325F"));
						encadrementTexteB_->setPen(QPen(Qt::white, 5));
					}
					else {
						couleurJoueurActuel = ModeleEchec::Couleur::NOIR;

						texteBas_->setDefaultTextColor(Qt::gray);
						encadrementTexteB_->setPen(QPen(Qt::gray, 5));

						texteHaut_->setDefaultTextColor(QColor("#4F325F"));
						encadrementTexteN_->setPen(QPen(Qt::black, 5));
					}

				}
			}

		}
	}
}

void InterfaceVueQt::FenetreJeu::enleverPieces(ModeleEchec::Piece* pieceSupprime, int colonne,  int ligne) {
	delete jeuEchecs_->listePieceItemAjoutee_[colonne][ligne];
	jeuEchecs_->listePiecesAjoutee_[colonne][ligne] = nullptr;
	
}

void InterfaceVueQt::FenetreJeu::netoyerPlateau() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			delete jeuEchecs_->listePieceItemAjoutee_[i][j];
			jeuEchecs_->listePieceItemAjoutee_[i][j] = nullptr;
			jeuEchecs_->listePiecesAjoutee_[i][j] = nullptr;
		}
	}
}

void InterfaceVueQt::FenetreJeu::commencerPartie() {
	for (int i = 0; i < NOMBRE_CASE; i++) {
		casesBoutons_->button(i)->setIcon(QIcon());
	}
	emit nouvellePartieDebutee();
};

void InterfaceVueQt::FenetreJeu::ajouterPieceEchiquier(ModeleEchec::Piece* piece) {
	QGraphicsPixmapItem* pieceGraphique = nullptr;
	switch (piece->obtenirType()) {
	case ModeleEchec::TypePiece::ROI:
		if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
			pieceGraphique = new RoiItem(ModeleEchec::Couleur::BLANC, piece);
		else if (piece->obtenirCouleur() == ModeleEchec::Couleur::NOIR)
			pieceGraphique = new RoiItem(ModeleEchec::Couleur::NOIR, piece);
		break;
	case ModeleEchec::TypePiece::REINE:
		if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
			pieceGraphique = new ReineItem(ModeleEchec::Couleur::BLANC, piece);
		else if (piece->obtenirCouleur() == ModeleEchec::Couleur::NOIR)
			pieceGraphique = new ReineItem(ModeleEchec::Couleur::NOIR, piece);
		break;
	case ModeleEchec::TypePiece::CAVALIER:
		if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
			pieceGraphique = new CavalierItem(ModeleEchec::Couleur::BLANC, piece);
		else if (piece->obtenirCouleur() == ModeleEchec::Couleur::NOIR)
			pieceGraphique = new CavalierItem(ModeleEchec::Couleur::NOIR, piece);
		break;
	case ModeleEchec::TypePiece::PION:
		if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
			pieceGraphique = new PionItem(ModeleEchec::Couleur::BLANC, piece);
		else if (piece->obtenirCouleur() == ModeleEchec::Couleur::NOIR)
			pieceGraphique = new PionItem(ModeleEchec::Couleur::NOIR, piece);
		break;
	case ModeleEchec::TypePiece::TOUR:
		if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
			pieceGraphique = new TourItem(ModeleEchec::Couleur::BLANC, piece);
		else if (piece->obtenirCouleur() == ModeleEchec::Couleur::NOIR)
			pieceGraphique = new TourItem(ModeleEchec::Couleur::NOIR, piece);
		break;
	case ModeleEchec::TypePiece::FOU:
		if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
			pieceGraphique = new FouItem(ModeleEchec::Couleur::BLANC, piece);
		else if (piece->obtenirCouleur() == ModeleEchec::Couleur::NOIR)
			pieceGraphique = new FouItem(ModeleEchec::Couleur::NOIR, piece);
		break;
	}

	if (pieceGraphique != nullptr) {
		int largeur = piece->obtenirPositionX();
		int hauteur = piece->obtenirPositionY();
		pieceGraphique->setPos(largeur * DIMENSION_CASE + 5, hauteur * DIMENSION_CASE + 5);
		sceneGraphique_->addItem(pieceGraphique);
		jeuEchecs_->listePiecesAjoutee_[piece->obtenirPositionY()][piece->obtenirPositionX()] = piece;
		jeuEchecs_->listePieceItemAjoutee_[piece->obtenirPositionY()][piece->obtenirPositionX()] = pieceGraphique;
	}
}

void InterfaceVueQt::FenetreJeu::changerPositionDepart(QListWidgetItem* item) {
	if (item) {
		position_ = item->text();
	}
}

void InterfaceVueQt::FenetreJeu::effacerCercleMouvementsPossibles() {
	for (auto& cercle : listeRondMouvement_) {
		sceneGraphique_->removeItem(cercle);
		delete cercle;
	}
	listeRondMouvement_.clear();
}

void InterfaceVueQt::FenetreJeu::changerPositionParDefaut() {
	ModeleEchec::Piece* roiBlanc = new ModeleEchec::Piece(ModeleEchec::TypePiece::ROI, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(roiBlanc, ModeleEchec::Position(4, 7));

	ModeleEchec::Piece* roiNoir = new ModeleEchec::Piece(ModeleEchec::TypePiece::ROI, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(roiNoir, ModeleEchec::Position(4, 0));

	ModeleEchec::Piece* reineBlanche = new ModeleEchec::Piece(ModeleEchec::TypePiece::REINE, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(reineBlanche, ModeleEchec::Position(3, 7));

	ModeleEchec::Piece* reineNoir = new ModeleEchec::Piece(ModeleEchec::TypePiece::REINE, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(reineNoir, ModeleEchec::Position(3, 0));

	ModeleEchec::Piece* cavalierBlanc1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(cavalierBlanc1, ModeleEchec::Position(1, 7));
	ModeleEchec::Piece* cavalierBlanc2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(cavalierBlanc2, ModeleEchec::Position(6, 7));

	ModeleEchec::Piece* cavalierNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(cavalierNoir1, ModeleEchec::Position(1, 0));
	ModeleEchec::Piece* cavalierNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(cavalierNoir2, ModeleEchec::Position(6, 0));

	for (int i = 0; i < 8; ++i) {
		ModeleEchec::Piece* pionNoir = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
		jeuEchecs_->ajouterPiece(pionNoir, ModeleEchec::Position(i, 1));
	}

	for (int i = 0; i < 8; ++i) {
		ModeleEchec::Piece* pionBlanc = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
		jeuEchecs_->ajouterPiece(pionBlanc, ModeleEchec::Position(i, 6));
	}

	ModeleEchec::Piece* tourBlanche1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(tourBlanche1, ModeleEchec::Position(0, 7));
	ModeleEchec::Piece* tourBlanche2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(tourBlanche2, ModeleEchec::Position(7, 7));

	ModeleEchec::Piece* tourNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(tourNoir1, ModeleEchec::Position(0, 0));
	ModeleEchec::Piece* tourNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(tourNoir2, ModeleEchec::Position(7, 0));

	ModeleEchec::Piece* fouBlanc1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(fouBlanc1, ModeleEchec::Position(2, 7));
	ModeleEchec::Piece* fouBlanc2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(fouBlanc2, ModeleEchec::Position(5, 7));

	ModeleEchec::Piece* fouNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(fouNoir1, ModeleEchec::Position(2, 0));
	ModeleEchec::Piece* fouNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(fouNoir2, ModeleEchec::Position(5, 0));
}

void InterfaceVueQt::FenetreJeu::changerPositionFouTourRoi() {
	ModeleEchec::Piece* roiBlanc = new ModeleEchec::Piece(ModeleEchec::TypePiece::ROI, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(roiBlanc, ModeleEchec::Position(4, 7));

	ModeleEchec::Piece* roiNoir = new ModeleEchec::Piece(ModeleEchec::TypePiece::ROI, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(roiNoir, ModeleEchec::Position(4, 0));

	ModeleEchec::Piece* tourBlanche1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(tourBlanche1, ModeleEchec::Position(0, 7));
	ModeleEchec::Piece* tourBlanche2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(tourBlanche2, ModeleEchec::Position(7, 7));

	ModeleEchec::Piece* tourNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(tourNoir1, ModeleEchec::Position(0, 0));
	ModeleEchec::Piece* tourNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(tourNoir2, ModeleEchec::Position(7, 0));

	ModeleEchec::Piece* fouBlanc1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(fouBlanc1, ModeleEchec::Position(2, 7));
	ModeleEchec::Piece* fouBlanc2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(fouBlanc2, ModeleEchec::Position(5, 7));

	ModeleEchec::Piece* fouNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(fouNoir1, ModeleEchec::Position(2, 0));
	ModeleEchec::Piece* fouNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(fouNoir2, ModeleEchec::Position(5, 0));
}

void InterfaceVueQt::FenetreJeu::changerPositionMatIdiot() {
	ModeleEchec::Piece* roiBlanc = new ModeleEchec::Piece(ModeleEchec::TypePiece::ROI, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(roiBlanc, ModeleEchec::Position(4, 7));

	ModeleEchec::Piece* roiNoir = new ModeleEchec::Piece(ModeleEchec::TypePiece::ROI, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(roiNoir, ModeleEchec::Position(4, 0));

	ModeleEchec::Piece* reineBlanche = new ModeleEchec::Piece(ModeleEchec::TypePiece::REINE, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(reineBlanche, ModeleEchec::Position(3, 7));

	ModeleEchec::Piece* reineNoir = new ModeleEchec::Piece(ModeleEchec::TypePiece::REINE, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(reineNoir, ModeleEchec::Position(3, 0));

	ModeleEchec::Piece* cavalierBlanc1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(cavalierBlanc1, ModeleEchec::Position(1, 7));
	ModeleEchec::Piece* cavalierBlanc2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(cavalierBlanc2, ModeleEchec::Position(6, 7));

	ModeleEchec::Piece* cavalierNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(cavalierNoir1, ModeleEchec::Position(1, 0));
	ModeleEchec::Piece* cavalierNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::CAVALIER, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(cavalierNoir2, ModeleEchec::Position(6, 0));

	ModeleEchec::Piece* pionNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir1, ModeleEchec::Position(0, 1));
	ModeleEchec::Piece* pionNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir2, ModeleEchec::Position(1, 1));
	ModeleEchec::Piece* pionNoir3 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir3, ModeleEchec::Position(2, 1));
	ModeleEchec::Piece* pionNoir4 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir4, ModeleEchec::Position(3, 1));
	ModeleEchec::Piece* pionNoir5 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir5, ModeleEchec::Position(4, 3));
	ModeleEchec::Piece* pionNoir6 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir6, ModeleEchec::Position(5, 1));
	ModeleEchec::Piece* pionNoir7 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir7, ModeleEchec::Position(6, 1));
	ModeleEchec::Piece* pionNoir8 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(pionNoir8, ModeleEchec::Position(7, 1));

	ModeleEchec::Piece* pionBlanc1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc1, ModeleEchec::Position(0, 6));
	ModeleEchec::Piece* pionBlanc2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc2, ModeleEchec::Position(1, 6));
	ModeleEchec::Piece* pionBlanc3 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc3, ModeleEchec::Position(2, 6));
	ModeleEchec::Piece* pionBlanc4 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc4, ModeleEchec::Position(3, 6));
	ModeleEchec::Piece* pionBlanc5 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc5, ModeleEchec::Position(4, 6));
	ModeleEchec::Piece* pionBlanc6 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc6, ModeleEchec::Position(5, 4));
	ModeleEchec::Piece* pionBlanc7 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc7, ModeleEchec::Position(6, 4));
	ModeleEchec::Piece* pionBlanc8 = new ModeleEchec::Piece(ModeleEchec::TypePiece::PION, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(pionBlanc8, ModeleEchec::Position(7, 6));

	ModeleEchec::Piece* tourBlanche1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(tourBlanche1, ModeleEchec::Position(0, 7));
	ModeleEchec::Piece* tourBlanche2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(tourBlanche2, ModeleEchec::Position(7, 7));

	ModeleEchec::Piece* tourNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(tourNoir1, ModeleEchec::Position(0, 0));
	ModeleEchec::Piece* tourNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::TOUR, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(tourNoir2, ModeleEchec::Position(7, 0));

	ModeleEchec::Piece* fouBlanc1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(fouBlanc1, ModeleEchec::Position(2, 7));
	ModeleEchec::Piece* fouBlanc2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::BLANC);
	jeuEchecs_->ajouterPiece(fouBlanc2, ModeleEchec::Position(5, 7));

	ModeleEchec::Piece* fouNoir1 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(fouNoir1, ModeleEchec::Position(2, 0));
	ModeleEchec::Piece* fouNoir2 = new ModeleEchec::Piece(ModeleEchec::TypePiece::FOU, ModeleEchec::Couleur::NOIR);
	jeuEchecs_->ajouterPiece(fouNoir2, ModeleEchec::Position(7, 4));
}

QGraphicsPixmapItem* InterfaceVueQt::FenetreJeu::trouverPieceItemSurCase(ModeleEchec::Position position) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (jeuEchecs_->listePiecesAjoutee_[i][j]->obtenirPositionX() == position.obtenirX() && jeuEchecs_->listePiecesAjoutee_[i][j]->obtenirPositionY() == position.obtenirY()) {
				return jeuEchecs_->listePieceItemAjoutee_[i][j];
			}
		}
	}
	return nullptr;
}