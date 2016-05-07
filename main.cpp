#include "Program.h"

int main()
{
    Program Game;
    Game.Loop();
    Game.~Program();

    return 0;
}
