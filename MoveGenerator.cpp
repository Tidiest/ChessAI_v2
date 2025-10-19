#include "MoveGenerator.h"

MoveGenerator::MoveGenerator(Board& board) : board(board)
{
	PrecomputedMoves();
}

void MoveGenerator::PrecomputedMoves()
{
	this->NumSquaresToEdge.resize(64);
	for (int file = 0; file < 8; file++)
	{
		for (int rank = 0; rank < 8; rank++)
		{
			int numNorth = 7 - rank;
			int numSouth = rank;
			int numWest = file;
			int numEast = 7 - file;

			int squareIndex = rank * 8 + file;

			std::vector<int> numSquaresOfIndex =
			{
				numNorth,
				numSouth,
				numWest,
				numEast,
				std::min(numNorth, numWest),
				std::min(numSouth, numEast),
				std::min(numNorth, numEast),
				std::min(numSouth, numWest)
			};
			this->NumSquaresToEdge[squareIndex] = (numSquaresOfIndex);

		}
	}
}

std::vector<BoardUtility::Move>& MoveGenerator::GenerateMoves()
{
	this->moves.clear();
	for (int startSquare = 0; startSquare < 64; startSquare++)
	{
		int piece = board.getBoard()[startSquare];
		int pieceType = BoardUtility::getPieceType(piece);
		int pieceColor = BoardUtility::getPieceColor(piece);

		if (pieceType == BoardUtility::None)
		{
			continue;
		}
		if (pieceColor == board.getColorToMove())
		{
			if (pieceType == BoardUtility::Queen ||
				pieceType == BoardUtility::Bishop ||
				pieceType == BoardUtility::Rook)
			{
				GenerateSlidingMoves(startSquare, piece);
			}
			else if (pieceType == BoardUtility::Knight)
			{
				GenerateKnightMoves(startSquare, piece);
			}
			else if (pieceType == BoardUtility::King)
			{
				GenerateKingMoves(startSquare, piece);
				GenerateCastlingMoves(startSquare, piece);
			}
			else if (pieceType == BoardUtility::Pawn)
			{
				GeneratePawnMoves(startSquare, piece);
			}
		}
	}
	std::cout << moves.size() << " moves generated!" << std::endl;
	return this->moves;
}

std::vector<BoardUtility::Move>& MoveGenerator::GenerateAttacks()
{
	this->moves.clear();
	this->pins.clear();
	//clear attack information boards
	for (int i = 0; i < 2; i++)
	{
		board.getAttackedSquares()[i].clear();
		board.getPins()[i].clear();
	}
	for (int startSquare = 0; startSquare < 64; startSquare++)
	{
		int piece = board.getBoard()[startSquare];
		int pieceType = BoardUtility::getPieceType(piece);
		int pieceColor = BoardUtility::getPieceColor(piece);

		if (pieceType == BoardUtility::None)
		{
			continue;
		}
		if (pieceType == BoardUtility::Queen ||
			pieceType == BoardUtility::Bishop ||
			pieceType == BoardUtility::Rook)
		{
			GenerateSlidingMoves(startSquare, piece, true);
		}
		else if (pieceType == BoardUtility::Knight)
		{
			GenerateKnightMoves(startSquare, piece, true);
		}
		else if (pieceType == BoardUtility::King)
		{
			GenerateKingMoves(startSquare, piece, true);
		}
		else if (pieceType == BoardUtility::Pawn)
		{
			GeneratePawnMoves(startSquare, piece, true);
		}
	}
	std::cout << moves.size() << " attacks generated!" << std::endl;
	//fill attacked squares array
	for (auto& move : this->moves)
	{
		board.getAttackedSquares()[BoardUtility::getPieceColor(board.getBoard()[move.startSquare]) == BoardUtility::White ? 0 : 1].push_back(move.targetSquare);
	}
	for (auto& pin : this->pins)
	{
		board.getPins()[BoardUtility::getPieceColor(board.getBoard()[pin.pinnedPieceSquare]) == BoardUtility::White ? 0 : 1].push_back(pin);
	}
	return this->moves;
}

