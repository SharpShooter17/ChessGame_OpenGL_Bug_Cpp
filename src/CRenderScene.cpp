#include "CRenderScene.h"
#include <SFML/OpenGL.hpp>
#include <OGLcolors.h>
#include <cstdio>
#include <StopWatch.h>

const GLfloat light_position[] =       { 0.0f, -2.0f, 0.0f, 1.0f };
const GLfloat light_direction[] =      { 0.0f, 0.0f, 0.0f };
const GLfloat light_ambient[] =        { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat light_diffuse[] =        { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] =       { 1.0f, 1.0f, 1.0f, 1.0f };

const GLfloat light_model_ambient[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

const GLfloat mat_ambient[]    =       { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    =       { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   =       { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] =       { 100.0f };

CRenderScene::CRenderScene( sf::RenderWindow *papp,
                           unsigned (*tab)[SizeModels],
                           unsigned (*tab1)[SizeTextures] ) :
    ViewLeft( -9.0f ),
    ViewRight( 9.0f ),
    ViewTop( 9.0f ),
    ViewBottom( -9.0f ),
    ViewNear( 50.0f ),
    ViewFar( 80.0f ),
    pApp( papp ),
    modele( tab ),
    textury( tab1 )
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    glHint(GL_FOG_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
    glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, 5 );        ///od  0 do 128
    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

    glLightModelf( GL_LIGHT_MODEL_LOCAL_VIEWER, 0 );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, light_model_ambient ); /// kolor swiatla otaczajacego
    glLightModelf(0x81F8, 0x81FA);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
}

void CRenderScene::Render( int mode )
{
    glClearColor( 0.0f, 0.2f, 0.3f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glPushMatrix();

    glTranslatef( 0.0f, 0.0f, - ( ViewNear + ViewFar ) / 2.0f );
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    if ( *bWyborPionkaZbitego )
    {
         PionkiDoWyboruPoZbiciu();
    }

    if ( *pRuchMa == PIONEK_KOLOR_BIALY )
    {
        glTranslatef( 8.0f, 5.0f, -7.0f );
        glRotatef( 55.0f, 1.0f, 0.0f, 0.0f );
        glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );
        glRotatef( 180.0f, 0.0f, 1.0f, 0.0f );
    } else
    {
        glTranslatef( -8.0f, -8.0f, 2.0f );
        glRotatef( 55.0f, 1.0f, 0.0f, 0.0f );
        glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );
    }

    glScalef( 2.0f, 2.0f, 2.0f );

    Pionki();
    Podswietlenie();
    PokazZbitePionki();

    glPopMatrix();

    glFlush();
    pApp->display();
}

int CRenderScene::Selekcja( int x, int y )
{
    if ( *bWyborPionkaZbitego )
    {
        WYBIERZ = WYBIERZ_PIONEK_ZBITY;
    } else
    {
        WYBIERZ = WYBIERZ_PIONEK;
    }

    const int BUFFER_LENGTH = 64;
    GLuint select_buffer[ BUFFER_LENGTH ];
    glSelectBuffer( BUFFER_LENGTH, select_buffer );
    int viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );
    int width = viewport[ 2 ];
    int height = viewport[ 3 ];
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix( x, height - y, 2, 2, viewport );
    glFrustum( ViewLeft, ViewRight, ViewBottom, ViewTop, ViewNear, ViewFar );
    glRenderMode( GL_SELECT );
    Render( GL_SELECT );
    GLint hits = glRenderMode( GL_RENDER );
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    ///Pole z pionkiem!
    if ( hits > 0 ) return select_buffer[ 4 ];
    else return -1;
}

