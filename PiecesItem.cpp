/**
* class pour la vue des pieces
* \file Echecs.cpp
* \date 3 mai 2024
* Cree le 8 avril 2024
*/
#include "PieceItems.hpp"
#include <QLabel>
#include <QApplication>
#include <QComboBox>
#include <QGraphicsProxyWidget>

#include <iostream>
#include <type_traits>
#include <QGraphicsEllipseItem>

const int DIMENSION_CASE = 85; 
const int DIMENSION_PIECE = DIMENSION_CASE - 10; 
const int LARGEUR_RECTANGLE = 360;
const int HAUTEUR_RECTANGLE = 960;
const int ESPACE = 50;
const int espaceEntreRectLar = 40;
const int espaceEntreRectHau = 35;
const int NOMBRE_CASE = 64;

InterfaceVueQt::PieceItem::PieceItem(ModeleEchec::TypePiece type, ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent) : piece_(new ModeleEchec::Piece(type, couleur)) {};

InterfaceVueQt::PieceItem::PieceItem(ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent) : piece_(piece) {};

InterfaceVueQt::RoiItem::RoiItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent) : PieceItem(ModeleEchec::TypePiece::ROI, couleur) {
	QPixmap imageFichier;
	if (couleur == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_king.png");
	else
		imageFichier = QPixmap("piecesChess/b_king.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
};

InterfaceVueQt::RoiItem::RoiItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent) : PieceItem(piece, parent) {
	QPixmap imageFichier;
	if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_king.png");
	else
		imageFichier = QPixmap("piecesChess/b_king.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);

};

InterfaceVueQt::ReineItem::ReineItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent) : PieceItem(ModeleEchec::TypePiece::REINE, couleur) {
	QPixmap imageFichier;
	if (couleur == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_queen.png");
	else
		imageFichier = QPixmap("piecesChess/b_queen.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
};

InterfaceVueQt::ReineItem::ReineItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent) : PieceItem(piece, parent) {
	QPixmap imageFichier;
	if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_queen.png");
	else
		imageFichier = QPixmap("piecesChess/b_queen.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);

};


InterfaceVueQt::CavalierItem::CavalierItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent) : PieceItem(ModeleEchec::TypePiece::CAVALIER, couleur) {
	QPixmap imageFichier;
	if (couleur == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_knight.png");
	else
		imageFichier = QPixmap("piecesChess/b_knight.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
};

InterfaceVueQt::CavalierItem::CavalierItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent) : PieceItem(piece, parent) {
	QPixmap imageFichier;
	if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_knight.png");
	else
		imageFichier = QPixmap("piecesChess/b_knight.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);

};

InterfaceVueQt::PionItem::PionItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent) : PieceItem(ModeleEchec::TypePiece::PION, couleur) {
	QPixmap imageFichier;
	if (couleur == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_knight.png");
	else
		imageFichier = QPixmap("piecesChess/b_knight.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
};

InterfaceVueQt::PionItem::PionItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent) : PieceItem(piece, parent) {
	QPixmap imageFichier;
	if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_pawn.png");
	else
		imageFichier = QPixmap("piecesChess/b_pawn.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);

};

InterfaceVueQt::TourItem::TourItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent) : PieceItem(ModeleEchec::TypePiece::TOUR, couleur, parent) {
	QPixmap imageFichier;
	if (couleur == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_tower.png");
	else
		imageFichier = QPixmap("piecesChess/b_tower.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
};

InterfaceVueQt::TourItem::TourItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent)
	: PieceItem(piece, parent) {
	QPixmap imageFichier;
	if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_rook.png");
	else
		imageFichier = QPixmap("piecesChess/b_rook.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
};

InterfaceVueQt::FouItem::FouItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent) : PieceItem(ModeleEchec::TypePiece::FOU, couleur, parent) {
	QPixmap imageFichier;
	if (couleur == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_bishop.png");
	else
		imageFichier = QPixmap("piecesChess/b_bishop.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
};

InterfaceVueQt::FouItem::FouItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent) : PieceItem(piece, parent) {
	QPixmap imageFichier;
	if (piece->obtenirCouleur() == ModeleEchec::Couleur::BLANC)
		imageFichier = QPixmap("piecesChess/w_bishop.png");
	else
		imageFichier = QPixmap("piecesChess/b_bishop.png");

	QPixmap imagePiece = imageFichier.scaled(DIMENSION_PIECE, DIMENSION_PIECE);
	setPixmap(imagePiece);
}