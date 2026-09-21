#include "SDL3/SDL_main.h"
#include "SDL3/SDL.h"
#include "GameState.h"
#include "GameStateManager.h"
#include "Menu.h"
#include <time.h>

int main(int argc, char* argv[])
{
	srand(time(NULL));
	GameStateManager* manager = new GameStateManager();
	manager->SetState(new Menu()); 
	manager->GameLoop();
	delete manager;
	return 0;
}
