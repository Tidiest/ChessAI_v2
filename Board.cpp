#include "Board.h"

Board::Board()
{
	//std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	//std::string fen = "8/8/8/8/7N/8/8/8 w KQkq - 0 1";
	std::string fen = "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR w KQkq - 0 1";
	//std::string fen = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";

	loadPositionFromFEN(fen);
	colorToMove = BoardUtility::White;

	this->rookSquares = { 63, 56, 7, 0 };
	this->rookMoves = { 0, 0, 0, 0 };
	this->kingMoves = { 0, 0 };
	this->castlingRight = { true, true, true, true };
}

void Board::loadPositionFromFEN(std::string fen)
{
	// Define the mapping of characters to enum values
	std::map<char, BoardUtility::PieceNames> pieceMap =
	{
		{'k', BoardUtility::King},
		{'q', BoardUtility::Queen},
		{'r', BoardUtility::Rook},
		{'b', BoardUtility::Bishop},
		{'n', BoardUtility::Knight},
		{'p', BoardUtility::Pawn}
	};

	int file = 0, rank = 0;
	std::string fenBoard = fen;

	for (char symbol : fenBoard)
	{
		if (symbol == '/')
		{
			file = 0;
			rank++;
		}
		else
		{
			if (isdigit(symbol))
			{
				file += symbol - '0';
			}
			else
			{
				int pieceColor = isupper(symbol) ? BoardUtility::White : BoardUtility::Black;
				int pieceType = pieceMap[tolower(symbol)];
				int index = (rank * 8 + file);
				this->board[index] = pieceColor | pieceType;
				file++;
			}
		}
	}

}

void Board::makeMove(BoardUtility::Move& move)
{
	if (move.moveFlag == BoardUtility::Move::NoFlag || move.moveFlag == BoardUtility::Move::EnPassantCaptureFlag || move.moveFlag == BoardUtility::Move::PawnTwoUpFlag)
	{
		// Update rook moves
		if (BoardUtility::getPieceType(board[move.startSquare]) == BoardUtility::Rook)
		{
			this->rookMoves[move.startSquare == this->rookSquares[0] ? 0 : 
				move.startSquare == this->rookSquares[1] ? 1 : 
				move.startSquare == this->rookSquares[2] ? 2 : 3]++;
			//Update castling rights for rooks
			this->castlingRight[move.startSquare == this->rookSquares[0] ? 0 : 
				move.startSquare == this->rookSquares[1] ? 1 : 
				move.startSquare == this->rookSquares[2] ? 2 : 3] = false;
		}
		// Update king moves
		if (BoardUtility::getPieceType(board[move.startSquare]) == BoardUtility::King)
		{
			this->kingMoves[BoardUtility::getPieceColor(board[move.startSquare]) == BoardUtility::White ? 0 : 1]++;
			//Update castling rights after king move
			this->castlingRight[BoardUtility::getPieceColor(board[move.startSquare]) == BoardUtility::White ? 0 : 2] = false;
			this->castlingRight[BoardUtility::getPieceColor(board[move.startSquare]) == BoardUtility::White ? 1 : 3] = false;
		}
		//If rook is captured, update castling rights
		if (BoardUtility::getPieceType(board[move.targetSquare]) == BoardUtility::Rook)
		{
			this->castlingRight[move.targetSquare == this->rookSquares[0] ?
				0 :
				move.targetSquare == this->rookSquares[1] ?
				1 :
				move.targetSquare == this->rookSquares[2] ?
				2 : 3] = false;
		}
		board[move.targetSquare] = board[move.startSquare];
		board[move.startSquare] = 0;
	}
	else if (move.moveFlag == BoardUtility::Move::CastleFlag)
	{
		//Update castling rights after castling
		this->castlingRight[BoardUtility::getPieceColor(board[move.startSquare]) == BoardUtility::White ? 0 : 2] = false;
		this->castlingRight[BoardUtility::getPieceColor(board[move.startSquare]) == BoardUtility::White ? 1 : 3] = false;
		switch (move.targetSquare)
		{
		case 62:
			std::cout << "Castle 62" << std::endl;
			board[move.targetSquare] = board[move.startSquare];

			std::cout << "Rook " << this->rookSquares[1] << std::endl;
			board[move.targetSquare - 1] = board[this->rookSquares[0]];
			board[this->rookSquares[0]] = 0;
			break;
		case 58:
			std::cout << "Castle 58" << std::endl;
			board[move.targetSquare] = board[move.startSquare];

			board[move.targetSquare + 1] = board[rookSquares[1]];
			board[rookSquares[1]] = 0;
			break;
		case 6:
			std::cout << "Castle 6" << std::endl;
			board[move.targetSquare] = board[move.startSquare];

			board[move.targetSquare - 1] = board[rookSquares[2]];
			board[rookSquares[2]] = 0;
			break;
		case 2:
			std::cout << "Castle 2" << std::endl;
			board[move.targetSquare] = board[move.startSquare];

			board[move.targetSquare + 1] = board[rookSquares[3]];
			board[rookSquares[3]] = 0;
			break;
		default:
			break;
		}
		board[move.startSquare] = 0;
	}

	this->colorToMove = colorToMove == BoardUtility::White ? BoardUtility::Black : BoardUtility::White;
}
