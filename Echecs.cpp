/**
* Classe qui regroupe le modele du jeu
* \file Echecs.cpp
* \date 3 mai 2024
* Cree le 8 avril 2024
*/

#include "Echecs.hpp"
#include <iostream>

using namespace std;

ModeleEchec::Echiquier::Echiquier() {
}

void ModeleEchec::Echiquier::ajouterPiece(Piece* piece, Position position)
{
	piece->changerPosition(position);
	emit pieceCree(piece);
}

void ModeleEchec::Echiquier::supprimerPiece(ModeleEchec::Piece* pieceSupprime, 
	int colonne, int ligne)
{
	emit pieceMangee(pieceSupprime, colonne, ligne);
}

void ModeleEchec::Echiquier::deplacerPiece(QGraphicsPixmapItem* pieceGraphique, 
	Piece* piece, Position position)
{
	emit pieceDeplacee(pieceGraphique, piece, position);	
}

bool ModeleEchec::Echiquier::estHorsPlateau(ModeleEchec::Position position) {
	return position.obtenirX() < 0 || position.obtenirX() >= NOMBRE_COLONNE ||
		position.obtenirY() < 0 || position.obtenirY() >= NOMBRE_LIGNE;
}

bool ModeleEchec::Echiquier::estCaseOccupe(ModeleEchec::Position position) {
	return listePiecesAjoutee_[position.obtenirY()][position.obtenirX()] != nullptr;
}

bool ModeleEchec::Echiquier::estCaseJoueurAdvese(ModeleEchec::Position position,
	ModeleEchec::Couleur couleur) {
	return listePiecesAjoutee_[position.obtenirY()][position.obtenirX()]->obtenirCouleur() != couleur;
}

ModeleEchec::Piece* ModeleEchec::Echiquier::trouverPieceSurCase(ModeleEchec::Position position) {
	if (listePiecesAjoutee_[position.obtenirY()][position.obtenirX()] != nullptr)
		return listePiecesAjoutee_[position.obtenirY()][position.obtenirX()];
	return nullptr;
}

void ModeleEchec::Echiquier::deplacerCaseACaseB(ModeleEchec::Position positionDepart, 
	ModeleEchec::Position positionArrivee) {
	listePiecesAjoutee_[positionArrivee.obtenirY()][positionArrivee.obtenirX()] 
		= listePiecesAjoutee_[positionDepart.obtenirY()][positionDepart.obtenirX()];
	listePiecesAjoutee_[positionDepart.obtenirY()][positionDepart.obtenirX()] = nullptr;
}

bool ModeleEchec::Echiquier::estEchec(ModeleEchec::Couleur joueur) {
	ModeleEchec::Position posRoi = trouverPositionRoi(joueur);
	std::vector<ModeleEchec::Position> mouvementsAdversaires;
	if (joueur == ModeleEchec::Couleur::BLANC) {
		mouvementsAdversaires = listeMouvementValideJoueur(ModeleEchec::Couleur::NOIR, true);
	}
	else {
		mouvementsAdversaires = listeMouvementValideJoueur(ModeleEchec::Couleur::BLANC, true);
	}
	return std::find(mouvementsAdversaires.begin(), mouvementsAdversaires.end(), posRoi) != mouvementsAdversaires.end();
}
vector<ModeleEchec::Position>  ModeleEchec::Echiquier::listeMouvementValideJoueur(ModeleEchec::Couleur joueur, 
	bool justeMouvement = false) {
	vector<ModeleEchec::Position> listeMouvement;
	for (int i = 0; i < NOMBRE_LIGNE; i++) {
		for (int j = 0; j < NOMBRE_COLONNE; j++) {
			ModeleEchec::Position position(i, j);
			if (estCaseOccupe(position)) {
				if (trouverPieceSurCase(position)->obtenirCouleur() == joueur) {
					vector<ModeleEchec::Position> listeJoueur 
						= trouverListeMouvementsValidePosition(position, justeMouvement);
					if (!listeJoueur.empty()) {
						listeMouvement.insert(listeMouvement.end(), 
							listeJoueur.begin(), listeJoueur.end());
					}
				}
			}
		}
	}
	return listeMouvement;
}

vector<ModeleEchec::Position>  ModeleEchec::Echiquier::trouverListeMouvementsValidePosition(ModeleEchec::Position pos, bool justeMouvement = false) {
	vector<ModeleEchec::Position> listeMouvement;
	ModeleEchec::TypePiece typePiece = trouverPieceSurCase(pos)->obtenirType();
	switch (typePiece) {
	case ModeleEchec::TypePiece::TOUR:
		listeMouvement = trouverListeMouvementValideTour(pos);
		break;
	case ModeleEchec::TypePiece::CAVALIER:
		break;
	case ModeleEchec::TypePiece::FOU:
		listeMouvement = trouverListeMouvementValideFou(pos);
		break;
	case ModeleEchec::TypePiece::REINE:
		break;
	case ModeleEchec::TypePiece::PION:
		break;
	case ModeleEchec::TypePiece::ROI:
		listeMouvement = trouverListeMouvementValideRoi(pos, justeMouvement);
		break;
	default:
		break;
	}

	return listeMouvement;
}

