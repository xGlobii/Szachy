#include "Chessboard.h"

Chessboard::Chessboard()
{
	board.resize(8, std::vector<Piece*>(8, nullptr));

	for (int i = 0; i < board.size(); i++)
	{
		board[i][1] = new Pawn(-1, 1);
		board[i][6] = new Pawn(1, 1);
	}

	//Black
	board[0][0] = new Rook(-1, 4);
	board[1][0] = new Knight(-1, 2);
	board[2][0] = new Bishop(-1, 3);
	board[3][0] = new Queen(-1, 5);
	board[4][0] = new King(-1, 6);
	board[5][0] = new Bishop(-1, 3);
	board[6][0] = new Knight(-1, 2);
	board[7][0] = new Rook(-1, 4);

	//White
	board[0][7] = new Rook(1, 4);
	board[1][7] = new Knight(1, 2);
	board[2][7] = new Bishop(1, 3);
	board[3][7] = new Queen(1, 5);
	board[4][7] = new King(1, 6);
	board[5][7] = new Bishop(1, 3);
	board[6][7] = new Knight(1, 2);
	board[7][7] = new Rook(1, 4);
}

Chessboard::~Chessboard()
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board.size(); j++)
		{
			delete board[i][j];
		}
	}
}

void Chessboard::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board.size(); j++)
		{
			if (board[i][j] != nullptr)
				board[i][j]->draw(window, 100, sf::Vector2f(i * 100, j * 100));
		}
	}
}

void Chessboard::handleMouseClick(sf::RenderWindow& window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	int col = mousePosition.x / 100;
	int row = mousePosition.y / 100;

	if (board[col][row] != nullptr)
	{
		if (whiteTurn && board[col][row]->getColor() == "White")
			selectedPiecePosition = sf::Vector2i(col, row);
		else if (!whiteTurn && board[col][row]->getColor() == "Black")
			selectedPiecePosition = sf::Vector2i(col, row);
	}
	else
	{
		if (selectedPiecePosition != sf::Vector2i(-1, -1))
		{
			if (board[selectedPiecePosition.x][selectedPiecePosition.y]->move(board, selectedPiecePosition, sf::Vector2i(col, row)))
				whiteTurn = !whiteTurn;
			selectedPiecePosition = sf::Vector2i(-1, -1);
		}
	}
}