std::vector<BoardUtility::Move>& MoveGenerator::GenerateMoves(int startSquare)
{
	this->moves.clear();
	int piece = board.getBoard()[startSquare];
	int pieceType = BoardUtility::getPieceType(piece);
	int pieceColor = BoardUtility::getPieceColor(piece);

	if (pieceColor == board.getColorToMove())
	{
		if (pieceType == BoardUtility::Queen ||
			pieceType == BoardUtility::Bishop ||
			pieceType == BoardUtility::Rook)
		{
			GenerateSlidingMoves(startSquare, piece);
		}
		else if (pieceType == BoardUtility::Knight)
		{
			GenerateKnightMoves(startSquare, piece);
		}
		else if (pieceType == BoardUtility::King)
		{
			GenerateKingMoves(startSquare, piece);
			GenerateCastlingMoves(startSquare, piece);
		}
		else if (pieceType == BoardUtility::Pawn)
		{
			GeneratePawnMoves(startSquare, piece);
		}
	}
	std::cout << moves.size() << " moves generated!" << std::endl;
	return this->moves;
}

void MoveGenerator::GenerateSlidingMoves(int startSquare, int piece, bool attackOnly)
{
	int startPieceColor = BoardUtility::getPieceColor(piece);
	int startPieceType = BoardUtility::getPieceType(piece);
	int startDirection = startPieceType == BoardUtility::Rook ? 0 : startPieceType == BoardUtility::Bishop ? 4 : 0;
	int endDirection = startPieceType == BoardUtility::Rook ? 4 : startPieceType == BoardUtility::Bishop ? 8 : 8;

	for (int direction = startDirection; direction < endDirection; direction++)
	{
		for (int n = 0; n < NumSquaresToEdge[startSquare][direction]; n++)
		{
			int targetSquare = startSquare + DirectionOffsets[direction] * (n + 1);
			int pieceOnTargetSquare = board.getBoard()[targetSquare];

			//Blocked by friendly piece, so can't move further
			if (BoardUtility::getPieceColor(pieceOnTargetSquare) == startPieceColor && !attackOnly)
			{
				break;
			}
			//limit moves if is pinned
			if (board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1].size() > 0)
			{
				bool isPinned = false;
				for (auto& pin : board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1])
				{
					if (pin.pinnedPieceSquare == startSquare)
					{
						isPinned = true;
						bool isMoveAllowed = false;
						for (auto& allowedMove : pin.allowedMoves)
						{
							if (allowedMove.targetSquare == targetSquare)
							{
								isMoveAllowed = true;
								break;
							}
						}
						if (isMoveAllowed)
						{
							BoardUtility::Move new_move(startSquare, targetSquare);
							this->moves.push_back(new_move);
							break;
						}
					}
				}
				if (!isPinned)
				{
					BoardUtility::Move new_move(startSquare, targetSquare);
					this->moves.push_back(new_move);
				}
			}
			else
			{
				BoardUtility::Move new_move(startSquare, targetSquare);
				this->moves.push_back(new_move);
			}
			

			//Very complicated pin detection logic
			if (attackOnly && BoardUtility::getPieceColor(pieceOnTargetSquare) != startPieceColor 
				&& BoardUtility::getPieceType(pieceOnTargetSquare) != BoardUtility::None && 
				BoardUtility::getPieceType(pieceOnTargetSquare) != BoardUtility::King)
			{
				std::vector<BoardUtility::Move> allowedMoves;
				int pieceOnLastTargetSquare = 0;
				int piecesInAWay = 0;
				int pinnedPiece = 0;
				for (int j = 0; j < NumSquaresToEdge[startSquare][direction]; j++)
				{
					int targetSquare = startSquare + DirectionOffsets[direction] * (j + 1);
					int pieceOnTargetSquare = board.getBoard()[targetSquare];
					if (BoardUtility::getPieceType(pieceOnTargetSquare) != BoardUtility::None && 
						BoardUtility::getPieceType(pieceOnTargetSquare) != BoardUtility::King)
					{
						piecesInAWay++;
						pinnedPiece = targetSquare;
					}
					if (piecesInAWay > 1)
					{
						break;
					}
					BoardUtility::Move new_move(startSquare, targetSquare);
					allowedMoves.push_back(new_move);
					pieceOnLastTargetSquare = board.getBoard()[targetSquare];
					if (BoardUtility::getPieceType(pieceOnTargetSquare) == BoardUtility::King)
					{
						allowedMoves.pop_back();
						BoardUtility::Move new_move(startSquare, startSquare);
						allowedMoves.push_back(new_move);
						break;
					}
				}
				if (allowedMoves.size() > 0 && BoardUtility::getPieceType(pieceOnLastTargetSquare) == BoardUtility::King &&
					BoardUtility::getPieceColor(pieceOnLastTargetSquare) != startPieceColor)
				{
					BoardUtility::Pin new_pin(startSquare, pinnedPiece);
					for (auto& pin : allowedMoves)
					{
						new_pin.allowedMoves.push_back(pin);
					}
					this->pins.push_back(new_pin);
				}
				break;
			}
			else if (BoardUtility::getPieceColor(pieceOnTargetSquare) != BoardUtility::None)
			{
				break;
			}

		}
	}
}

