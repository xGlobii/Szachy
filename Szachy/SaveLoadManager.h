#ifndef SLM_H
#define SLM_H

#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Chessboard.h"
#include "Button.h"

namespace fs = std::filesystem;

class SaveLoadManager
{
	fs::path savePath;
	fs::path loadPath;
	fs::path historySavePath;
	fs::path historyLoadPath;

	std::vector<std::vector<Piece*>> board;

public:
	SaveLoadManager();

	void loadFromFile(Chessboard& chessboard, Timer& timerWhite, Timer& timerBlack, InputBox& input1, InputBox& input2);
	void saveToFile(Chessboard& chessboard, Timer& timerWhite, Timer& timerBlack, InputBox& input1, InputBox& input2);

	void saveGameHistory(std::vector<std::string>& gameHistory);
	void loadGameHistory(std::vector<std::string>& gameHistory);
};

#endif