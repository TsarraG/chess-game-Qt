#pragma once

namespace ModeleEchec {

	enum class TypePiece {
		ROI = 0,
		REINE = 1,
		TOUR = 2,
		FOU = 3,
		CAVALIER = 4,
		PION = 5,
	};

	enum class Couleur {
		BLANC = 0,
		NOIR = 1,
	};

	class Position {

	public:
		Position() = default;
		Position(unsigned x, unsigned y);


		int obtenirX() const { return x_; };
		int obtenirY() const { return y_; };

		bool operator==(const Position& autre) const {
			return x_ == autre.x_ && y_ == autre.y_;
		}

	private:
		int x_ = -1;
		int y_ = -1;
	};

	class Piece {
	public:
		Piece(TypePiece type, Couleur couleur);
		Piece(TypePiece type, Couleur couleur, Position position);

		void changerPosition(Position position);
		TypePiece obtenirType() const { return type_; };
		Couleur obtenirCouleur() const { return couleur_; };
		Position obtenirPosition() const { return position_; };
		int obtenirPositionX() const { return position_.obtenirX(); };
		int obtenirPositionY() const { return position_.obtenirY(); };

	private:
		TypePiece type_;
		Couleur couleur_;
		Position position_;
		//int nombrePiece_; //Ca va ou
	};


}


