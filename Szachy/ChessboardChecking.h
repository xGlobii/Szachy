#ifndef CHESS_CHECK_H
#define CHESS_CHECK_H

#include <vector>
#include <future>
#include <sstream>
#include <unordered_map>

#include "Piece.h"
#include "Pawn.h"
#include "Knight.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"

class ChessboardChecking
{
	std::vector<std::vector<Piece*>>& board;

	std::string endGameType;

	std::unordered_map<std::string, int> positionHistory;

public:
	ChessboardChecking() = default;
	ChessboardChecking(std::vector<std::vector<Piece*>>& board);

	bool isCheckmate(PieceColor pc, sf::Vector2i attackerPosition);
	bool checkIfCanBlockMate(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc);
	bool checkIfThereMultipleChecks(sf::Vector2i kingPosition, PieceColor pc);
	bool checkIfKingCanEscape(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc);

	sf::Vector2i findKing(const PieceColor pc);
	bool isKingInCheck(const PieceColor kingColor);
	bool isDraw(PieceColor pc, bool whiteturn, int movesWithoutTakes);

	void updatePositionHistory(bool whiteturn);
	std::string generatePositionKey(bool whiteturn);

	std::string getEndgametype();
};


#endif