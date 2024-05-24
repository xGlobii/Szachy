#include "Piece.h"

std::string getStringColor(const Color color)
{
	return (color == Color::Black ? "Black" : "White");
}

Piece::Piece(Color color, PieceType pType) : color(color), pType(pType)
{
	std::string colorStr = (getStringColor(color));
	std::string typeStr = (getStringType(pType));

	texture.loadFromFile("..\\images\\" + colorStr + typeStr + ".png");
}

Piece::~Piece() {}

void Piece::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	sf::Sprite piece;

	float scaleX = size / texture.getSize().x;
	float scaleY = size / texture.getSize().y;

	piece.setScale(0.1, 0.1);
	piece.setTexture(texture);
	piece.setPosition(position);
	window.draw(piece);
}

bool Piece::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(board, piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

std::string getStringType(const PieceType pType)
{
	switch (pType)
	{
	case PieceType::King:
		return "King";
		break;
	case PieceType::Queen:
		return "Queen";
		break;
	case PieceType::Rook:
		return "Rook";
		break;
	case PieceType::Bishop:
		return "Bishop";
		break;
	case PieceType::Knight:
		return "Knight";
		break;
	case PieceType::Pawn:
		return "Pawn";
		break;
	}
}