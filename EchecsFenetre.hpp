#pragma once
#include "Echecs.hpp"
#include "Piece.hpp"
#pragma warning(push, 0)
#include <QMainWindow>
#include <QPushButton>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QRect> 
#include <QMessageBox>
#include <QGraphicsItem>
#include <QListWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#pragma pop()

namespace InterfaceVueQt {
	class FenetreJeu : public QMainWindow {
		Q_OBJECT

	public:
		const int NOMBRE_LIGNE = 8;
		const int NOMBRE_COLONNE = 8;

		FenetreJeu(QWidget* parent = nullptr);
		~FenetreJeu();

		void changerPositionParDefaut();
		void changerPositionFouTourRoi();
		void changerPositionMatIdiot();
		void netoyerPlateau();
		void afficherListeMouvementValidePiece(vector<ModeleEchec::Position>& listeMouvement);
		void effacerCercleMouvementsPossibles();
		void effacerPartie();
		void interpreterClique(int ligne, int colonne);
		void selectionnerPiece(int ligne, int colonne);
		void deposerPieceVide(int ligne, int colonne);
		QGraphicsPixmapItem* trouverPieceItemSurCase(ModeleEchec::Position position);

	public slots:
		void commencerPartie();
		void ajouterPieceEchiquier(ModeleEchec::Piece* piece); 
		void changerPositionDepart(QListWidgetItem* item);
		void enleverPieces(ModeleEchec::Piece* pieceSupprime, int colonne, int ligne);

		void appuyerCase();

		void deplacerPositionPiece(QGraphicsPixmapItem* pieceGraphique, ModeleEchec::Piece* piece, ModeleEchec::Position position);

	signals:
		void nouvellePartieDebutee();

	private:
		void configurer();
		void configurerInterface();
		void configurerPlateau();
		void configurerChoixPosition();
		void configurerPieces();

		ModeleEchec::Echiquier* jeuEchecs_;
		QGraphicsScene* sceneGraphique_;
		QString position_;
		QButtonGroup* casesBoutons_;
		
		ModeleEchec::Couleur couleurJoueurActuel = ModeleEchec::Couleur::BLANC; 
		vector <QGraphicsEllipseItem*> listeRondMouvement_;
		vector < ModeleEchec::Position> listeMouvementActuel_;
		vector < ModeleEchec::Position> listeMouvementPossible_;
		
		
		vector < ModeleEchec::Position> listeMouvementValideTotal_;

		bool estPrisePiece = false;
		bool estBlancJouer = true;

		int ancienLigne = 0;
		int ancienColonne = 0;
		ModeleEchec::Piece* piece = 0;
		QGraphicsPixmapItem* pieceItem = 0;

		const int LARGEUR_RECTANGLE = 350;
		const int HAUTEUR_RECTANGLE = 950;
		const int ESPACE_PLATEAU_RECTANGLE = 50;

		const int DIMENSION_CASE = 85; 
		const int DIMENSION_PIECE = DIMENSION_CASE - 10;
		const int NOMBRE_CASE = 64;

		QGraphicsTextItem* texteHaut_;
		QGraphicsTextItem* texteBas_;
		QGraphicsRectItem* encadrementTexteB_;
		QGraphicsRectItem* encadrementTexteN_;

		QMessageBox messageBoxN_;
		QMessageBox messageBoxB_;
	};

	
}