void MoveGenerator::GenerateKnightMoves(int startSquare, int piece, bool attackOnly)
{
	int file = BoardUtility::indexToFile(startSquare);
	int rank = BoardUtility::indexToRank(startSquare);
	int startPieceColor = BoardUtility::getPieceColor(piece);
	int startPieceType = BoardUtility::getPieceType(piece);

	for (int directionIndex = 0; directionIndex < 8; directionIndex++)
	{
		for (int n = 0; n < 1; n++)
		{
			if ((file == 1 && (KnightDirectionOffsets[directionIndex] == -17 or KnightDirectionOffsets[directionIndex] == 15)) ||
				((file == 1 or file == 2) && (KnightDirectionOffsets[directionIndex] == -10 or KnightDirectionOffsets[directionIndex] == 6)) ||
				(file == 8 && (KnightDirectionOffsets[directionIndex] == -15 or KnightDirectionOffsets[directionIndex] == 17)) ||
				((file == 8 or file == 7) && (KnightDirectionOffsets[directionIndex] == -6 or KnightDirectionOffsets[directionIndex] == 10)))
			{
				break;
			}

			if ((rank == 1 or rank == 2) && (KnightDirectionOffsets[directionIndex] == 15 or KnightDirectionOffsets[directionIndex] == 17) ||
				(rank == 8 or rank == 7) && (KnightDirectionOffsets[directionIndex] == -15 or KnightDirectionOffsets[directionIndex] == -17) ||
				rank == 8 && (KnightDirectionOffsets[directionIndex] == -10 or KnightDirectionOffsets[directionIndex] == -6) ||
				rank == 1 && (KnightDirectionOffsets[directionIndex] == 10 or KnightDirectionOffsets[directionIndex] == 6))
			{
				break;
			}

			int targetSquare = startSquare + KnightDirectionOffsets[directionIndex] * (n + 1);
			int pieceOnTargetSquare = board.getBoard()[targetSquare];

			//Blocked by friendly piece, so can't move further
			if (BoardUtility::getPieceColor(pieceOnTargetSquare) == startPieceColor && !attackOnly)
			{
				break;
			}

			//limit moves if is pinned
			if (board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1].size() > 0)
			{
				bool isPinned = false;
				for (auto& pin : board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1])
				{
					if (pin.pinnedPieceSquare == startSquare)
					{
						isPinned = true;
						bool isMoveAllowed = false;
						for (auto& allowedMove : pin.allowedMoves)
						{
							if (allowedMove.targetSquare == targetSquare)
							{
								isMoveAllowed = true;
								break;
							}
						}
						if (isMoveAllowed)
						{
							BoardUtility::Move new_move(startSquare, targetSquare);
							this->moves.push_back(new_move);
							break;
						}
					}
				}
				if (!isPinned)
				{
					BoardUtility::Move new_move(startSquare, targetSquare);
					this->moves.push_back(new_move);
				}
			}
			else
			{
				BoardUtility::Move new_move(startSquare, targetSquare);
				this->moves.push_back(new_move);
			}
		}
	}
}

void MoveGenerator::GenerateKingMoves(int startSquare, int piece, bool attackOnly)
{
	int startPieceColor = BoardUtility::getPieceColor(piece);
	int startPieceType = BoardUtility::getPieceType(piece);

	for (int directionIndex = 0; directionIndex < 8; directionIndex++)
	{
		for (int n = 0; n < std::min(NumSquaresToEdge[startSquare][directionIndex], 1); n++)
		{
			int targetSquare = startSquare + DirectionOffsets[directionIndex] * (n + 1);
			int pieceOnTargetSquare = board.getBoard()[targetSquare];

			//Blocked by friendly piece, so can't move further
			if (BoardUtility::getPieceColor(pieceOnTargetSquare) == startPieceColor && !attackOnly)
			{
				break;
			}
			//check if target square is attacked
			bool targetSquareIsAttacked = false;
			for (auto& attackSquare : board.getAttackedSquares()[BoardUtility::getOpponentColor(piece) == BoardUtility::White ? 0 : 1])
			{
				if (attackSquare == targetSquare)
				{
					targetSquareIsAttacked = true;
					break;
				}
			}
			if (targetSquareIsAttacked)
			{
				break;
			}

			BoardUtility::Move new_move(startSquare, targetSquare);
			moves.push_back(new_move);

			//Can't move any further after capturing opponent's piece
			if (BoardUtility::getPieceColor(pieceOnTargetSquare) != BoardUtility::None)
			{
				break;
			}
		}
	}
}

