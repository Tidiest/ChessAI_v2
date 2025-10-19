#pragma once
#include <SFML/Graphics.hpp>
#include "BoardUtility.h"

class BoardDrawer
{
public:

	struct Cell
	{
		int id;
		sf::RectangleShape sprite;
		sf::Vector2f pos;
		sf::Vector2f size;
		sf::Color color;

		Cell(sf::Vector2f pos, float size, sf::Color color)
			: pos(pos), size(sf::Vector2f(size, size)), color(color)
		{
			this->sprite.setFillColor(color);
			this->sprite.setSize(sf::Vector2f(size, size));
			this->sprite.setPosition(pos);
		}

	};

	struct PieceSprite
	{
		sf::Sprite sprite;
		bool isDragging;

		PieceSprite(sf::Vector2f pos, sf::Texture& texture)
		{
			isDragging = false;
			this->sprite.setTexture(texture);
			this->sprite.setPosition(pos);
			this->sprite.setScale(0.75f, 0.75f);
		}
		~PieceSprite()
		{

		}
	};

	BoardDrawer(float cell_size, int* board);
	void drawBoard(sf::RenderWindow& window);
	void drawPieces(int* board);
	PieceSprite* getPieceOnMouse(sf::Vector2i mousePos);
	std::vector<Cell>& getCells() { return this->cells; };
	Cell* getCellOnPos(sf::Vector2f pos);
	void loadTextures();

	void highlightMoves(std::vector<BoardUtility::Move>& moves);
	void highlightMoves(std::vector<int>& squares);
	void highlightMoves(std::vector<BoardUtility::Pin>& pins);
	void unhighlightMoves();

private:
	float cell_size;
	sf::Color wColor;
	sf::Color bColor;
	std::vector<Cell> cells;
	std::vector<PieceSprite> pieces;
	std::map<std::string, sf::Texture> textureMap;
};

