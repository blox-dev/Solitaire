#include "GameEngine.h"
#include "MenuState.h"

int main(int argc, char** argv)
{
	GameEngine game;

	// initialize the engine
	game.Init("Engine Test v1.0", 800, 600, 0);

	// load the intro
	game.ChangeState(MenuState::Instance());

	// main loop
	while (game.Running())
	{
		game.HandleEvents();
		game.Update();
		game.Draw();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}