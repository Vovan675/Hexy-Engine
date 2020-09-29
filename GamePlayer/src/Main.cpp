#include "Hexy.h"
#include "GameLayer.h"

class Game : public Hexy::Application
{
public:
	Game()
	{
		PushLayer(new GameLayer());
	}
};

int main()
{
	Game* game = new Game();
	game->Run();
	return 0;
}