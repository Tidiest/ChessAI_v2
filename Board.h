#pragma once
#include <iostream>
#include <map>
#include <array>
#include <vector>
#include "BoardUtility.h"

class Board
{
public:
	Board();

	int* getBoard() { return this->board; };
	void loadPositionFromFEN(std::string fen);

	int getColorToMove() { return this->colorToMove; };
	void makeMove(BoardUtility::Move& move);
	void unmakeMove(BoardUtility::Move& move);

	std::array<std::vector<int>, 2>& getAttackedSquares() { return this->attackedSquares; }
	std::array<std::vector<BoardUtility::Pin>, 2>& getPins() { return this->pins; }


	std::array<bool, 4> castlingRight; //white|kingside, white|queenside, black|kingside, black|queenside
	std::array<int, 2> kingMoves; //white, black
	std::array<int, 4> rookMoves; //white|kingside, white|queenside, black|kingside, black|queenside
	std::array<int, 4> rookSquares;

private:
	int board[64];
	int colorToMove;
	std::array<std::vector<int>, 2> pawns; //0-white, 1-black | square index
	std::array<std::vector<int>, 2> knights; //0-white, 1-black | square index
	std::array<std::vector<int>, 2> bishops; //0-white, 1-black | square index
	std::array<std::vector<int>, 2> rooks; //0-white, 1-black | square index
	std::array<std::vector<int>, 2> queens; //0-white, 1-black | square index
	std::array<std::vector<int>, 2> kings; //0-white, 1-black | square index
	std::array<std::vector<int>, 2> attackedSquares; //0-white, 1-black | square index
	std::array<std::vector<BoardUtility::Pin>, 2> pins; //0-white, 1-black | square index
};

