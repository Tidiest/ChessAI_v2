#pragma once
#include "BoardDrawer.h"

class Player
{
public:
	Player();
	BoardDrawer::PieceSprite* draggingPiece;
	sf::Vector2f pieceStartPos;

private:
	int colorSide;
	int time;

};

