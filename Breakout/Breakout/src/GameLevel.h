#pragma once

#include "GameObject.h"

#include <vector>
#include <string>

class GameLevel
{
public:
	static constexpr unsigned int MAX_BRICKS = 1024; // Hard maximum number of bricks in a level

	GameLevel() = default; // Default constructor
	GameLevel(const std::string& file, unsigned int viewportWidth, unsigned int viewportHeight);
	~GameLevel();

	bool Load(const std::string& file, unsigned int viewportWidth, unsigned int viewportHeight);
	void Draw(const std::unique_ptr<SpriteRenderer>& renderer) const;
	bool IsCompleted() const;

	const std::string& GetFileName() { return m_FileName; }
	GameObject* GetBrick(unsigned int index) const { return m_Bricks[index]; }
	unsigned int GetRowCount() const { return m_RowCount; }
	unsigned int GetColumnCount() const { return m_ColumnCount; }
	unsigned int Size() const 
	{ 
		return m_RowCount * m_ColumnCount;
	}
private:
	void Initialize(const unsigned int* levelData, unsigned int viewportWidth, unsigned int viewportHeight);
private:
	GameObject** m_Bricks = nullptr; // List of bricks in the level
	unsigned int m_RowCount = 0, m_ColumnCount = 0;
	std::string m_FileName; // Useful if we want to reload the level
};

