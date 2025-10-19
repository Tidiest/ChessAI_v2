#pragma once
#include "PlayerUtility.h"
#include "MoveGenerator.h"
class Game
{
public:
	Game();

	void loop();
private:
	Player player;
	PlayerUtility plUt;
	Board board;
	BoardDrawer drawer;
	BoardUtility bUt;
	MoveGenerator moveGen;
	sf::RenderWindow window;
	static const int WINDOW_SIZE;
	static const unsigned int FPS;
};

