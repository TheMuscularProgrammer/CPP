#include "GamesManager.h"
#include "Checkers.h"
#include "AmazonasGame.h"
#include "Chess.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

GameManager::GameManager()
{
	currGame = lst.end();
}

void GameManager::menu() {
	int selectedMenuOp;
	bool continueToPlay = true;
	int row, col;

	while (continueToPlay) {
		selectedMenuOp = printMenu();
		if (selectedMenuOp < 1 && selectedMenuOp > 10) {
			clearScreen();
			cout << endl << "-- Please choose by the menu! --" << endl << endl;
			continue;
		}
		if ((selectedMenuOp != 8 && selectedMenuOp != 1 && selectedMenuOp != 10) && numOfGames == 0) {
			clearScreen();
			cout << endl << "-- Please create new Game before --" << endl << endl;
			continue;
		}

		switch (selectedMenuOp)
		{
		case 8:
			continueToPlay = false;
			clearScreen();
			freeAllGames(); 
			break;
		case 1:
			createNewGame();
			numOfGames++;
			clearScreen();
			cout << endl << "-- A new Game has been created --" << endl;
			cout << "-- There are " << numOfGames << " games in the memory --" << endl << endl;
			inWhichGameTypeAmI();
			break;

		case 2:
			if (numOfGames > 1) {
				moveToNextGame();
				clearScreen();
				cout << endl << "-- Sucssesfuly moved to the next Game --" << endl << endl;
			}
			else {
				clearScreen();
				cout << endl << "-- There is only one Game --" << endl << endl;
			}
			break;
		case 3:
			deleteGame();
			numOfGames--;
			clearScreen();
			cout << endl << "-- The current Game has been deleted --" << endl;
			break;
		case 4:
			clearScreen();
			printGameStats();
			break;
		case 5:
			cout << endl << "Please enter the row and col of the Game tool: " << endl;
			cin >> row >> col;
			clearScreen();
			printPossibleMoves(row, col);
			break;
		case 6:
			cout << endl << "Please enter the row and col of the Game tool: " << endl;
			cin >> row >> col;
			clearScreen();
			if (checkGameToolIsValid(row, col))
				printDangerousGameTools(row, col);
			break;
		case 7:
			moveTool();
			break;
		case 9:
			try
			{
				saveCurrGame();
			}
			catch (runtime_error& error)
			{
				cout << "The current game was not saved to the file." << endl;
				cout << error.what() << endl;
			}
			break;
		case 10:
			try
			{
				loadGame();
				numOfGames++;
				clearScreen();
				cout << endl << "-- A new Game has been loaded from file --" << endl;
				cout << "-- There are " << numOfGames << " games in the memory --" << endl << endl;
				inWhichGameTypeAmI();
			}
			catch (runtime_error& error)
			{
				cout << "The new game was not created something was bad in the file." << endl;
				cout << error.what() << endl;
			}
			break;
		}
	}
	cout << "Hope you enjoyed!" << endl << endl;
}

void GameManager::saveCurrGame()
{
	string fileName;
	cout << "Please insert the file name:" << endl;
	cin >> fileName;

	ofstream o(fileName, ios::binary | ios::out);
	if (!o.good())
		cout << "Can't open the file. The game was not saved." << endl;
	else
	{
		int gameType = (int)(*currGame)->getGameType();
		o.write(reinterpret_cast<const char*>(&gameType), sizeof(int));
		(*currGame)->saveToFile(o);
	}
}

void GameManager::loadGame()
{
	string fileName;
	cout << "Please insert the file name:" << endl;
	cin >> fileName;

	ifstream ifs(fileName, ios::binary | ios::in);
	gameType gType;
	ifs.read(reinterpret_cast<char*>(&gType), sizeof(gType));
	addTheGame(gType, false);
	(*currGame)->readFromFile(ifs);
}

void GameManager::printTitle(gameType gT) {
	if (gT == gameType::amazons)
		cout << "Amazons";
	else if (gT == gameType::checkers)
		cout << "Checkers";
	else
		cout << "Chess";
}

void GameManager::inWhichGameTypeAmI() {
	cout << "-- You're playing ";
	printTitle((*currGame)->getGameType());
	cout << " --" << endl;
}

void GameManager::moveTool()
{
	MoveResult res = (*currGame)->move();

	//check if winner
	if ((*currGame)->getGameOn() == false)
	{
		cout << "the winner is player:" << (int)(*currGame)->getWinner() << endl;
	}
}

