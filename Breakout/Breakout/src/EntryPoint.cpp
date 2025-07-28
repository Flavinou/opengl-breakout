#include "Game.h"

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

int main(int argc, char** argv)
{
    auto* game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");
    game->Run();
    delete game;
}