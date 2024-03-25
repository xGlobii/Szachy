#include "Piece.h"

Piece::Piece(const int _color, const int _pType)
{
	color = static_cast<Color>(_color);
	pType = static_cast<PieceType>(_pType);


	colorStr = getColor(color);
	typeStr = getPiece(pType);

	texture.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\" + colorStr + typeStr + ".png");
}

Piece::~Piece() {}

void Piece::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	sf::Sprite piece;
	//piece.setSize(sf::Vector2f(size, size));
	//sf::Vector2f textureSize;

	float scaleX = size / texture.getSize().x;
	float scaleY = size / texture.getSize().y;

	// Ustawienie skali sprite'a
	piece.setScale(scaleX, scaleY);
	piece.setOrigin(piece.getLocalBounds().width/2, piece.getLocalBounds().height/2);

	//textureSize = piece.getScale();
	piece.setScale(0.1, 0.1);
	piece.setTexture(texture);
	piece.setPosition(position);
	window.draw(piece);
}

King::King(const int _color, const int _pType) : Piece(_color, _pType) {}
King::~King() {}

void King::move() const
{

}

void King::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

Queen::Queen(const int _color, const int _pType) : Piece(_color, _pType) {}
Queen::~Queen() {}

void Queen::move() const
{

}

void Queen::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

Rook::Rook(const int _color, const int _pType) : Piece(_color, _pType) {}
Rook::~Rook() {}

void Rook::move() const
{

}

void Rook::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

Bishop::Bishop(const int _color, const int _pType) : Piece(_color, _pType) {}
Bishop::~Bishop() {}

void Bishop::move() const
{

}

void Bishop::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

Knight::Knight(const int _color, const int _pType) : Piece(_color, _pType) {}
Knight::~Knight() {}

void Knight::move() const
{

}

void Knight::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

Pawn::Pawn(const int _color, const int _pType) : Piece(_color, _pType) {}
Pawn::~Pawn() {}

void Pawn::move() const
{

}

void Pawn::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}