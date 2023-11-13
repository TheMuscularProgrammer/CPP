#pragma once
#include "GameBase.h"
#include <list>

class GameManager
{
private:
	std::list<GameBase*> lst;
	int			numOfGames = 0;
	std::list<GameBase*>::iterator currGame;
private:
	void clearScreen();
	int printMenu();
	bool checkGameToolIsValid(int row, int col);
	void deleteGame();
	void createNewGame();
	void addTheGame(gameType gType, bool bSetBoard);

	void moveToNextGame();
	void printGameStats();
	void printPossibleMoves(int row, int col);
	void printDangerousGameTools(int row, int col);
	void freeAllGames();
	void moveTool();
	gameType printGameTypeMenu();
	void inWhichGameTypeAmI();
	void printTitle(gameType gT);
	void saveCurrGame();
	void loadGame();
public:
	void menu();
	GameManager();
};