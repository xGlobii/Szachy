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
#include "ChessboardChecking.h"

struct MoveRecord 
{
	std::string notation;

	bool isCastle;
	bool isCheck;
	bool isCheckmate;
	bool isPromoting;
	bool isTaking;
};


class Chessboard
{
private:
	std::vector<std::vector<Piece*>> board;
	std::vector<std::string> amovesHistory;
	std::vector<std::string> moveHistory;

	ChessboardChecking checking;

	sf::Vector2i selectedPiecePosition = sf::Vector2i(-1, -1);
	sf::Vector2i promotionPosition;
	sf::Vector2i promotionPositionFrom;
	sf::Vector2f offset;

	std::mutex mtx;

	MoveRecord record;

	std::string endGameType;

	int tileSize;
	int whitePoints;
	int blackPoints;
	int movesWithoutTake;

	bool promoting;
	bool checkmate;
	bool whiteTurn;
	bool startTimer;
	bool stopTimers;

public:
	Chessboard();
	~Chessboard();

	int getPlayersPoints(PieceColor pc);
	int getMovesWithoutTakes();

	sf::Vector2i getPromotionPosition();

	std::string generateAlgebricNotation(sf::Vector2i from, sf::Vector2i to, PieceType pt);
	std::string pieceTypeToSymbol(PieceType pt);
	std::string getEndGameType();
	
	std::vector<std::vector<Piece*>> getBoard();
	std::vector<std::string> movesHistory;
	std::vector<std::string> getMovesHistory();
	std::vector<std::string> getMoves();

	void drawLastMoves(sf::RenderWindow& window);
	void drawPromotionIcons(sf::RenderWindow& window, sf::Vector2i position);
	void setGameStatus(bool status);
	void setMoves(std::vector<std::string> moves);
	void handlePromotionClick(sf::Vector2i mousePosition);
	void draw(sf::RenderWindow& window);
	void handleMouseClick(sf::RenderWindow& window);
	void setPlayerPoints(PieceColor pc, int value);
	void promotePawn(sf::Vector2i position, sf::RenderWindow& window);
	void setBoard(std::vector<std::vector<Piece*>>& board);
	void initializeBoard();
	void setTurn(bool whiteTurn);
	void pieceTakes(sf::Vector2i to, sf::RenderWindow& window);
	void pieceMoves(sf::Vector2i to, sf::RenderWindow& window);
	void setMovesWithoutTakes(int moves);

	bool getTurn();
	bool getTimerStatus();
	bool canCastle(sf::Vector2i boardPosition);
	bool getGameStatus();
	bool getPromoting();
};

#endif