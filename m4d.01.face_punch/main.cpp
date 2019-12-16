#include <iostream>
#include "Game.h"
#include "SFML/Audio.hpp"

void main(int argc, void** argv[])
{
	sf::Music music;
	if (!music.openFromFile("the_cheetahmen.ogg"))
	{
		std::cout << "fail load cheetahmen" << std::endl;
	}

	music.play();

	// Program entry point.
	Game game; // Creating our game object.
	while (!game.GetWindow()->IsDone()) 
	{
		game.HandleInput();
		game.Update();
		game.Render();
		game.RestartClock();
	}

	music.stop();
}
