#include "Program.h"
#include "../OBJLoader/loadObj.h"
#include "windows.h"

Program::Program() : WindowWidth( 1024 ),
                     WindowHeight( 768 ),
                     STATE( MENU )
{
    const char *chModele[ SizeModels ] = { "models/wieza.obj",
                                           "models/konik.obj",
                                           "models/skoczek.obj",
                                           "models/dama.obj",
                                           "models/krol.obj",
                                           "models/pionek.obj" };

    for ( int i = 0; i < SizeModels; ++i )
        load_obj( chModele[i], modele[i] );

    const char *chTextury[ SizeTextures ] = {
                                                "textures/woodlig0.tga",
                                                "textures/woodlig1.tga",
                                                "textures/woodlig2.tga",
                                                "textures/woodlig3.tga",
                                                "textures/woodlig4.tga",
                                                "textures/woodlig5.tga",
                                                "textures/wooddar0.tga",
                                                "textures/wooddar1.tga",
                                                "textures/wooddar2.tga",
                                                "textures/wooddar3.tga",
                                                "textures/wooddar4.tga",
                                                "textures/wooddar5.tga"
                                            };

    GLsizei width, height;
    GLenum format, type;
    GLvoid *pixels;

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    for ( int i = 0; i < SizeTextures; ++i ){

        if ( !load_targa( chTextury[i], width, height, format, type, pixels) ){
        }

        glGenTextures(1,&textures[ i ]);
        glBindTexture(GL_TEXTURE_2D, textures[ i ]);
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,height,format,type,pixels);

        delete [] (unsigned char *)pixels;
    }
}

Program::~Program()
{
    app.close();
}

void Program::Loop()
{
    while ( STATE != EXIT )
    {
        switch ( STATE )
        {
        case MENU:
            MainLoop();
            break;
        case GAME:
            GameLoop();
            break;
        case EXIT:
            return;
            break;
        }
    }
}

void Program::MainLoop()
{
    int result = MessageBox( NULL, "Czy chcesz zagrac?", "Chess-game",
                            MB_ICONQUESTION | MB_YESNO );

    if ( result == IDNO )
        STATE = EXIT;
    else if ( result == IDYES )
        STATE = GAME;
}

void Program::GameLoop()
{
    CLogika Logika;

    sf::ContextSettings settings;

    settings.antialiasingLevel = 16;
    settings.depthBits = 32;
    settings.stencilBits = 32;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::RenderWindow app( sf::VideoMode(WindowWidth, WindowHeight),
                "Chess", sf::Style::Default, settings );

    app.setVerticalSyncEnabled( true );

    CRenderScene RenderScene( &app, &modele, &textures );

    RenderScene.ZmienRozmiarOkna( WindowWidth, WindowHeight );
    RenderScene.pSzachownica = &Logika.Szachownica;
    RenderScene.pDozwolonyRuch = &Logika.DozwolonyRuchNaPoleId;
    RenderScene.Zbite = &Logika.Zbite_pionki;
    RenderScene.bWyborPionkaZbitego = &Logika.bPionekDotarlDoKonicaPlanszy;
    RenderScene.pRuchMa = &Logika.RuchMa;

    bool wyborPionka = false;

    while ( STATE == GAME ){
        sf::Event event;
        while (app.pollEvent( event ))
        {
            if ( ( event.type == sf::Event::Closed ) ||
                 ( event.type == sf::Event::KeyReleased &&
                   event.key.code == sf::Keyboard::Escape ) )
            {
                STATE = MENU;
                app.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                RenderScene.ZmienRozmiarOkna( event.size.width, event.size.height );
            }
            else if ( event.type == sf::Event::MouseButtonReleased ){
                if (event.mouseButton.button == sf::Mouse::Left){
                    int id = RenderScene.Selekcja( sf::Mouse::getPosition( app ).x,
                                                    sf::Mouse::getPosition( app ).y );
                    if ( id >= 0 && id <= 64 )
                        Logika.KontrolaRuchu( id );
                }
            }
        } /// Petla zdarzen

        Logika.Logika_gry();

        switch ( Logika.STATE )
        {
        case MAT:
            {
                app.close();
                LPCSTR text = Logika.RuchMa == PIONEK_KOLOR_BIALY ? "Wygraly czerwone!" : "Wygraly biale!";
                MessageBox( NULL, text, "KONIEC GRY",
                                MB_ICONINFORMATION | MB_OK );
                STATE = MENU;
                break;
            }
        case PAT:
            {
                app.close();
                MessageBox( NULL, "PAT!", "KONIEC GRY",
                                MB_ICONINFORMATION | MB_OK );
                STATE = MENU;
                break;
            }
            break;
        }

        RenderScene.Render( GL_RENDER );
    }
}
