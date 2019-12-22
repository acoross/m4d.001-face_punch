#include <entityx/entityx.h>
#include "Game.h"

void main(int argc, void** argv[]){
	// Program entry point.
	{
		Game game;
		while(!game.GetWindow()->IsDone()){
			if (game.GetElapsed().asMilliseconds() > 16)
			{
				game.Update();
				game.Render();
				game.LateUpdate();
			}
		}
	}
	system("PAUSE");
}