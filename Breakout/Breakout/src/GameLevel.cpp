#include "GameLevel.h"
#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

GameLevel::GameLevel(const std::string& file, unsigned int viewportWidth, unsigned int viewportHeight)
{
	Load(file, viewportWidth, viewportHeight);
}

GameLevel::~GameLevel()
{
	delete[] m_Bricks; // Clean up dynamically allocated memory for bricks
}


bool GameLevel::Load(const std::string& file, unsigned int viewportWidth, unsigned int viewportHeight)
{
	std::ifstream levelFileStream(file);
	if (!levelFileStream.is_open())
	{
		std::cerr << "Failed to open level file: " << file << std::endl;
		return false;
	}

	// Allocate memory for level data
	// Assuming levelData is a 1D array representing a 2D grid of tiles
	std::vector<unsigned int> tmpData; // Initialize with zeros
	unsigned int rowCount = 0, columnCount = 0;
	std::string line;
	while (std::getline(levelFileStream, line))
	{
		std::istringstream sstream(line);
		unsigned int tileCode = 0;
		unsigned int column = 0; // Reset column for each new row
		while (sstream >> tileCode)
		{
			tmpData.push_back(tileCode); // Store the tile code in the level data array
			++column; // Move to the next column
		}
		if (column > 0) // Update column count if this row has more columns
		{
			if (columnCount == 0)
			{
				columnCount = column; // Set column count for the first row
			}
			else if (column != columnCount)
			{
				std::cerr << "Inconsistent number of columns in level file: " << file << std::endl;
				return false; // Inconsistent row lengths
			}
			++rowCount;
		}
	}

	if (rowCount * columnCount > MAX_BRICKS)
	{
		std::cerr << "Level data exceeds maximum number of bricks: " << MAX_BRICKS << std::endl;
		return false;
	}

	if (rowCount == 0 || columnCount == 0)
	{
		std::cerr << "No valid level data found in file: " << file << std::endl;
		return false;
	}

	unsigned int* levelData = new unsigned int[MAX_BRICKS]();
	std::copy(tmpData.begin(), tmpData.end(), levelData); // Copy data to levelData

	m_RowCount = rowCount; // Set the number of rows
	m_ColumnCount = columnCount; // Set the number of columns

	// Initialize the level with the loaded data
	Initialize(levelData, viewportWidth, viewportHeight);

	delete[] levelData; // Clean up the allocated memory
	return true; // Successfully loaded the level
}

void GameLevel::Draw(const std::unique_ptr<SpriteRenderer>& renderer) const
{
	for (unsigned int i = 0; i < Size(); ++i)
	{
		if (m_Bricks[i] == nullptr) continue; // Skip if the brick pointer is null
		if (m_Bricks[i]->IsDestroyed()) continue; // SKip if the brick is destroyed
		m_Bricks[i]->Draw(renderer); // Draw each brick using the sprite renderer
	}
}

bool GameLevel::IsCompleted() const
{
	for (unsigned int i = 0; i < Size(); ++i)
	{
		if (!m_Bricks[i]) continue; // Skip if the brick pointer is null
		if (!m_Bricks[i]->IsDestroyed() && !m_Bricks[i]->IsSolid())
		{
			return false; // If any brick is not destroyed and not solid, the level is not completed
		}
	}

	return true; // All bricks are either destroyed or solid, level is completed
}

void GameLevel::Initialize(const unsigned int* levelData, unsigned int viewportWidth, unsigned int viewportHeight)
{
	// Retrieve textures
	auto solidBrickTexture = ResourceManager::Instance().GetTexture("brick_solid");
	auto brickTexture = ResourceManager::Instance().GetTexture("brick");

	// Calculate dimensions
	float unitWidth = static_cast<float>(viewportWidth) / m_ColumnCount;
	float unitHeight = static_cast<float>(viewportHeight) / m_RowCount;
	m_Bricks = new GameObject*[MAX_BRICKS](); // Allocate memory for bricks

	unsigned int brickCount = 0;
	for (unsigned int row = 0; row < m_RowCount; ++row)
	{
		for (unsigned int column = 0; column < m_ColumnCount; ++column)
		{
			unsigned int index = row * m_ColumnCount + column;
			unsigned int tileCode = levelData[index];
			if (index > MAX_BRICKS)
			{
				std::cerr << "Index out of bounds: " << index << " for level data size: " << MAX_BRICKS << std::endl;
				return; // Skip invalid indices
			}

			if (tileCode == 1) // If the tile code is greater than 1, create an unbreakable brick
			{
				glm::vec2 position(column * unitWidth, row * unitHeight);
				glm::vec2 size(unitWidth, unitHeight);
				GameObject* brick = new GameObject(
					position, size, solidBrickTexture, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f));
				brick->SetSolid(true); // Set the brick as solid
				m_Bricks[index] = brick; // Store the brick in the array

				brickCount++; // Increment the count of bricks
			}
			else if (tileCode > 1) // If greater than 1, create a breakable brick with dynamic color based on the code
			{
				glm::vec4 color(1.0f); // Default color : white
				if (tileCode == 2)
					color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
				else if (tileCode == 3)
					color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
				else if (tileCode == 4)
					color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
				else if (tileCode == 5)
					color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

				glm::vec2 position(column * unitWidth, row * unitHeight);
				glm::vec2 size(unitWidth, unitHeight);
				m_Bricks[index] = new GameObject(
					position, size, brickTexture, 0.0f, color, glm::vec2(0.0f, 0.0f));

				brickCount++; // Increment the count of bricks
			}
		}
	}

	std::cout << "Initialized level with " << brickCount << " bricks." << std::endl;
}