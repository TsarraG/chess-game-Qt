//#pragma once
//#include "Piece.hpp"
//#pragma warning(push, 0)
//#include <QObject>
//#include <QMainWindow>
//#include <QPushButton>
//#include <QButtonGroup>
//#include <QGraphicsScene>
//#include <QGraphicsView>
//#include <QHBoxLayout>
//#include <QRect> 
//#include <QMessageBox>
//#include <QGraphicsItem>
//#include <QListWidget>
//#include <QGraphicsPixmapItem>
//#pragma pop()
//#include <vector>
//using namespace std;
//
//namespace ModeleEchec {
//	class JeuEchec {//: public QObject {
//		//Q_OBJECT
//	public:
//		vector<vector<ModeleEchec::Piece*>> listePieces_;
//		vector<vector<QGraphicsPixmapItem*>> listePieceVue_;
//
//		JeuEchec() {}
//		~JeuEchec() {}
//
//		ModeleEchec::Position trouverPositionRoi(ModeleEchec::Couleur joueur);
//
//		vector<ModeleEchec::Position> trouverListeMouvementValideFou(ModeleEchec::Position position);
//		vector<ModeleEchec::Position> trouverListeMouvementValideTour(ModeleEchec::Position position);
//		vector<ModeleEchec::Position> trouverListeMouvementValideRoi(ModeleEchec::Position position, bool mouvement);
//		vector<ModeleEchec::Position> trouverListeMouvementsValidePosition(ModeleEchec::Position pos, bool justeMouvement);
//		vector<ModeleEchec::Position> listeMouvementValideJoueur(ModeleEchec::Couleur joueur, bool justeMouvement);
//
//		bool estHorsPlateau(ModeleEchec::Position position);
//		bool estCaseOccupe(ModeleEchec::Position position);
//		bool estCaseJoueurAdvese(ModeleEchec::Position position, ModeleEchec::Couleur couleur);
//		bool estEchec(ModeleEchec::Couleur joueur);
//
//		bool estPositionDansListe(const ModeleEchec::Position& position, const vector<ModeleEchec::Position>& listeMouvementActuel);
//
//		ModeleEchec::Piece* trouverPieceSurCase(ModeleEchec::Position position);
//		void deplacerCaseACaseB(ModeleEchec::Position positionDepart, ModeleEchec::Position positionArrivee);
//	public slots:
//	signals:
//
//	private:
//		const int NOMBRE_LIGNE = 8;
//		const int NOMBRE_COLONNE = 8;
//	};
//}