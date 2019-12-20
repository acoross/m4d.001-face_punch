#pragma once

#include <SFML/Graphics.hpp>
#include "C_Base.h"

class C_Drawable : public C_Base{
public:
	C_Drawable() : C_Base()
	{
	}

	virtual ~C_Drawable(){}

	virtual void ReadIn(std::stringstream& l_stream) override
	{
		float x, y;
		l_stream >> x >> y;
		circle.setRadius(1);
		circle.setScale(x, y);
	}

	void SetSize(const sf::Vector2f& size)
	{
		circle.setRadius(1);
		circle.setScale(size);

		circle.setOrigin(size.x / 2, size.y / 2);
	}

	void SetColor(const sf::Color color)
	{
		circle.setFillColor(color);
		circle.setOutlineColor(color);
	}

	/// angle: degree
	void UpdatePosition(const sf::Vector2f& l_vec, const float angle)
	{
		circle.setPosition(l_vec);
		circle.setRotation(angle);
	}

	const sf::Vector2u& GetSize()
	{
		float radius = circle.getRadius();
		auto scale = circle.getScale();

		return sf::Vector2u(scale * radius);
	}

	void Draw(sf::RenderWindow* l_wind)
	{
		l_wind->draw(circle);
	}

private:
	sf::CircleShape circle;
};