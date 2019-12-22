#pragma once

#include <SFML/System.hpp>
#include <cmath>

inline float length(sf::Vector2f vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

inline float dist(sf::Vector2f vec1, sf::Vector2f vec2)
{
	return length(vec1 - vec2);
}

inline sf::Vector2f norm(sf::Vector2f vec)
{
	float len = length(vec);
	if (len > std::numeric_limits<float>::epsilon())
	{
		return vec / len;
	}

	return vec;
}