void MoveGenerator::GenerateCastlingMoves(int startSquare, int piece)
{
	int startPieceColor = BoardUtility::getPieceColor(piece);

	int kingIndex = startPieceColor == BoardUtility::White ? 0 : 1;
	
	for (int direction = 0; direction < 2; direction++)
	{
		bool blocked = false;
		if (board.castlingRight[kingIndex*2 + direction] != true)
		{
			continue;
		}
		for (int n = 0; n < 2; n++)
		{
			int targetSquare = startSquare + CastlingDirectionOffsets[direction] * (n + 1);
			int pieceOnTargetSquare = board.getBoard()[targetSquare];

			if (BoardUtility::getPieceColor(pieceOnTargetSquare) == startPieceColor ||
				BoardUtility::getPieceColor(pieceOnTargetSquare) == BoardUtility::getOpponentColor(piece))
			{
				blocked = true;
				break;
			}
			
			//DON'T KNOW IF NECCESSARY
			if (direction == 1)
			{
				int targetSquare = startSquare + CastlingDirectionOffsets[direction] * (n + 2);
				int pieceOnTargetSquare = board.getBoard()[targetSquare];
				if (BoardUtility::getPieceColor(pieceOnTargetSquare) == startPieceColor ||
					BoardUtility::getPieceColor(pieceOnTargetSquare) == BoardUtility::getOpponentColor(piece))
				{
					blocked = true;
					break;
				}
			}

		}

		if (!blocked)
		{
			int targetSquare = startSquare + CastlingDirectionOffsets[direction] * 2;
			BoardUtility::Move new_move(startSquare, targetSquare, BoardUtility::Move::CastleFlag);
			moves.push_back(new_move);
		}

	}

}

