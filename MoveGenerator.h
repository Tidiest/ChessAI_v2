#pragma once
#include <vector>
#include "Board.h"
class MoveGenerator
{
public:
	MoveGenerator(Board& board);

	void PrecomputedMoves();
	std::vector<BoardUtility::Move>& GenerateMoves();
	std::vector<BoardUtility::Move>& GenerateAttacks();
	std::vector<BoardUtility::Move>& GenerateMoves(int startSquare);
	void GenerateSlidingMoves(int startSquare, int piece, bool attackOnly = false);
	void GenerateKnightMoves(int startSquare, int piece, bool attackOnly = false);
	void GenerateKingMoves(int startSquare, int piece, bool attackOnly = false);
	void GeneratePawnMoves(int startSquare, int piece, bool attackOnly = false);
	void GenerateCastlingMoves(int startSquare, int piece);

	std::vector<BoardUtility::Move>& getMovesForSquare(int index);
	BoardUtility::Move& getMoveForPiece(BoardUtility::Move& move);

	bool isLegalMove(BoardUtility::Move& move)
	{ 
		for (auto& m : moves) 
		{ 
			if (move.startSquare == m.startSquare && move.targetSquare == m.targetSquare) 
			{ return true; } 
		}; 
		return false; 
	};
	std::vector<BoardUtility::Move> moves;
	std::vector<BoardUtility::Move> movesForPiece;
private:
	Board& board;
	int DirectionOffsets[8] = { 8, -8, -1, 1, 7, -7, 9, -9 };
	int PawnDirectionOffsets[8] = { 8, 7, 9, 16, -8, -7, -9, -16 }; //down, down-left, down-right, two-down, up, up-right, up-left, two-up 
	int KnightDirectionOffsets[8] = { 15, -15, -6, 6, 10, -10, 17, -17 };
	int CastlingDirectionOffsets[2] = { 1, -1 };
	std::vector<std::vector<int>> NumSquaresToEdge;
	std::vector<BoardUtility::Pin> pins;
};