int GameManager::printMenu() {
	int userChoise;

	cout << "Please choose:" << endl;
	cout << endl;
	cout << " 1.  New Game" << endl;
	cout << " 2.  Move to the next Game" << endl;
	cout << " 3.  Delete current Game" << endl;
	cout << " 4.  Show Game stats" << endl;
	cout << " 5.  Show GameTool's possible move" << endl;
	cout << " 6.  Show GameTool's risks" << endl;
	cout << " 7.  Make a move in the Game" << endl;
	cout << " 8.  Exit" << endl;
	cout << " 9.  Save curr game" << endl;
	cout << " 10. Load game" << endl;
	cout << endl;

	cin >> userChoise;
	return userChoise;
}

gameType GameManager::printGameTypeMenu() {
	int userChoise;
	clearScreen();
	cout << endl << " Please choose game type:" << endl << endl;
	cout << " 1. Checkers" << endl;
	cout << " 2. Amazons" << endl;
	cout << " 3. Chess" << endl << endl;

	cin >> userChoise;
	while (userChoise < 1 || userChoise > 3)
	{
		clearScreen();
		cout << "Wrong game type." << endl;
		cout << endl << " Please choose game type:" << endl << endl;
		cout << " 1. Checkers" << endl;
		cout << " 2. Amazons" << endl;
		cout << " 3. Chess" << endl << endl;
		cin >> userChoise;
	}

	if (userChoise == 1)
		return gameType::checkers;
	else if (userChoise == 2)
		return gameType::amazons;
	else
		return gameType::chess;
} 

void GameManager::createNewGame() {
	gameType gType = printGameTypeMenu();
	addTheGame(gType,true);
}

void GameManager::addTheGame(gameType gType, bool bSetBoard)
{
	//LstGameNode* newGameNode = new LstGameNode;

	GameBase* newGame = nullptr;
	if (gType == gameType::amazons)
		newGame = new AmazonasGame(bSetBoard);
	else if (gType == gameType::chess)
		newGame = new Chess(bSetBoard);
	else if (gType == gameType::checkers)
		newGame = new Checkers(bSetBoard);
	else
	{
		//delete newGameNode;
		throw runtime_error("the game type is wrong");
	}


	lst.push_back(newGame);
	currGame = lst.end();
	--currGame;
}

void GameManager::moveToNextGame() {
	++currGame;
	if (currGame == lst.end())
		currGame = lst.begin();
}

void GameManager::clearScreen() {
	system("CLS"); 
}

void GameManager::deleteGame() {

	std::list<GameBase*>::iterator gameToDelete = currGame;
	
	delete (*gameToDelete);

	++currGame;
	lst.erase(gameToDelete);

	if (currGame == lst.end())
	{
		currGame = lst.begin();
	}
}

bool GameManager::checkGameToolIsValid(int row, int col) {
	if((*currGame)->isValidPos(row,col) == false) {
		cout << endl << "-- Not in board range! --" << endl;
		return false;
	}
	else if ((*currGame)->checkIfEmptyPanel(row, col)) {
		cout << endl << "-- The panel is empty! --" << endl;
		return false;
	}
	else
		return true;
}

void GameManager::printGameStats() {
	if ((*currGame)->getGameOn() == true)
	{
		cout << "Game is on. Curr player is: " << (int)(*currGame)->getTurn() << endl;
	}
	else
	{
		cout << "This game is over. The Winner is: Player " << (int)(*currGame)->getWinner() << endl;
	}

	(*currGame)->printBoard();
}

void GameManager::printPossibleMoves(int row, int col) {
	int size;
	if (checkGameToolIsValid(row, col) == false)
	{
		cout << "This position is not valid" << endl;
	}
	else
	{
		Position* arr = (*currGame)->getGameToolInPosition(row, col).getPossibleMoves(size);
		if (size == 0)
		{
			cout << "no posible moves for this tile" << endl;
		}
		else
		{
			cout << "you can move to these locations:" << endl;

			for (int i = 0; i < size; i++)
			{
				cout << "( " << arr[i].r << " , " << arr[i].c << " )" << endl;
			}
			delete[] arr;
		}
	}
}

void GameManager::printDangerousGameTools(int row, int col) {
	//move to class
	int size;
	Position* arr = (*currGame)->getGameToolInPosition(row, col).getRisks(size);
	if (size == 0)
	{
		cout << "no danger from this square" << endl;
	}
	else
	{
		cout << "this is the danger moves! : " << endl;

		for (int i = 0; i < size; i++)
		{
			cout << "(" << arr[i].r << " ," << arr[i].c << ")" << endl;
		}
		delete[] arr;
	}
}

void GameManager::freeAllGames() {
	std::list<GameBase*>::iterator curr;

	for (curr = lst.begin(); curr != lst.end(); ++curr)
	{
		delete (*curr);
	}
}
