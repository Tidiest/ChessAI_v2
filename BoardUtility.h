#pragma once
#include <iostream>
#include <vector>
class BoardUtility
{
public:
	enum PieceNames
	{
		None, King, Pawn, Knight, Bishop, Rook, Queen,
		White = 8, Black = 16
	};

	struct Move
	{
		int moveValue;

		static const int NoFlag = 0b0000;
		static const int EnPassantCaptureFlag = 0b0001;
		static const int CastleFlag = 0b0010;
		static const int PawnTwoUpFlag = 0b0100;
		static const int PromoteToQueenFlag = 0b0110;
		static const int PromoteToRookFlag = 0b1000;
		static const int PromoteToBishopFlag = 0b1010;
		static const int PromoteToKnightFlag = 0b1100;

		int startSquare;
		int targetSquare;
		int moveFlag;

		Move()
		{
			this->startSquare = 0;
			this->targetSquare = 0;
			this->moveFlag = 0;
			this->moveValue = -1;
		}

		Move(int startSquare, int targetSquare, int flag = NoFlag)
		{
			moveValue = (startSquare | targetSquare << 6 | flag << 12);
			this->startSquare = moveValue & 0b0000000000111111;
			this->targetSquare = (moveValue & 0b0000111111000000) >> 6;
			moveFlag = moveValue >> 12;
		}

		bool isSame(const Move& move)
		{
			if (move.startSquare == this->startSquare && move.targetSquare == this->targetSquare)
			{
				return true;
			}
			return false;
		}

		static bool isNull(Move& move)
		{
			if (move.moveValue == -1)
			{
				return true;
			}
			return false;
		}
	};

	struct Pin
	{
		int pinningPieceSquare;
		int pinnedPieceSquare;
		std::vector<Move> allowedMoves;
		Pin(int pinningPieceSquare, int pinnedPieceSquare)
		{
			this->pinningPieceSquare = pinningPieceSquare;
			this->pinnedPieceSquare = pinnedPieceSquare;
		};
	};

	BoardUtility();

	static int getPieceColor(int piece) { int pieceColor = piece & 0b11000; return pieceColor; };
	static int getPieceType(int piece) { int pieceType = piece & 7; return pieceType; };
	static int getOpponentColor(int piece) 
	{
		int pieceColor = piece & 0b11000; 
		if (pieceColor == White)
		{
			return Black;
		}
		else
		{
			return White;
		}
	};
	static int indexToRank(int index) { return 8 - (index / 8); };
	static int indexToFile(int index) { return (index & 0b111) + 1; };
private:

};

