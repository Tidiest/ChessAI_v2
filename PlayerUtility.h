#pragma once
#include "Player.h"
#include "BoardUtility.h"
#include "MoveGenerator.h"
class PlayerUtility
{
public:
	PlayerUtility(Player* player);

	void handlePieceMovement(sf::Vector2i& mousePos, sf::RenderWindow& window);
	void startMovingPiece(BoardDrawer& drawer, sf::Vector2i& mousePos, sf::RenderWindow& window, MoveGenerator& moveGen);
	//returns true if a move was made
	bool stopMovingPiece(sf::Vector2i& mousePos, Board& board, BoardDrawer& drawer, MoveGenerator& moveGen);

	sf::Vector2f offset;
	Player* player;
	BoardDrawer::Cell* startCell;
	BoardDrawer::Cell* targetCell;
private:

};

