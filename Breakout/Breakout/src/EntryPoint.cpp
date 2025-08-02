#include "Application.h"

constexpr unsigned int SCREEN_WIDTH = 1280;
constexpr unsigned int SCREEN_HEIGHT = 720;

int main(int argc, char** argv)
{
    auto* app = new Application(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");
    app->Run();
    delete app;
}