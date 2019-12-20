#pragma once
#include <SFML/System/Vector2.hpp>
#include "C_Base.h"

class C_Position : public C_Base{
public:
	C_Position(): C_Base(), m_elevation(0){}
	~C_Position(){}

	void ReadIn(std::stringstream& l_stream) override
	{
		l_stream >> m_position.x >> m_position.y >> m_elevation >> m_angle;
	}

	const sf::Vector2f& GetPosition(){ return m_position; }
	unsigned int GetElevation(){ return m_elevation; }
	float GetAngle() { return m_angle; }

	void SetPosition(float l_x, float l_y){
		m_position = sf::Vector2f(l_x,l_y);
	}

	void SetPosition(const sf::Vector2f& l_vec){
		m_position = l_vec;
	}

	void SetElevation(unsigned int l_elevation){
		m_elevation = l_elevation;
	}

	void MoveBy(float l_x, float l_y){
		m_position += sf::Vector2f(l_x,l_y);
	}

	void MoveBy(const sf::Vector2f& l_vec){
		m_position += l_vec;
	}

	void SetAngle(float angle)
	{
		m_angle = angle;
	}

private:
	sf::Vector2f m_position;
	unsigned int m_elevation;
	float m_angle;
};