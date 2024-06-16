#ifndef SLM_H
#define SLM_H

#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Chessboard.h"

namespace fs = std::filesystem;

class SaveLoadManager
{
	fs::path savePath;
	fs::path loadPath;

	std::vector<std::vector<Piece*>> board;

public:
	SaveLoadManager();

	void loadFromFile(Chessboard& chessboard);
	void saveToFile(Chessboard& chessboard);
};

#endif