void CRenderScene::PionkiDoWyboruPoZbiciu()
{
    static CStopWatch timer;
    static float rot = 0.0f;

    rot += timer.GetElapsedSeconds() * 25.0f;
    timer.Reset();

    if ( rot > 360.0f )
        rot = 0.0f;


    glInitNames();
    glPushName( 0 );
    glLoadName( 1 );

    glEnable( GL_TEXTURE_2D );

    glPushMatrix();

    glPushMatrix();
    glPushName( WIEZA );

    if ( *pRuchMa == PIONEK_KOLOR_BIALY ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 1)) );
    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 1 + 6)) );

    glTranslated( -3.0f, 0.0f, 0.0f );
    glRotatef( rot, 0.0f, 1.0f, 0.0f );

    glCallList( *(*modele + 0) );

    glPopName();
    glPopMatrix();

    glPushMatrix();
    glPushName( SKOCZEK );

    if ( *pRuchMa == PIONEK_KOLOR_BIALY ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 3)) );
    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 3 + 6)) );

    glTranslated( -1.0f, 0.0f, 0.0f );
    glRotatef( rot, 0.0f, 1.0f, 0.0f );

    glCallList( *(*modele + 2) );

    glPopName();
    glPopMatrix();

    glPushMatrix();
    glPushName( KONIK );

    if ( *pRuchMa == PIONEK_KOLOR_BIALY ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 2)) );
    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 2 + 6)) );

    glTranslated( 1.0f, 0.0f, 0.0f );
    glRotatef( rot, 0.0f, 1.0f, 0.0f );

    glCallList( *(*modele + 1) );

    glPopName();
    glPopMatrix();

    glPushMatrix();
    glPushName( DAMA );

    if ( *pRuchMa == PIONEK_KOLOR_BIALY ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 4)) );
    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 4 + 6)) );

    glTranslated( 3.0f, 0.0f, 0.0f );
    glRotatef( rot, 0.0f, 1.0f, 0.0f );

    glCallList( *(*modele + 3) );

    glPopName();
    glPopMatrix();

    glDisable( GL_TEXTURE_2D );

    glPopName();

    glPopMatrix();

    WYBIERZ = WYBIERZ_PIONEK;
}

void CRenderScene::PokazZbitePionki()
{
    static CStopWatch timer;
    static float rot = 0.0f;

    rot += timer.GetElapsedSeconds() * 25.0f;
    timer.Reset();

    if ( rot > 360.0f )
        rot = 0.0f;

    int ZbiteBiale = 0;
    int ZbiteCzarne = 0;

    if ( Zbite->size() != 0 )
    {
        for ( int i = 0; i < Zbite->size(); ++i )
        {
            glPushMatrix();

            bool bBialy;
            glColor4fv( White );

            switch ( (Zbite->begin() + i)->KOLOR )
            {
            case NO_COLOR:
                break;
            case PIONEK_KOLOR_CZARNY:
                bBialy = false;
                glTranslatef( ZbiteCzarne, 0.0f, - 0.5f );
                ZbiteCzarne++;
                break;
            case PIONEK_KOLOR_BIALY:
                bBialy = true;
                glTranslatef( 8.0 - ZbiteBiale, 0.0f, 8.5f );
                ZbiteBiale++;
                break;
            default:
                break;
            }

            glRotatef( rot, 0.0f, 1.0f, 0.0f );

            glEnable( GL_TEXTURE_2D );

            switch ( (Zbite->begin() + i)->ktoTo )
                {
                case WIEZA:
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 1)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 1 + 6)) );

                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 0) );
                    break;
                case KONIK:
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 2)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 2 + 6)) );

                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 1) );
                    break;
                case SKOCZEK:
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 3)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 3 + 6)) );

                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 2) );
                    break;
                case DAMA:
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 4)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 4 + 6)) );

                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 3) );
                    break;
                default:
                    break;
                }

            glDisable( GL_TEXTURE_2D );

            glPopMatrix();
        }
    }
}

