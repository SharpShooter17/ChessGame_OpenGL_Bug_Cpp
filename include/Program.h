#ifndef PROGRAM_H
#define PROGRAM_H

#include "CLogika.h"
#include "CRenderScene.h"

#define SizeModels      6
#define SizeTextures    12

class Program
{
    public:

        Program();
        ~Program();

        void Loop();

        unsigned int modele[ SizeModels ];
        unsigned int textures[ SizeTextures ];

    protected:

    private:

        sf::RenderWindow app;

        enum {
                MENU,
                GAME,
                EXIT
             } STATE;

        void MainLoop();
        void GameLoop();

        int WindowWidth, WindowHeight;
};

#endif // PROGRAM_H
