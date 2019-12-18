#include "Game.h"
#include <entityx/entityx.h>

void main(int argc, void** argv[]){
	// Program entry point.
	{
		entityx::EntityX entityX;
		entityX.systems.configure();

		auto entity = entityX.entities.create();

		Game game;
		while(!game.GetWindow()->IsDone()){
			game.Update();
			game.Render();
			game.LateUpdate();
		}
	}
	system("PAUSE");
}