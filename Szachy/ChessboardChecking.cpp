#include "ChessboardChecking.h"

ChessboardChecking::ChessboardChecking(std::vector<std::vector<Piece*>>& board) : board(board) {}

bool ChessboardChecking::isCheckmate(PieceColor pc, sf::Vector2i attackerPosition)
{
	if (!isKingInCheck(pc))
		return false;

	sf::Vector2i kingPosition = findKing(pc);

	std::future<bool> f1 = std::async(&ChessboardChecking::checkIfKingCanEscape, this, kingPosition, attackerPosition, pc);
	std::future<bool> f2 = std::async(&ChessboardChecking::checkIfThereMultipleChecks, this, kingPosition, pc);
	std::future<bool> f3 = std::async(&ChessboardChecking::checkIfCanBlockMate, this, kingPosition, attackerPosition, pc);

	if (!f1.get() || f2.get() || !f3.get())
		return false;
	else
	{
		endGameType = "Checkmate";
		return true;
	}
}

bool ChessboardChecking::checkIfKingCanEscape(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc)
{
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (dx == 0 && dy == 0)
				continue;

			int newX = kingPosition.x + dx;
			int newY = kingPosition.y + dy;

			if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
			{
				if (board[newX][newY] == nullptr ||
					(board[newX][newY]->getColor() != pc && board[newX][newY]->takes(sf::Vector2i(newX, newY), kingPosition, board)))
				{
					Piece* tempPiece = board[newX][newY];
					bool tempSpecial = board[kingPosition.x][kingPosition.y]->getSpecialMoveStatus();

					board[newX][newY] = board[kingPosition.x][kingPosition.y];
					board[kingPosition.x][kingPosition.y] = nullptr;

					if (!isKingInCheck(pc))
					{
						board[kingPosition.x][kingPosition.y] = std::move(board[newX][newY]);
						board[newX][newY] = tempPiece;
						board[kingPosition.x][kingPosition.y]->setSpecialMove(tempSpecial);
						return false;
					}

					board[kingPosition.x][kingPosition.y] = std::move(board[newX][newY]);
					board[newX][newY] = tempPiece;
					board[kingPosition.x][kingPosition.y]->setSpecialMove(tempSpecial);
				}
			}

		}
	}

	return true;
}

bool ChessboardChecking::checkIfThereMultipleChecks(sf::Vector2i kingPosition, PieceColor pc)
{
	std::vector<sf::Vector2i> attackers;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (board[x][y] != nullptr && board[x][y]->getColor() != pc && board[x][y]->takes(sf::Vector2i(x, y), kingPosition, board))
				attackers.push_back(sf::Vector2i(x, y));
		}
	}

	if (attackers.size() > 1)
		return true;
	else
		return false;
}

bool ChessboardChecking::checkIfCanBlockMate(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc)
{
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (board[x][y] != nullptr && board[x][y]->getColor() == pc && board[x][y]->getPiece() != PieceType::King)
			{
				if (board[x][y]->takes(sf::Vector2i(x, y), attackerPosition, board))
					return false;

				int dx = attackerPosition.x - kingPosition.x;
				int dy = attackerPosition.y - kingPosition.y;

				int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
				int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

				for (int i = kingPosition.x + stepX, j = kingPosition.y + stepY;
					(stepX == 0 || i != attackerPosition.x) && (stepY == 0 || j != attackerPosition.y);
					i += stepX, j += stepY)
				{
					if (board[x][y]->possibleMove(board, sf::Vector2i(x, y), sf::Vector2i(i, j)))
						return false;
				}
			}
		}
	}
	return true;
}


sf::Vector2i ChessboardChecking::findKing(const PieceColor pc)
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y] != nullptr && board[x][y]->getPiece() == PieceType::King && board[x][y]->getColor() == pc)				//Searching for king
				return sf::Vector2i(x, y);																							//Returning position of king
		}
	}
	return sf::Vector2i(-1, -1);																									//King not found
}

bool ChessboardChecking::isKingInCheck(const PieceColor kingColor)
{
	sf::Vector2i kingPosition = findKing(kingColor);
	if (kingPosition == sf::Vector2i(-1, -1))				//King not found
		return false;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y] != nullptr && board[x][y]->getColor() != kingColor)				//Searching for 'enemy' pieces on board
			{
				if (board[x][y]->takes(sf::Vector2i(x, y), kingPosition, board))		//Checking if king is in direct attack
					return true;
			}
		}
	}

	return false;
}

