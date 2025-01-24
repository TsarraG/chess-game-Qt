#pragma once
#include "Piece.hpp"
#pragma warning(push, 0)
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem> 
#pragma pop()

using namespace std;

namespace ModeleEchec {
	class Echiquier : public QObject {
		Q_OBJECT
	public:
		vector<vector<ModeleEchec::Piece*>> listePiecesAjoutee_;
		vector<vector<QGraphicsPixmapItem*>> listePieceItemAjoutee_;

		Echiquier();
		~Echiquier() {};

		ModeleEchec::Position trouverPositionRoi(ModeleEchec::Couleur joueur);

		vector<ModeleEchec::Position> trouverListeMouvementValideFou(ModeleEchec::Position position);
		vector<ModeleEchec::Position> trouverListeMouvementValideTour(ModeleEchec::Position position);
		vector<ModeleEchec::Position> trouverListeMouvementValideRoi(ModeleEchec::Position position, bool mouvement);
		vector<ModeleEchec::Position> trouverListeMouvementsValidePosition(ModeleEchec::Position pos, bool justeMouvement);
		vector<ModeleEchec::Position> listeMouvementValideJoueur(ModeleEchec::Couleur joueur, bool justeMouvement);

		bool estHorsPlateau(ModeleEchec::Position position);
		bool estCaseOccupe(ModeleEchec::Position position);
		bool estCaseJoueurAdvese(ModeleEchec::Position position, ModeleEchec::Couleur couleur);
		bool estEchec(ModeleEchec::Couleur joueur);
		bool estEnEchecApresDeplacement(const ModeleEchec::Position& pos_depart, const ModeleEchec::Position& pos_fin, ModeleEchec::Couleur joueur);

		bool estPositionDansListe(const ModeleEchec::Position& position, const vector<ModeleEchec::Position>& listeMouvementActuel);
		ModeleEchec::Piece* trouverPieceSurCase(ModeleEchec::Position position);
		void deplacerCaseACaseB(ModeleEchec::Position positionDepart, ModeleEchec::Position positionArrivee);

	public slots:
		void ajouterPiece(Piece* piece, Position position); // est-ce necessaire
		void supprimerPiece(ModeleEchec::Piece* pieceSupprime, int colonne, int ligne);
		void deplacerPiece(QGraphicsPixmapItem* pieceGraphique, Piece* piece, Position position);

	signals:
		void pieceCree(Piece* piece);
		void pieceMangee(ModeleEchec::Piece* pieceSupprime, int colonne, int ligne);
		void pieceDeplacee(QGraphicsPixmapItem* pieceGraphique, Piece* piece, Position position);

	private:
		const int NOMBRE_LIGNE = 8;
		const int NOMBRE_COLONNE = 8;
		bool estEnEchecBlanc = false;
		bool estEnEchecNoir = false;
	
	};

};