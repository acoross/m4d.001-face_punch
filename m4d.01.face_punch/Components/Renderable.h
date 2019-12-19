#pragma once

#include <cstdint>
#include "../C_Drawable.h"
//#include "Meta.h"

enum class RenderableType
{
	Circle,
	Rectangle,
};


class Renderable : public C_Drawable {
public:
	Renderable() {}
	Renderable(float size) : circle(size) {}
	Renderable() {}

	~Renderable() { }

	void ReadIn(std::stringstream& l_stream) override {
		float radius = 0;
		l_stream >> radius;
		circle = sf::CircleShape(radius);
	}

	void UpdatePosition(const sf::Vector2f& l_vec) override {
		circle.setPosition(l_vec);
	}

	const sf::Vector2u& GetSize() override {
		float radius = circle.getRadius();
		return sf::Vector2u{ (unsigned int)radius, (unsigned int)radius };
	}

	void Draw(sf::RenderWindow* l_wind) override {
		l_wind->draw(circle);
	}

	int border = 1;
	sf::CircleShape circle;
};

//
//struct UIRenderable
//{
//	UIRenderable()
//	{}
//
//	UIRenderable(float2 pos, float size)
//		: UIRenderable(pos, size, size)
//	{}
//
//	UIRenderable(float2 pos, float width, float height)
//		: pos(pos), width(width), height(height)
//	{}
//
//	float2 pos;
//	float width = 1.f;
//	float height = 1.f;
//	int border = 1;
//
//	RenderableType type = RenderableType::Rectangle;
//};
//
//namespace meta
//{
//	template <>
//	inline auto registerMembers<Renderable>()
//	{
//		return members(
//			member("width", &Renderable::width),
//			member("height", &Renderable::height),
//			member("border", &Renderable::border),
//			member("type", &Renderable::type)
//		);
//	}
//}