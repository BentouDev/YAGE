#include <GameLib.h>
#include <SimpleGameRunner.h>

int main(int argc, const char* args[])
{
    SimpleGameRunner<GameLib> game;
    game.Run();
}