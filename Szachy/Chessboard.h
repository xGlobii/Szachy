#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <thread>
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

	std::mutex mtx;

	MoveRecord record;

	std::atomic<bool> whitePauseFlag;
	std::atomic<bool> blackPauseFlag;
	std::atomic<bool> stopFlag;

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

	sf::Vector2f offset;

	std::vector<std::string> moveHistory;

	std::thread whiteClockThread;
	std::thread blackClockThread;

	Timer blackTimer;
	Timer whiteTimer;

public:
	Chessboard();
	~Chessboard();

	void draw(sf::RenderWindow& window);
	void handleMouseClick(sf::RenderWindow& window);
	int getPlayersPoints(PieceColor pc);
	void setPlayerPoints(PieceColor pc, int value);

	sf::Vector2i findKing(const PieceColor pc);
	bool isKingInCheck(const PieceColor kingColor);

	void startTimers();
	void stopTimers();

	void promotePawn(sf::Vector2i position, sf::RenderWindow& window);

	std::vector<std::string> movesHistory;

	std::vector<std::vector<Piece*>> getBoard();

	void drawPointsPlace(sf::RenderWindow& window, const int points, sf::Vector2f position);

	void setTimers(sf::Vector2f position, sf::RenderWindow& window, const PieceColor pc);
	void setBoard(std::vector<std::vector<Piece*>>& board);

	void initializeBoard();

	bool getTurn();
	void setTurn(bool whiteTurn);

	int getWhiteTimerValue();
	int getBlackTimerValue();

	void setWhiteTimerValue(const int time);
	void setBlackTimerValue(const int time);

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
};

#endif