bool ChessboardChecking::isDraw(PieceColor pc, bool whiteturn, int movesWithoutTakes)
{
	if (isKingInCheck(pc))
		return false;
	if (movesWithoutTakes >= 100)
	{
		endGameType = "Draw\n50-moves rule";
		return true;
	}

	updatePositionHistory(whiteturn);
	if (positionHistory[generatePositionKey(whiteturn)] >= 6) {
		endGameType = "Draw\nThreefold repetition";
		return true;
	}

	sf::Vector2i kingPosition = findKing(pc);

	int whiteKnights = 0, whiteBishops = 0, whiteOthers = 0;
	int blackKnights = 0, blackBishops = 0, blackOthers = 0;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (board[x][y] != nullptr && board[x][y]->getPiece() != PieceType::King)
			{
				PieceType type = board[x][y]->getPiece();
				PieceColor color = board[x][y]->getColor();

				if (color == PieceColor::White)
				{
					if (type == PieceType::Knight)
						whiteKnights++;
					else if (type == PieceType::Bishop)
						whiteBishops++;
					else
						whiteOthers++;
				}
				else
				{
					if (type == PieceType::Knight)
						blackKnights++;
					else if (type == PieceType::Bishop)
						blackBishops++;
					else
						blackOthers++;
				}
			}
		}
	}

	if (whiteOthers == 0 && blackOthers == 0)
	{
		if (whiteBishops == 0 && blackBishops == 0)
		{
			if (whiteKnights <= 1 && blackKnights <= 1)
			{
				endGameType = "Draw\nInsufficient material";
				return true;
			}
		}
		else if (whiteBishops == 1 && blackBishops == 0 && whiteKnights == 0 && blackKnights == 0)
		{
			endGameType = "Draw\nInsufficient material";
			return true;
		}
		else if (whiteBishops == 0 && blackBishops == 1 && whiteKnights == 0 && blackKnights == 0)
		{
			endGameType = "Draw\nInsufficient material";
			return true;
		}
	}

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (board[x][y] != nullptr && board[x][y]->getPiece() == PieceType::Pawn && board[x][y]->getColor() == pc)
			{
				if (board[x][y]->getSpecialMoveStatus())
				{
					if (board[x][y]->possibleMove(board, sf::Vector2i(x, y), sf::Vector2i(x, y + (pc == PieceColor::White ? 2 : -2))))
					{
						return false;
					}
				}
				else if (board[x][y]->possibleMove(board, sf::Vector2i(x, y), sf::Vector2i(x, y + (pc == PieceColor::White ? 1 : -1))))
				{
					return false;
				}
				if (x - 1 > 0 && x + 1 < 7)
				{
					if (board[x][y]->takes(sf::Vector2i(x, y), sf::Vector2i(x - 1, y + (pc == PieceColor::White ? 1 : -1)), board))
					{
						return false;
					}
					if (board[x][y]->takes(sf::Vector2i(x, y), sf::Vector2i(x + 1, y + (pc == PieceColor::White ? 1 : -1)), board))
					{
						return false;
					}
				}
			}
			else if (board[x][y] != nullptr && board[x][y]->getPiece() != PieceType::Pawn && board[x][y]->getPiece() != PieceType::King && board[x][y]->getColor() == pc)
			{
				return false;
			}
		}
	}

	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (dx == 0 && dy == 0)
				continue;

			int newX = kingPosition.x + dx;
			int newY = kingPosition.y + dy;

			if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
			{
				if (board[newX][newY] == nullptr ||
					(board[newX][newY]->getColor() != pc && board[newX][newY]->takes(sf::Vector2i(newX, newY), kingPosition, board)))
				{
					Piece* tempPiece = board[newX][newY];
					bool tempSpecial = board[kingPosition.x][kingPosition.y]->getSpecialMoveStatus();

					board[newX][newY] = board[kingPosition.x][kingPosition.y];
					board[kingPosition.x][kingPosition.y] = nullptr;

					if (!isKingInCheck(pc))
					{
						board[kingPosition.x][kingPosition.y] = std::move(board[newX][newY]);
						board[newX][newY] = tempPiece;
						board[kingPosition.x][kingPosition.y]->setSpecialMove(tempSpecial);
						return false;
					}

					board[kingPosition.x][kingPosition.y] = std::move(board[newX][newY]);
					board[newX][newY] = tempPiece;
					board[kingPosition.x][kingPosition.y]->setSpecialMove(tempSpecial);
				}
			}

		}
	}

	endGameType = "Draw\nStalemate";
	return true;
}

void ChessboardChecking::updatePositionHistory(bool whiteturn)
{
	std::string key = generatePositionKey(whiteturn);
	positionHistory[key]++;
}

std::string ChessboardChecking::generatePositionKey(bool whiteTurn)
{
	std::stringstream ss;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (board[x][y] == nullptr)
			{
				ss << ".";
			}
			else
			{
				char pieceChar;
				PieceType type = board[x][y]->getPiece();
				PieceColor color = board[x][y]->getColor();

				switch (type) {
				case PieceType::King:   pieceChar = 'K'; break;
				case PieceType::Queen:  pieceChar = 'Q'; break;
				case PieceType::Rook:   pieceChar = 'R'; break;
				case PieceType::Bishop: pieceChar = 'B'; break;
				case PieceType::Knight: pieceChar = 'N'; break;
				case PieceType::Pawn:   pieceChar = 'P'; break;
				}

				ss << (color == PieceColor::White ? pieceChar : std::tolower(pieceChar));
			}
		}
		ss << "/";
	}
	ss << (whiteTurn ? "w" : "b");
	return ss.str();
}

std::string ChessboardChecking::getEndgametype()
{
	return endGameType;
}