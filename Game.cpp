#include "Game.h"

const int Game::WINDOW_SIZE = 800;
const unsigned int Game::FPS = 40;

Game::Game() : drawer(WINDOW_SIZE/8, board.getBoard()), plUt(&player), moveGen(board)
{
	window.create(sf::VideoMode(WINDOW_SIZE,WINDOW_SIZE), "Chess", sf::Style::Close);
	window.setFramerateLimit(FPS);
}

void Game::loop()
{
	sf::Vector2i mousePos = { 0,0 };
	moveGen.GenerateAttacks();
	moveGen.GenerateMoves();
	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					plUt.startMovingPiece(drawer, mousePos, window, moveGen);
					drawer.highlightMoves(moveGen.movesForPiece);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (plUt.stopMovingPiece(mousePos, board, drawer, moveGen))
					{
						moveGen.GenerateAttacks();
					}
				}
				break;
			case sf::Event::KeyPressed:
				if (evnt.key.code == sf::Keyboard::W)
				{
					drawer.highlightMoves(board.getAttackedSquares()[0]);
				}
				if (evnt.key.code == sf::Keyboard::B)
				{
					drawer.highlightMoves(board.getAttackedSquares()[1]);
				}
				if (evnt.key.code == sf::Keyboard::P)
				{
					drawer.highlightMoves(board.getPins()[1]);
				}
				break;
			case sf::Event::KeyReleased:
				if (evnt.key.code == sf::Keyboard::W)
				{
					drawer.unhighlightMoves();
				}
				if (evnt.key.code == sf::Keyboard::B)
				{
					drawer.unhighlightMoves();
				}
				if (evnt.key.code == sf::Keyboard::P)
				{
					drawer.unhighlightMoves();
				}
				break;
			default:
				break;
			}
		}
		plUt.handlePieceMovement(mousePos, window);

		window.clear(sf::Color::White);
		drawer.drawBoard(window);
		window.display();

	}
}