ModeleEchec::Position  ModeleEchec::Echiquier::trouverPositionRoi(ModeleEchec::Couleur joueur) {
	for (int i = 0; i < NOMBRE_LIGNE; i++) {
		for (int j = 0; j < NOMBRE_COLONNE; j++) {
			ModeleEchec::Position position(i, j);
			if (estCaseOccupe(position)) {
				if (trouverPieceSurCase(position)->obtenirType() == ModeleEchec::TypePiece::ROI &&
					trouverPieceSurCase(position)->obtenirCouleur() == joueur) {
					return position;
				}
			}
		}
	}
	return ModeleEchec::Position(-1, -1);
}


vector<ModeleEchec::Position> ModeleEchec::Echiquier::trouverListeMouvementValideFou(ModeleEchec::Position position) {
	vector<ModeleEchec::Position> listeMouvementFou;
	vector<pair<int, int>> deplacementFou = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	for (const auto& direction : deplacementFou) {
		int j = 1;
		bool flag = true;
		while (j <= max(NOMBRE_COLONNE, NOMBRE_LIGNE) && flag) {
			ModeleEchec::Position positionFutur(position.obtenirX() + direction.first * j, 
				position.obtenirY() + direction.second * j);
			if (estHorsPlateau(positionFutur)) {
				flag = false;
				continue;
			}
			if (estCaseOccupe(positionFutur)) {
				if (estCaseJoueurAdvese(positionFutur, listePiecesAjoutee_[position.obtenirY()][position.obtenirX()]->obtenirCouleur())) {
					listeMouvementFou.push_back(positionFutur);
				}
				flag = false;
			}
			else {
				listeMouvementFou.push_back(positionFutur);
			}
			j++;
		}
	}
	return listeMouvementFou;
}


vector<ModeleEchec::Position>  ModeleEchec::Echiquier::trouverListeMouvementValideTour(ModeleEchec::Position position) {
	vector<ModeleEchec::Position> listeMouvementTour;
	vector<pair<int, int>> deplacementTour = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	ModeleEchec::Couleur joueur = listePiecesAjoutee_[position.obtenirY()][position.obtenirX()]->obtenirCouleur();
	for (const auto& direction : deplacementTour) {
		int j = 1;
		bool flag = true;
		while (j < max(NOMBRE_COLONNE, NOMBRE_LIGNE) && flag) {
			ModeleEchec::Position posFutur(position.obtenirX() + direction.first * j, 
				position.obtenirY() + direction.second * j);
			if (estHorsPlateau(posFutur)) {
				flag = false;
				continue;
			}
			if (estCaseOccupe(posFutur)) {
				if (estCaseJoueurAdvese(posFutur, joueur)) {
						listeMouvementTour.push_back(posFutur);
				}
				flag = false;
			}
			else {
				listeMouvementTour.push_back(posFutur);
			}
			j++;
		}
	}
	return listeMouvementTour;
}

vector<ModeleEchec::Position>   ModeleEchec::Echiquier::trouverListeMouvementValideRoi(ModeleEchec::Position position, bool justeMouvement = false) {
	vector<ModeleEchec::Position> listeMouvementRoi;
	vector<pair<int, int>> deplacementRoi = { {1,0}, {-1, 0}, {0, 1}, {0,-1}, {1,1}, {1, -1}, {-1, 1}, {-1,-1} };

	for (const auto& direction : deplacementRoi) {
		ModeleEchec::Position positionFutur(position.obtenirX() + direction.first, 
			position.obtenirY() + direction.second);
		if (!estHorsPlateau(positionFutur) && (!estCaseOccupe(positionFutur) || estCaseJoueurAdvese(positionFutur, listePiecesAjoutee_[position.obtenirY()][position.obtenirX()]->obtenirCouleur()))) {
			if (justeMouvement) {
				listeMouvementRoi.push_back(positionFutur);
			}
			else {
				ModeleEchec::Piece* piece = trouverPieceSurCase(position);
				ModeleEchec::Piece* pieceArrive = listePiecesAjoutee_[positionFutur.obtenirY()][positionFutur.obtenirX()];
				deplacerCaseACaseB(position, positionFutur);
				bool echec = estEchec(piece->obtenirCouleur());
				if (!echec) {
					listeMouvementRoi.push_back(positionFutur);
				}
				deplacerCaseACaseB(positionFutur, position);
				listePiecesAjoutee_[positionFutur.obtenirY()][positionFutur.obtenirX()] = pieceArrive;
			}
		}
	}
	return listeMouvementRoi;
}

bool  ModeleEchec::Echiquier::estPositionDansListe(const ModeleEchec::Position& position, const vector<ModeleEchec::Position>& listeMouvementActuel) {
	for (const auto& pos : listeMouvementActuel) {
		if (pos.obtenirX() == position.obtenirX() && pos.obtenirY() == position.obtenirY()) {
			return true;
		}
	}
	return false;
}

bool ModeleEchec::Echiquier::estEnEchecApresDeplacement(const ModeleEchec::Position& 
	positionDepart, const ModeleEchec::Position& positionArivee, ModeleEchec::Couleur joueurCouleur) {
	ModeleEchec::Piece* pieceCapture = trouverPieceSurCase(positionArivee);
	deplacerCaseACaseB(positionDepart, positionArivee);
	bool echec = estEchec(joueurCouleur);
	deplacerCaseACaseB(positionArivee, positionDepart);
	listePiecesAjoutee_[positionArivee.obtenirY()][positionArivee.obtenirX()] = pieceCapture;
	return echec;
}



