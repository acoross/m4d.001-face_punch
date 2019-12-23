#pragma once

#include <SFML/Graphics.hpp>
#include "C_Base.h"

class C_Drawable : public C_Base{
public:
	C_Drawable() : C_Base()
	{
		float radius = 1;
		SetSize(radius);
	}

	virtual ~C_Drawable(){}

	virtual void ReadIn(std::stringstream& l_stream) override
	{
		float radius;
		l_stream >> radius;
		circle.setRadius(radius);
	}

	void SetSize(float radius)
	{
		circle.setRadius(radius);
		circle.setOrigin(radius, radius);
	}

	void SetColor(const sf::Color color)
	{
		circle.setFillColor(color);
		circle.setOutlineColor(color);
	}

	/// angle: degree
	void UpdatePosition(const sf::Vector2f& l_vec)
	{
		circle.setPosition(l_vec);
	}

	float GetRadius() const
	{
		return circle.getRadius();
	}

	const sf::FloatRect GetGlobalBound() const
	{
		return circle.getGlobalBounds();
	}

	void Draw(sf::RenderWindow* l_wind)
	{
		l_wind->draw(circle);
	}

private:
	sf::CircleShape circle;
};