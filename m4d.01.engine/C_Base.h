#pragma once
#include <iostream>
#include <sstream>

class C_Base{
public:
	C_Base() {}
	virtual ~C_Base(){}

	friend std::stringstream& operator >>(
		std::stringstream& l_stream, C_Base& b)
	{
		b.ReadIn(l_stream);
		return l_stream;
	}

	virtual void ReadIn(std::stringstream& l_stream) = 0;
};