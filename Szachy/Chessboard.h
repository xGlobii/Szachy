#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <future>

#include "Piece.h"
#include "Queen.h"
#include "King.h"
#include "Rook.h"
#include "Knight.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Timer.h"

struct MoveRecord 
{
	std::string notation;
	bool isCastle;
	bool isCheck;
};


class Chessboard
{
private:
	std::vector<std::vector<Piece*>> board;
	sf::Vector2i selectedPiecePosition = sf::Vector2i(-1, -1);

	std::unordered_map<std::string, int> positionHistory;
	std::vector<std::string> amovesHistory;



	sf::Vector2i draggingPiecePosition = sf::Vector2i(-1, -1);
	bool isDragging = false;
	sf::Vector2f draggingOffset;

	Piece* draggedPiece;
	sf::Vector2i draggedPieceOriginalPosition;



	sf::Vector2i promotionPosition;
	bool promoting;



	std::mutex mtx;

	MoveRecord record;

	int tileSize;

	int whitePoints;
	int blackPoints;

	int movesWithoutTake;

	bool whiteTurn;
	bool startTimer;

	bool isCastle;
	bool isCheck;
	bool takes;
	bool checkmate;

	bool stopTimers;

	sf::Vector2f offset;

	std::string endGameType;

	std::vector<std::string> moveHistory;

public:
	Chessboard();
	~Chessboard();

	void draw(sf::RenderWindow& window);
	void handleMouseClick(sf::RenderWindow& window);
	int getPlayersPoints(PieceColor pc);
	void setPlayerPoints(PieceColor pc, int value);

	sf::Vector2i findKing(const PieceColor pc);
	bool isKingInCheck(const PieceColor kingColor);

	void promotePawn(sf::Vector2i position, sf::RenderWindow& window);

	std::vector<std::string> movesHistory;

	std::vector<std::vector<Piece*>> getBoard();

	void drawPointsPlace(sf::RenderWindow& window, const int points, sf::Vector2f position);

	void setBoard(std::vector<std::vector<Piece*>>& board);

	void initializeBoard();

	bool getTurn();
	void setTurn(bool whiteTurn);

	bool getTimerStatus();

	void pieceTakes(sf::Vector2i to, sf::RenderWindow& window);
	void pieceMoves(sf::Vector2i to, sf::RenderWindow& window);


	bool canCastle(sf::Vector2i boardPosition);

	int getMovesWithoutTakes();
	void setMovesWithoutTakes(int moves);

	std::string generateAlgebricNotation(sf::Vector2i from, sf::Vector2i to, PieceType pt);
	std::string pieceTypeToSymbol(PieceType pt);

	bool isCheckmate(PieceColor pc, sf::Vector2i attackerPosition);

	void drawLastMoves(sf::RenderWindow& window);

	void drawPromotionIcons(sf::RenderWindow& window, sf::Vector2i position);

	bool checkIfCanBlockMate(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc);
	bool checkIfThereMultipleChecks(sf::Vector2i kingPosition, PieceColor pc);
	bool checkIfKingCanEscape(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc);

	bool getGameStatus();
	void setGameStatus(bool status);

	bool isDraw(PieceColor pc);

	std::string getEndGameType();

	std::vector<std::string> getMoves();
	void setMoves(std::vector<std::string> moves);



	std::vector<std::string> getMovesHistory();
	void updatePositionHistory();
	std::string generatePositionKey();


	void handlePromotionClick(sf::Vector2i mousePosition);

	bool getPromoting();
	sf::Vector2i getPromotionPosition();
};

#endif