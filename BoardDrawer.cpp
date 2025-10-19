#include "BoardDrawer.h"

BoardDrawer::BoardDrawer(float cell_size, int* board)
{
	this->wColor = sf::Color(200, 120, 180);
	this->bColor = sf::Color(170, 90, 150);

	this->cell_size = cell_size;

	for (int file = 0; file < 8; file++)
	{
		for (int rank = 0; rank < 8; rank++)
		{
			sf::Vector2f cell_pos(rank * this->cell_size, file * this->cell_size);
			Cell new_cell(cell_pos, this->cell_size, (file + rank) % 2 == 0 ? this->wColor : this->bColor);
			new_cell.id = file*8 + rank;
			this->cells.push_back(new_cell);
		}
	}
	this->loadTextures();
	drawPieces(board);
}

void BoardDrawer::drawBoard(sf::RenderWindow& window)
{
	for (const auto& cell : this->cells)
	{
		window.draw(cell.sprite);
	}
	for (auto& piece : this->pieces)
	{
		window.draw(piece.sprite);
	}
}

void BoardDrawer::drawPieces(int* board)
{
	this->pieces.clear();

	for (int i = 0; i < 64; i++)
	{
		if (board[i] >> 3 == 0)
		{
			continue;
		}
		std::string piece_color = board[i] >> 3 == 1 ? "w" : "b";
		int pieceIndex = board[i] & 7;
		std::string textureKey = piece_color + "-" + std::to_string(pieceIndex);

		// Access the texture from the map instead of creating new ones
		if (textureMap.find(textureKey) != textureMap.end())
		{
			PieceSprite piece1(cells[i].pos, textureMap[textureKey]);
			pieces.push_back(piece1);
		}
	}
}

BoardDrawer::PieceSprite* BoardDrawer::getPieceOnMouse(sf::Vector2i mousePos)
{
	PieceSprite* piece = nullptr;
	for (auto& p : pieces)
	{
		if (p.sprite.getGlobalBounds().contains(sf::Vector2f(mousePos)))
		{
			piece = &p;
		}
		if (piece != nullptr)
		{
			break;
		}
	}
	return piece;
}


BoardDrawer::Cell* BoardDrawer::getCellOnPos(sf::Vector2f pos)
{
	BoardDrawer::Cell* cellptr = nullptr;

	for (auto& cell : this->getCells())
	{
		if (cell.sprite.getGlobalBounds().contains(pos.x, pos.y))
		{
			cellptr = &cell;
		}
		if (cellptr != nullptr)
		{
			break;
		}
	}

	return cellptr;
}

void BoardDrawer::loadTextures()
{
	// Load all textures at once and store in the map
	std::vector<std::string> pieceNames = { "w-1", "w-2", "w-3", "w-4", "w-5", "w-6", "b-1", "b-2", "b-3", "b-4", "b-5", "b-6" };

	for (const auto& name : pieceNames)
	{
		sf::Texture texture;
		if (texture.loadFromFile("sprites/" + name + ".png"))
		{
			textureMap[name] = texture;  // Store texture in map with key as filename
		}
	}
}

void BoardDrawer::highlightMoves(std::vector<BoardUtility::Move>& moves)
{
	for (auto& move : moves)
	{
		Cell* cell = &cells[move.targetSquare];
		this->cells[move.targetSquare].sprite.setFillColor(sf::Color(cell->sprite.getFillColor().r, cell->sprite.getFillColor().g, cell->sprite.getFillColor().b, 130));
	}
}

void BoardDrawer::highlightMoves(std::vector<int>& squares)
{
	for (auto& square : squares)
	{
		Cell* cell = &cells[square];
		this->cells[square].sprite.setFillColor(sf::Color(cell->sprite.getFillColor().r, cell->sprite.getFillColor().g, cell->sprite.getFillColor().b, 130));
	}
}

void BoardDrawer::highlightMoves(std::vector<BoardUtility::Pin>& pins)
{
	for (auto& pin : pins)
	{
		for (auto& allowedMove : pin.allowedMoves)
		{
			Cell* cell = &cells[allowedMove.targetSquare];
			this->cells[allowedMove.targetSquare].sprite.setFillColor(sf::Color(cell->sprite.getFillColor().r, cell->sprite.getFillColor().g, cell->sprite.getFillColor().b, 130));
		}
	}
}

void BoardDrawer::unhighlightMoves()
{
	for (auto& square : cells)
	{
		square.sprite.setFillColor(sf::Color(square.sprite.getFillColor().r, square.sprite.getFillColor().g, square.sprite.getFillColor().b, 255));
	}
}