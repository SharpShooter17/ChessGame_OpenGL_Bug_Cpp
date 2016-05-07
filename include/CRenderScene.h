#ifndef CRENDERSCENE_H
#define CRENDERSCENE_H

#include <SFML/Graphics.hpp>
#include "CLogika.h"

#define SizeModels      6
#define SizeTextures    12

class CRenderScene
{
    public:
        CRenderScene( sf::RenderWindow *papp, unsigned (*tab)[ SizeModels ], unsigned (*tab1)[ SizeTextures ] );
        ~CRenderScene();

        void ZmienRozmiarOkna( int w, int h );
        void Render( int mode );
        int Selekcja( int x, int y );

        Pola (*pSzachownica)[8][8];
        std::vector <int> *pDozwolonyRuch;
        std::vector <sPionek> *Zbite;
        bool *bWyborPionkaZbitego;
        PIONEK_KOLOR *pRuchMa;

    private:

        sf::RenderWindow *pApp;

        float ViewLeft;
        float ViewRight;
        float ViewTop;
        float ViewBottom;
        float ViewNear;
        float ViewFar;

        void Pionki();
        void Podswietlenie();
        void PokazZbitePionki();
        void PionkiDoWyboruPoZbiciu();

        unsigned int (*modele)[SizeModels];
        unsigned int (*textury)[SizeTextures];

        enum { WYBIERZ_PIONEK_ZBITY,
               WYBIERZ_PIONEK
               } WYBIERZ;
};

#endif // CRENDERSCENE_H