void CRenderScene::Pionki()
{
    if ( WYBIERZ == WYBIERZ_PIONEK )
    {
        glInitNames();

        glPushName( 0 );
        glLoadName( 1 );
    }

    glPushMatrix();
    for ( int i = 0; i < 8; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            glPushMatrix();

            if ( WYBIERZ == WYBIERZ_PIONEK )
                glPushName( ( *( *pSzachownica + i ) + j )->id  );

            if ( ( *( *pSzachownica + i ) + j )->bCzyPionekJest == true )
            {
                glPushMatrix();

                bool bBialy;
                glColor4fv( White );
                switch ( ( *( *pSzachownica + i ) + j )->Pionek.KOLOR )
                {
                case PIONEK_KOLOR_BIALY:
                    bBialy = true;
                    break;
                case PIONEK_KOLOR_CZARNY:
                    bBialy = false;
                    break;
                }

                switch ( (*( *pSzachownica + i ) + j )->Pionek.ktoTo )
                {
                case PIONEK:
                    glEnable( GL_TEXTURE_2D );
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 0)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 0 + 6)) );

                    glTranslatef( i + 0.5f, 0.0, j + 0.5f );
                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 5) );

                    glDisable( GL_TEXTURE_2D );
                    break;
                case WIEZA:
                    glEnable( GL_TEXTURE_2D );
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 1)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 1 + 6)) );

                    glTranslatef( i + 0.5f, 0.0, j + 0.5f );
                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 0) );

                    glDisable( GL_TEXTURE_2D );
                    break;
                case KONIK:
                    glEnable( GL_TEXTURE_2D );
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 2)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 2 + 6)) );

                    glTranslatef( i + 0.5f, 0.0, j + 0.5f );
                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 1) );

                    glDisable( GL_TEXTURE_2D );
                    break;
                case SKOCZEK:
                    glEnable( GL_TEXTURE_2D );
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 3)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 3 + 6)) );

                    glTranslatef( i + 0.5f, 0.0, j + 0.5f );
                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 2) );

                    glDisable( GL_TEXTURE_2D );
                    break;
                case DAMA:
                    glEnable( GL_TEXTURE_2D );
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 4)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 4 + 6)) );

                    glTranslatef( i + 0.5f, 0.0, j + 0.5f );
                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 3) );

                    glDisable( GL_TEXTURE_2D );
                    break;
                case KROL:
                    glEnable( GL_TEXTURE_2D );
                    if ( bBialy ) glBindTexture( GL_TEXTURE_2D, (*(*textury + 5)) );
                    else glBindTexture( GL_TEXTURE_2D, (*(*textury + 5 + 6)) );

                    glTranslatef( i + 0.5f, 0.0, j + 0.5f );
                    glScalef( 0.3f, 0.3f, 0.3f );
                    glCallList( *(*modele + 4) );

                    glDisable( GL_TEXTURE_2D );
                    break;
                default:
                    break;
                }
                glPopMatrix();
            }

            ///Rysuje szachownice
            if ( ( i + j ) % 2 != 0 ) glColor4fv( Black );
            else glColor4fv( Silver );

            glTranslated( i, 0.0, j );

            glEnable( GL_POLYGON_SMOOTH );
            glBlendFunc( GL_SRC_ALPHA_SATURATE, GL_ONE );

            glBegin( GL_QUADS );
            glVertex3f( 0.0, 0.0, 0.0 );
            glVertex3f( 0.0, 0.0, 1.0 );
            glVertex3f( 1.0, 0.0, 1.0 );
            glVertex3f( 1.0, 0.0, 0.0 );
            glEnd();

            glDisable( GL_POLYGON_SMOOTH );

            if ( WYBIERZ == WYBIERZ_PIONEK )
                glPopName();

            glPopMatrix();
        }
    }
    glPopMatrix();
}

void CRenderScene::Podswietlenie()
{
    glEnable( GL_BLEND );
    glEnable( GL_POLYGON_SMOOTH );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glColor4f( YellowGreen[0], YellowGreen[1], YellowGreen[2], 0.5f );

    for ( int i = 0; i < pDozwolonyRuch->size(); ++i ) {
        std::vector <int>::iterator it = pDozwolonyRuch->begin() + i;
        int x = *it / 8;
        int z = *it % 8;
        glPushMatrix();
        glTranslated( x, 0.001, z );

        glBegin( GL_QUADS );
        glVertex3f( 0.0, 0.0, 0.0 );
        glVertex3f( 0.0, 0.0, 1.0 );
        glVertex3f( 1.0, 0.0, 1.0 );
        glVertex3f( 1.0, 0.0, 0.0 );
        glEnd();

        glPopMatrix();
    }

    glDisable( GL_POLYGON_SMOOTH );
    glDisable( GL_BLEND );
}

CRenderScene::~CRenderScene()
{
}

void CRenderScene::ZmienRozmiarOkna( int w, int h )
{
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( ViewLeft, ViewRight, ViewBottom, ViewTop, ViewNear, ViewFar );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}
