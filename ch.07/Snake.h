#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

struct SnakeSegment
{
	SnakeSegment(int x, int y)
		: position(x, y)
	{}

	sf::Vector2i position;
};

using SnakeContainer = std::vector<SnakeSegment>;

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

class Snake {
public:
	Snake(int blockSize);
	~Snake();

	void Tick(); // Update method.

	// Helper methods.
	bool HasLost() const;
	void Lose(); // Handle losing here.
	void ToggleLost();
	
	void Extend(); // Grow the snake.
	void Reset(); // Reset to starting position.
	void Move(); // Movement method.
	void Cut(int l_segments); // Method for cutting snake.
	sf::Vector2i GetPosition() const;
	void IncreaseScore();
	Direction GetPhysicalDirection();

	void CheckCollision(); // Checking for collisions.

	void Render(sf::RenderWindow& l_window);
	
	SnakeContainer m_snakeBody; // Segment vector.
	int m_size; // Size of the graphics.
	Direction m_dir = Direction::None; // Current direction.
	int m_speed; // Speed of the snake.
	int m_lives; // Lives.
	int m_score; // Score.
	bool m_lost; // Losing state.
	sf::RectangleShape m_bodyRect; // Shape used in rendering.
};