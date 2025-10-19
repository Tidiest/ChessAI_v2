#include "PlayerUtility.h"

PlayerUtility::PlayerUtility(Player* player) : player(player)
{
	this->offset = {0,0};
	startCell = nullptr;
	targetCell = nullptr;
}

void PlayerUtility::handlePieceMovement(sf::Vector2i& mousePos, sf::RenderWindow& window)
{
	if (player->draggingPiece == nullptr)
	{
		return;
	}
	else if (player->draggingPiece->isDragging)
	{
		mousePos = sf::Mouse::getPosition(window);
		//set piece' pos under mouse
		player->draggingPiece->sprite.setPosition(sf::Vector2f(mousePos) + this->offset);
	}
}

void PlayerUtility::startMovingPiece(BoardDrawer& drawer, sf::Vector2i& mousePos, sf::RenderWindow& window, MoveGenerator& moveGen)
{
	mousePos = sf::Mouse::getPosition(window);
	player->draggingPiece = drawer.getPieceOnMouse(mousePos);
	moveGen.movesForPiece.clear();

	if (player->draggingPiece != nullptr && player->draggingPiece->isDragging == false)
	{
		//start dragging a piece under mouse
		player->draggingPiece->isDragging = true;
		//set starting cell
		this->startCell = drawer.getCellOnPos(player->draggingPiece->sprite.getPosition());
		//calculate offset
		this->offset = player->draggingPiece->sprite.getPosition() - sf::Vector2f(mousePos);

		moveGen.movesForPiece = moveGen.GenerateMoves(startCell->id);
	}
}

bool PlayerUtility::stopMovingPiece(sf::Vector2i& mousePos, Board& board, BoardDrawer& drawer, MoveGenerator& moveGen)
{
	if (player->draggingPiece != nullptr)
	{
		player->draggingPiece->isDragging = false;
		this->targetCell = drawer.getCellOnPos(sf::Vector2f(mousePos));

		if (targetCell != nullptr && this->startCell->id != targetCell->id)
		{
			BoardUtility::Move new_move(this->startCell->id, targetCell->id);
			BoardUtility::Move rightMove = moveGen.getMoveForPiece(new_move);

			if (BoardUtility::Move::isNull(rightMove))
			{
				std::cout << "Null move" << std::endl;
			}
			else
			{
				std::cout << rightMove.moveFlag << std::endl;
				player->draggingPiece->sprite.setPosition(targetCell->pos);
				board.makeMove(rightMove);
				drawer.drawPieces(board.getBoard());
				drawer.unhighlightMoves();
				return true;
			}
		}
		player->draggingPiece->sprite.setPosition(this->startCell->pos);
	}
	drawer.unhighlightMoves();
	return false;
}