void MoveGenerator::GeneratePawnMoves(int startSquare, int piece, bool attackOnly)
{
	int file = BoardUtility::indexToFile(startSquare);
	int rank = BoardUtility::indexToRank(startSquare);

	int startPieceColor = BoardUtility::getPieceColor(piece);
	int startPieceType = BoardUtility::getPieceType(piece);

	int startDirection = startPieceColor == BoardUtility::Black ? 0 : 4;
	int endDirection = startPieceColor == BoardUtility::Black ? 2 : 6;

	//if on the starting rank -> can two-up
	if ((startPieceColor == BoardUtility::Black && rank == 7) ||
		(startPieceColor == BoardUtility::White && rank == 2))
	{
		endDirection++;
	}
	for (int direction = startDirection; direction <= endDirection; direction++)
	{
		int targetSquare = startSquare + PawnDirectionOffsets[direction];
		int pieceOnTargetSquare = board.getBoard()[targetSquare];

		//Blocked by friendly piece, so can't move further
		if (BoardUtility::getPieceColor(pieceOnTargetSquare) == startPieceColor)
		{
			continue;
		}

		//Blocked by opponent piece, so can't move further
		if (PawnDirectionOffsets[direction] == 8 || PawnDirectionOffsets[direction] == -8 || 
			PawnDirectionOffsets[direction] == 16 || PawnDirectionOffsets[direction] == -16)
		{
			if (BoardUtility::getPieceColor(pieceOnTargetSquare) != BoardUtility::None)
			{
				continue;
			}
		}
		//Captures
		if ((PawnDirectionOffsets[direction] == 7) || (PawnDirectionOffsets[direction] == 9) ||
			(PawnDirectionOffsets[direction] == -7) ||
			(PawnDirectionOffsets[direction] == -9))
		{
			if (!((PawnDirectionOffsets[direction] == 7 && file != 1) || (PawnDirectionOffsets[direction] == 9 && file != 8) ||
				(PawnDirectionOffsets[direction] == -7 && file != 8) ||
				(PawnDirectionOffsets[direction] == -9 && file != 1)))
			{
				continue;
			}
			if (BoardUtility::getPieceColor(pieceOnTargetSquare) != BoardUtility::None)
			{
				std::cout << "TRUE" << std::endl;
				
				//limit moves if is pinned
				if (board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1].size() > 0)
				{
					bool isPinned = false;
					for (auto& pin : board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1])
					{
						if (pin.pinnedPieceSquare == startSquare)
						{
							isPinned = true;
							bool isMoveAllowed = false;
							for (auto& allowedMove : pin.allowedMoves)
							{
								if (allowedMove.targetSquare == targetSquare)
								{
									isMoveAllowed = true;
									break;
								}
							}
							if (isMoveAllowed)
							{
								BoardUtility::Move new_move(startSquare, targetSquare, BoardUtility::Move::EnPassantCaptureFlag);
								moves.push_back(new_move);
								break;
							}
						}
					}
					if (!isPinned)
					{
						BoardUtility::Move new_move(startSquare, targetSquare, BoardUtility::Move::EnPassantCaptureFlag);
						moves.push_back(new_move);
					}
				}
				else
				{
					BoardUtility::Move new_move(startSquare, targetSquare, BoardUtility::Move::EnPassantCaptureFlag);
					moves.push_back(new_move);
				}
				
			}
			continue;
		}
		
		if ((!attackOnly) && (PawnDirectionOffsets[direction] == -16 || PawnDirectionOffsets[direction] == 16))
		{
			if (board.getBoard()[targetSquare - PawnDirectionOffsets[direction] / 2] == BoardUtility::None)
			{
				//limit moves if is pinned
				if (board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1].size() > 0)
				{
					bool isPinned = false;
					for (auto& pin : board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1])
					{
						if (pin.pinnedPieceSquare == startSquare)
						{
							isPinned = true;
							bool isMoveAllowed = false;
							for (auto& allowedMove : pin.allowedMoves)
							{
								if (allowedMove.targetSquare == targetSquare)
								{
									isMoveAllowed = true;
									break;
								}
							}
							if (isMoveAllowed)
							{
								BoardUtility::Move new_move(startSquare, targetSquare, BoardUtility::Move::PawnTwoUpFlag);
								moves.push_back(new_move);
								break;
							}
						}
					}
					if (!isPinned)
					{
						BoardUtility::Move new_move(startSquare, targetSquare, BoardUtility::Move::PawnTwoUpFlag);
						moves.push_back(new_move);
					}
				}
				else
				{
					BoardUtility::Move new_move(startSquare, targetSquare, BoardUtility::Move::PawnTwoUpFlag);
					moves.push_back(new_move);
				}
			}
			continue;
		}
		if (!attackOnly)
		{
			//limit moves if is pinned
			if (board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1].size() > 0)
			{
				bool isPinned = false;
				for (auto& pin : board.getPins()[startPieceColor == BoardUtility::White ? 0 : 1])
				{
					if (pin.pinnedPieceSquare == startSquare)
					{
						isPinned = true;
						bool isMoveAllowed = false;
						for (auto& allowedMove : pin.allowedMoves)
						{
							if (allowedMove.targetSquare == targetSquare)
							{
								isMoveAllowed = true;
								break;
							}
						}
						if (isMoveAllowed)
						{
							BoardUtility::Move new_move(startSquare, targetSquare);
							this->moves.push_back(new_move);
							break;
						}
					}
				}
				if (!isPinned)
				{
					BoardUtility::Move new_move(startSquare, targetSquare);
					this->moves.push_back(new_move);
				}
			}
			else
			{
				BoardUtility::Move new_move(startSquare, targetSquare);
				this->moves.push_back(new_move);
			}
		}
	}
	
}

std::vector<BoardUtility::Move>& MoveGenerator::getMovesForSquare(int index)
{
	std::vector<BoardUtility::Move> movesForSquare;
	movesForSquare.clear();

	for (auto& move : moves)
	{
		if (move.startSquare == index)
		{
			movesForSquare.push_back(move);
		}
	}
	return movesForSquare;
}

BoardUtility::Move& MoveGenerator::getMoveForPiece(BoardUtility::Move& move)
{
	BoardUtility::Move null_move;

	for (auto& m : moves)
	{
		if (m.isSame(move))
		{
			return m;
		}
	}
	return null_move;
}
