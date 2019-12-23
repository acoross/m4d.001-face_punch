#include <entityx/entityx.h>
#include "Game.h"

void main(int argc, void** argv[]){
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