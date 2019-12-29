#pragma once

#include <SFML/Graphics.hpp>

class C_Drawable
{
public:
	C_Drawable()
	{
		float radius = 1;
		SetSize(radius);
	}

	~C_Drawable(){}

	void SetSize(float radius)
	{
		radius_ = radius;

		if (auto texture = sprite.getTexture())
		{
			const auto size = texture->getSize();

			const auto scale = sf::Vector2f(2 * radius / size.x, 2 * radius / size.y);
			sprite.setScale(scale);
		}
	}

	void SetRotation(const float angle)
	{
		sprite.setRotation(angle);
	}

	void SetTexture(const sf::Texture& texture)
	{
		sprite.setTexture(texture);
		const auto size = texture.getSize();
		sprite.setOrigin(size.x / 2.f, size.y / 2.f);

		SetSize(radius_);
	}

	/// angle: degree
	void UpdatePosition(const sf::Vector2f& l_vec)
	{
		sprite.setPosition(l_vec);
	}

	float GetRadius() const
	{
		return radius_;
	}

	const sf::FloatRect GetGlobalBound() const
	{
		return sprite.getGlobalBounds();
	}

	void Draw(sf::RenderWindow* l_wind)
	{
		l_wind->draw(sprite);
	}

private:
	float radius_;
	sf::Sprite sprite;
};