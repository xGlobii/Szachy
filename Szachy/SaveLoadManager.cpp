#include "SaveLoadManager.h"

SaveLoadManager::SaveLoadManager() : savePath("..\\saves\\save.txt"), loadPath("..\\saves\\save.txt")
{}

void SaveLoadManager::loadFromFile(Chessboard& chessboard)
{
	std::ifstream inputFile(loadPath);
	std::string linia;

	board = chessboard.getBoard();

	int i = 0;

	if (!inputFile.is_open())
	{
		std::cerr << "File with name '" + loadPath.filename().string() + "' was not found.";
	}
	else
	{
		while (std::getline(inputFile, linia))
		{
			std::stringstream ss;
			ss << linia;

			if (i < 8)
			{
				for (int x = 0; x < linia.length(); x++)
				{
					switch (linia[x])
					{
					case 'r':
						board[x][i] = new Rook(PieceColor::Black, PieceType::Rook);
						if ((x == 0 || x == 7) && i == 0)
							board[x][i]->setSpecialMove(true);
						else
							board[x][i]->setSpecialMove(false);
						break;
					case 'n':
						board[x][i] = new Knight(PieceColor::Black, PieceType::Knight);
						break;
					case 'b':
						board[x][i] = new Bishop(PieceColor::Black, PieceType::Bishop);
						break;
					case 'q':
						board[x][i] = new Queen(PieceColor::Black, PieceType::Queen);
						break;
					case 'k':
						board[x][i] = new King(PieceColor::Black, PieceType::King);
						if (x == 4 && i == 0)
							board[x][i]->setSpecialMove(true);
						else
							board[x][i]->setSpecialMove(false);
						break;
					case 'p':
						board[x][i] = new Pawn(PieceColor::Black, PieceType::Pawn);
						if (i == 1)
							board[x][i]->setSpecialMove(true);
						else
							board[x][i]->setSpecialMove(false);
						break;
					case 'R':
						board[x][i] = new Rook(PieceColor::White, PieceType::Rook);
						if ((x == 0 || x == 7) && i == 7)
							board[x][i]->setSpecialMove(true);
						else
							board[x][i]->setSpecialMove(false);
						break;
					case 'N':
						board[x][i] = new Knight(PieceColor::White, PieceType::Knight);
						break;
					case 'B':
						board[x][i] = new Bishop(PieceColor::White, PieceType::Bishop);
						break;
					case 'Q':
						board[x][i] = new Queen(PieceColor::White, PieceType::Queen);
						break;
					case 'K':
						board[x][i] = new King(PieceColor::White, PieceType::King);
						if (x == 4 && i == 7)
							board[x][i]->setSpecialMove(true);
						else
							board[x][i]->setSpecialMove(false);
						break;
					case 'P':
						board[x][i] = new Pawn(PieceColor::White, PieceType::Pawn);
						if (i == 6)
							board[x][i]->setSpecialMove(true);
						else
							board[x][i]->setSpecialMove(false);
						break;

					}
				}
				chessboard.setBoard(board);
			}

			if (i == 9)
				chessboard.setWhiteTimerValue(std::stoi(linia));
			else if (i == 10)
				chessboard.setBlackTimerValue(std::stoi(linia));
			else if (i == 12)
			{
				if (std::stoi(linia) == 1)
					chessboard.setTurn(true);
				else
					chessboard.setTurn(false);
			}
			else if (i == 14)
				chessboard.setPlayerPoints(PieceColor::White, std::stoi(linia));
			else if (i == 15)
				chessboard.setPlayerPoints(PieceColor::Black, std::stoi(linia));
			else if (i == 17)
				chessboard.setMovesWithoutTakes(std::stoi(linia));
			i++;
		}

		inputFile.close();
	}
}

void SaveLoadManager::saveToFile(Chessboard& chessboard)
{
	std::ofstream outputFile(savePath);
	std::string linia;

	int i = 0;

	board = chessboard.getBoard();

	if (!outputFile.is_open())
	{
		std::cerr << "File with name '" + savePath.filename().string() + "' was not found.";
	}
	else
	{
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				if (board[x][y] != nullptr)
				{
					PieceColor pc = board[x][y]->getColor();
					PieceType pt = board[x][y]->getPiece();
					if (pc == PieceColor::Black)
					{
						switch (pt)
						{
						case PieceType::King:
							outputFile << 'k';
							break;
						case PieceType::Queen:
							outputFile << 'q';
							break;
						case PieceType::Rook:
							outputFile << 'r';
							break;
						case PieceType::Bishop:
							outputFile << 'b';
							break;
						case PieceType::Knight:
							outputFile << 'n';
							break;
						case PieceType::Pawn:
							outputFile << 'p';
							break;
						default:
							break;
						}
					}
					else
					{
						switch (pt)
						{
						case PieceType::King:
							outputFile << 'K';
							break;
						case PieceType::Queen:
							outputFile << 'Q';
							break;
						case PieceType::Rook:
							outputFile << 'R';
							break;
						case PieceType::Bishop:
							outputFile << 'B';
							break;
						case PieceType::Knight:
							outputFile << 'N';
							break;
						case PieceType::Pawn:
							outputFile << 'P';
							break;
						default:
							break;
						}
					}
				}
				else
					outputFile << '#';
			}
			outputFile << std::endl;
		}

		outputFile << "\n" << chessboard.getWhiteTimerValue() << "\n" << chessboard.getBlackTimerValue() << "\n\n" << chessboard.getTurn() << "\n\n" << chessboard.getPlayersPoints(PieceColor::White)
			<< "\n" << chessboard.getPlayersPoints(PieceColor::Black) << "\n\n" << chessboard.getMovesWithoutTakes();

		outputFile.close();
	}
}