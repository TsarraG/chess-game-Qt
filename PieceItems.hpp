#pragma once
#include "Echecs.hpp"
#include "EchecsFenetre.hpp"
#pragma warning(push, 0)
#pragma pop()

namespace InterfaceVueQt {
	class PieceItem : public QGraphicsPixmapItem {
	public:
		PieceItem(ModeleEchec::TypePiece type, ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent = nullptr);
		PieceItem(ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent = nullptr);
		~PieceItem() { delete piece_; };

	private:
		ModeleEchec::Piece* piece_;

	};

	class RoiItem : public PieceItem {
	public:
		RoiItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent = nullptr);
		RoiItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent = nullptr);
		~RoiItem() {};
	};

	class ReineItem : public PieceItem {
	public:
		ReineItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent = nullptr);
		ReineItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent = nullptr);
		~ReineItem() {};
	};

	class CavalierItem : public PieceItem {
	public:
		CavalierItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent = nullptr);
		CavalierItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent = nullptr);
		~CavalierItem() {};
	};

	class PionItem : public PieceItem {
	public:
		PionItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent = nullptr);
		PionItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent = nullptr);
		~PionItem() {};
	};

	class TourItem : public PieceItem {
	public:
		TourItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent = nullptr);
		TourItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent = nullptr);
		//ModeleEchecs::Tour obtenirTour() { return tour_; }
		~TourItem() {};
	private:
		//ModeleEchecs::Tour tour_;
	};

	class FouItem : public PieceItem {
	public:
		FouItem(ModeleEchec::Couleur couleur, QGraphicsPixmapItem* parent = nullptr);
		FouItem(ModeleEchec::Couleur couleur, ModeleEchec::Piece* piece, QGraphicsPixmapItem* parent = nullptr);
		//ModeleEchecs::Fou obtenirFou() { return fou_; };
		~FouItem() {};
	private :
		//ModeleEchecs::Fou fou_;
	};
}