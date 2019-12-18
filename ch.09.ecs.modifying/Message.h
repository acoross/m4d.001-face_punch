#pragma once
#include "entityx/Entity.h"

using MessageType = unsigned int;
struct TwoFloats{ float m_x; float m_y; };

struct Message{
	Message(const MessageType& l_type) : m_type(l_type){}

	MessageType m_type;
	int m_sender;
	entityx::Entity m_receiver;

	union{
		TwoFloats m_2f;
		bool m_bool;
		int m_int;
	};
};