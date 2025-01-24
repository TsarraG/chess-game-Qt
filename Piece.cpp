/**
* class qui regroupe les pieces et la positions
* \file Echecs.cpp
* \date 3 mai 2024
* Cree le 8 avril 2024
*/

#include "Piece.hpp"

ModeleEchec::Piece::Piece(TypePiece type, Couleur couleur) : type_(type), couleur_(couleur), position_() {};

ModeleEchec::Piece::Piece(TypePiece type, Couleur couleur, Position position) : type_(type), couleur_(couleur), position_(position) {};

void ModeleEchec::Piece::changerPosition(Position position) {
	position_ = position;
}

ModeleEchec::Position::Position(unsigned x, unsigned y) : x_(x), y_(y) {}