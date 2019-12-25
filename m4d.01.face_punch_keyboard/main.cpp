#include <entityx/entityx.h>
#include "Game.h"
#include <SFML/Network/Packet.hpp>
#include <strstream>

class Foo
{
public:
	template <class Ar>
	void Serialize(Ar& ar)
	{
		ar(a);
		ar(b);
	}

	int a = 0;
	bool b = false;
};

class Writer
{
public:
	template <class T>
	Writer& operator()(T& val)
	{
		stream << val;
		stream << ' ';
		return *this;
	}

	std::stringstream stream;
};

class Reader
{
public:
	template <class T>
	Reader& operator()(T& val)
	{
		stream >> val;
		return *this;
	}

	std::stringstream stream;
};

class PacketWriter
{
public:
	template <class T>
	PacketWriter& operator()(T& val)
	{
		packet << val;
		return *this;
	}

	sf::Packet packet;
};

class PacketReader
{
public:
	template <class T>
	PacketReader& operator()(T& val)
	{
		packet >> val;
		return *this;
	}

	sf::Packet packet;
};

void main(int argc, void** argv[]){
	Foo foo;
	foo.a = 2398110;
	foo.b = true;
	
	PacketWriter packetWriter;
	foo.Serialize(packetWriter);

	const void* packetData = packetWriter.packet.getData();
	const int size = packetWriter.packet.getDataSize();

	Writer writer;
	foo.Serialize(writer);

	const auto str = writer.stream.str();

	std::cout << "packet size: " << size << ", str len: " << str.length() << std::endl;
	std::cout << str.c_str() << std::endl;

	Reader reader;
	reader.stream = std::stringstream(str);

	Foo foo2;
	foo2.Serialize(reader);

	std::cout << "foo2: " << foo2.a << ", " << foo2.b << std::endl;

	PacketReader packetReader;
	packetReader.packet = packetWriter.packet;

	Foo foo3;
	foo3.Serialize(packetReader);

	std::cout << "foo3: " << foo3.a << ", " << foo3.b << std::endl;

	// Program entry point.
	{
		sf::Clock clock;
		clock.restart();

		Game game;
		while(!game.GetWindow()->IsDone()){
			if (clock.getElapsedTime().asMilliseconds() > 16)
			{
				clock.restart();

				sf::Clock updateClock;
				updateClock.restart();
				game.Update();
				auto updateElapsed = updateClock.restart().asMilliseconds();

				game.Render();
				auto renderElapsed = updateClock.restart().asMilliseconds();

				game.LateUpdate();

				if (updateElapsed > 5 || renderElapsed > 5)
				{
					std::cout << "update: " << updateElapsed
						<< ", render: " << renderElapsed << std::endl;
				}
			}
		}
	}
	system("PAUSE");
}