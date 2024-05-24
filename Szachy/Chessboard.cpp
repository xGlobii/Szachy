#include "Chessboard.h"

Chessboard::Chessboard()
{
	board.resize(8, std::vector<Piece*>(8, nullptr));

	for (int i = 0; i < board.size(); i++)
	{
		board[i][1] = new Pawn(Color::Black, PieceType::Pawn);
		board[i][6] = new Pawn(Color::White, PieceType::Pawn);
	}

	//Black
	board[0][0] = new Rook(Color::Black, PieceType::Rook);
	board[1][0] = new Knight(Color::Black, PieceType::Knight);
	board[2][0] = new Bishop(Color::Black, PieceType::Bishop);
	board[3][0] = new Queen(Color::Black, PieceType::Queen);
	board[4][0] = new King(Color::Black, PieceType::King);
	board[5][0] = new Bishop(Color::Black, PieceType::Bishop);
	board[6][0] = new Knight(Color::Black, PieceType::Knight);
	board[7][0] = new Rook(Color::Black, PieceType::Rook);

	//White
	board[0][7] = new Rook(Color::White, PieceType::Rook);
	board[1][7] = new Knight(Color::White, PieceType::Knight);
	board[2][7] = new Bishop(Color::White, PieceType::Bishop);
	board[3][7] = new Queen(Color::White, PieceType::Queen);
	board[4][7] = new King(Color::White, PieceType::King);
	board[5][7] = new Bishop(Color::White, PieceType::Bishop);
	board[6][7] = new Knight(Color::White, PieceType::Knight);
	board[7][7] = new Rook(Color::White, PieceType::Rook);
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
				board[i][j]->draw(window, 100, sf::Vector2f(i * tileSize, j * tileSize));
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
		if (selectedPiecePosition != sf::Vector2i(-1, -1))
		{
			if ((whiteTurn && board[col][row]->getColor() == Color::Black) || (!whiteTurn && board[col][row]->getColor() == Color::White))
			{
				if (board[selectedPiecePosition.x][selectedPiecePosition.y]->takes(selectedPiecePosition, sf::Vector2i(col, row), board))
					whiteTurn = !whiteTurn;
				selectedPiecePosition = sf::Vector2i(-1, -1);
			}
			else
				selectedPiecePosition = sf::Vector2i(-1, -1);
		}
		else
		{
			if (whiteTurn && board[col][row]->getColor() == Color::White)				//Checking if white tries to move white piece
				selectedPiecePosition = sf::Vector2i(col, row);
			else if (!whiteTurn && board[col][row]->getColor() == Color::Black)			//Checking if black tries to move black piece
				selectedPiecePosition = sf::Vector2i(col, row);

		}
	}
	else if (board[col][row] == nullptr)
	{
		if (selectedPiecePosition != sf::Vector2i(-1, -1))
		{
			if (board[selectedPiecePosition.x][selectedPiecePosition.y]->move(board, selectedPiecePosition, sf::Vector2i(col, row)))
				whiteTurn = !whiteTurn;
			selectedPiecePosition = sf::Vector2i(-1, -1);
		}
	}
}