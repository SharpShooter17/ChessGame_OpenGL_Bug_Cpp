#include "loadObj.h"

/// struktura ze wspó³rzêdnymi wierzcho³ków

struct geometric_vertices
{
    GLfloat x, y, z, w;
};

/// struktura ze wspó³rzêdnymi tekstur

struct texture_vertices
{
    GLfloat u, v, w;
};

/// struktura ze wspó³rzêdnymi wektorów normalnych

struct vertex_normals
{
    GLfloat i, j, k;
};

/// wspó³rzêdne przestrzeni parametrycznej

struct parameter_space_vertices
{
    GLfloat u, v, w;
};

/// tablica ze wspó³rzêdnymi wierzcho³ków

std::vector < geometric_vertices > v;

/// tablica ze wspó³rzêdnymi tekstur

std::vector < texture_vertices > vt;

/// tablica ze wspó³rzênymi wektorów normalnych

std::vector < vertex_normals > vn;

/// tablica ze wspó³rzêdnymi przestrzeni parametrycznej

std::vector < parameter_space_vertices > vp;

/// definicja materia³u

struct material
{
    char name[ 256 ]; // nazwa materia³u
    GLfloat Ka[ 4 ]; // stopieñ odbicia œwiat³a otaczaj¹cego
    GLfloat Kd[ 4 ]; // stopieñ rozproszenia œwiat³a rozproszonego
    GLfloat Ks[ 4 ]; // stopieñ odbicia œwiat³a odbitego
    int illum; // model oœwietlenia
    GLfloat d; // stopieñ przezroczystoœci materia³u
    GLfloat Ns; // wyk³adnik odb³ysku œwiat³a
    char map_Kd[ 256 ]; // nazwa pliku z tekstur¹
};

// tablica z definicjami materia³ów

std::vector < material > materials;

// ustawienie domyœlnych w³aœciwoœci materia³u

void DeafultMaterial( material & mat )
{
    mat.name[ 0 ] = 0;
    mat.Ka[ 0 ] = 0.2;
    mat.Ka[ 1 ] = 0.2;
    mat.Ka[ 2 ] = 0.2;
    mat.Ka[ 3 ] = 1.0;
    mat.Kd[ 0 ] = 0.8;
    mat.Kd[ 1 ] = 0.8;
    mat.Kd[ 2 ] = 0.8;
    mat.Kd[ 3 ] = 1.0;
    mat.Ks[ 0 ] = 1.0;
    mat.Ks[ 1 ] = 1.0;
    mat.Ks[ 2 ] = 1.0;
    mat.Ks[ 3 ] = 1.0;
    mat.illum = 1;
    mat.d = 1.0;
    mat.Ns = 0.0;
    mat.map_Kd[ 0 ] = 0;
}

void load_vertex_data( const char * str )
{
    switch( str[ 1 ] )
    {
        // wspó³rzêdne wierzcho³ków
    case ' ':
        geometric_vertices tmp_v;
        tmp_v.w = 1.0;
        sscanf( str + 1, "%f %f %f %f", & tmp_v.x, & tmp_v.y, & tmp_v.z, & tmp_v.w );
        v.insert( v.end(), tmp_v );
        break;

        // wspó³rzêdne tekstur
    case 't':
        texture_vertices tmp_vt;
        tmp_vt.v = tmp_vt.w = 0.0;
        sscanf( str + 2, "%f %f %f", & tmp_vt.u, & tmp_vt.v, & tmp_vt.w );
        vt.insert( vt.end(), tmp_vt );
        break;

        // wspó³rzêdne wektorów normalnych
    case 'n':
        vertex_normals tmp_vn;
        sscanf( str + 2, "%f %f %f", & tmp_vn.i, & tmp_vn.j, & tmp_vn.k );
        vn.insert( vn.end(), tmp_vn );
        break;

        // wspó³rzêdne przestrzeni parametrycznej
    case 'p':
        parameter_space_vertices tmp_vp;
        tmp_vp.w = 1.0;
        sscanf( str + 2, "%f %f %f", & tmp_vt.u, & tmp_vt.v, & tmp_vt.w );
        vp.insert( vp.end(), tmp_vp );
        break;
    }
}

// odczyt biblioteki materia³ów

void load_materials( const char * str )
{
    // sprawdzenie czy jest to definicja materia³ów
    char tmpstr[ 512 ];
    unsigned int pos = 0;
    sscanf( str, "%s", tmpstr );
    if( !strcmp( tmpstr, "mtllib" ) )
    {
        // odczyt kolejnych plików z definicjami materia³ów
        pos += strlen( tmpstr ) + 1;
        while( pos < strlen( str ) )
        {
            // pobranie nazwy pliku
            sscanf( str + pos, "%s", tmpstr );

            // nastêpny (opcjonalny) plik z definicj¹ materia³ów
            pos += strlen( tmpstr ) + 1;

            // otwarcie pliku do odczytu
            FILE * mtl = fopen( tmpstr, "rt" );

            // sprawdzenie poprawnoœci otwarcia pliku
            if( !mtl )
                 continue;

            // struktura z danymi materia³u z wartoœciami domyœlnymi
            material mat;
            DeafultMaterial( mat );

            // odczyt danych pliku
            char mtlstr[ 512 ];
            while( !feof( mtl ) )
            {
                // pobranie linii pliku
                fgets( mtlstr, 512, mtl );

                // sprawdzenie co zawiera linia
                switch( mtlstr[ 0 ] )
                {
                    // nazwa materia³u newmtl name
                case 'n':
                    sscanf( mtlstr, "%s", tmpstr );
                    if( !strcmp( tmpstr, "newmtl" ) )
                    {
                        // sprawdzenie czy to pierwsza definicja materia³u
                        // je¿eli nie, to zapisujemy materia³ do tablicy
                        if( strlen( mat.name ) )
                        {
                            materials.insert( materials.end(), mat );

                            // ponownie wartoœci domyœlne
                            DeafultMaterial( mat );
                        }
                        sscanf( mtlstr + strlen( tmpstr ) + 1, "%s", mat.name );
                    }
                    break;

                    // stopieñ odbicia œwiat³a otaczaj¹cego (Ka)
                    // stopieñ rozproszenia œwiat³a rozproszonego (Kd)
                    // stopieñ odbicia œwiat³a odbitego (Ks)
                case 'K':
                    switch( mtlstr[ 1 ] )
                    {
                    case 'a':
                        sscanf( mtlstr + 3, "%f %f %f", mat.Ka + 0, mat.Ka + 1, mat.Ka + 2 );
                        break;

                    case 'd':
                        sscanf( mtlstr + 3, "%f %f %f", mat.Kd + 0, mat.Kd + 1, mat.Kd + 2 );
                        break;

                    case 's':
                        sscanf( mtlstr + 3, "%f %f %f", mat.Ks + 0, mat.Ks + 1, mat.Ks + 2 );
                        break;
                    }
                    break;

                    // sk³adowa alfa - przezroczystoœæ (d)
                case 'd':
                    if( mtlstr[ 1 ] == ' ' )
                    {
                        sscanf( mtlstr + 2, "%f", & mat.d );
                        mat.Ka[ 3 ] = mat.d;
                        mat.Kd[ 3 ] = mat.d;
                        mat.Ks[ 3 ] = mat.d;
                    }
                    break;

                    // wyk³adnik odb³ysku œwiat³a (Ns)
                case 'N':
                    if( mtlstr[ 1 ] == 's' && mtlstr[ 2 ] == ' ' )
                         sscanf( mtlstr + 3, "%f", & mat.Ns );

                    break;

                    // tekstura (map_Kd)
                case 'm':
                    sscanf( mtlstr, "%s", tmpstr );

                    // sprawdzenie czy to tekstura
                    if( !strcmp( tmpstr, "map_Kd" ) )
                         sscanf( mtlstr + strlen( tmpstr ) + 1, "%s", mat.map_Kd );

                    break;

                    // model oœwietlenia (illum)
                case 'i':
                    sscanf( mtlstr, "%s", tmpstr );

                    // sprawdzenie czy to model oœwietlenia
                    if( !strcmp( tmpstr, "illum" ) )
                         sscanf( mtlstr + strlen( tmpstr ) + 1, "%i", & mat.illum );

                    break;

                    // domyœlnie - pominiêcie linii pliku
                    default:
                    break;
                }
            }

            // zapisanie ostatniego materia³u do tablicy
            materials.insert( materials.end(), mat );

            // zamkniêcie pliku
            fclose( mtl );
        }
    }
}

// odczyt danych o wierzcho³kach œciany (wielok¹ta)

void load_face( const char * str )
{
    /// ustalenie iloœci i formatu danych opisuj¹cych wierzcho³ek œciany
    char tmpstr[ 512 ];
    unsigned int pos = 2;
    sscanf( str + pos, "%s", tmpstr );
    unsigned int count = 0;
    for( unsigned int i = 0; i < strlen( tmpstr ); i++ )
    if( tmpstr[ i ] == '/' )
    {
        count++;
        if( tmpstr[ i + 1 ] == '/' )
             count++;

    }

    /// zliczanie iloœci wierzcho³ków œciany
    int vertex = 0;
    while( pos < strlen( str ) )
    {
        vertex++;
        pos += strlen( tmpstr ) + 1;
        sscanf( str + pos, "%s", tmpstr );
    }

    /// pocz¹tek definiowania wielok¹ta
    switch( vertex )
    {
    case 3:
        glBegin( GL_TRIANGLES );
        break;
    case 4:
        glBegin( GL_QUADS );
        break;
        default:
        glBegin( GL_POLYGON );
        break;
    };

    /// odczyt i definiowanie danych poszczególnych wierzcho³ków
    pos = 2;
    vertex = 0;
    int iv, ivt, ivn;
    sscanf( str + pos, "%s", tmpstr );
    while( pos < strlen( str ) )
    {
        switch( count )
        {
            // f v
        case 0:
            sscanf( tmpstr, "%i", & iv );
            glVertex3f( v[ iv - 1 ].x, v[ iv - 1 ].y, v[ iv - 1 ].z );
            break;

            // f v/t
        case 1:
            sscanf( tmpstr, "%i/%i", & iv, & ivt );
            glTexCoord3f( vt[ ivt - 1 ].u, vt[ ivt - 1 ].v, vt[ ivt - 1 ].w );
            glVertex3f( v[ iv - 1 ].x, v[ iv - 1 ].y, v[ iv - 1 ].z );
            break;

            // f v/t/n
        case 2:
            sscanf( tmpstr, "%i/%i/%i", & iv, & ivt, & ivn );
            glNormal3f( vn[ ivn - 1 ].i, vn[ ivn - 1 ].j, vn[ ivn - 1 ].k );
            glTexCoord3f( vt[ ivt - 1 ].u, vt[ ivt - 1 ].v, vt[ ivt - 1 ].w );
            glVertex3f( v[ iv - 1 ].x, v[ iv - 1 ].y, v[ iv - 1 ].z );
            break;

            // f v//n
        case 3:
            sscanf( tmpstr, "%i//%i", & iv, & ivn );
            glNormal3f( vn[ ivn - 1 ].i, vn[ ivn - 1 ].j, vn[ ivn - 1 ].k );
            glVertex3f( v[ iv - 1 ].x, v[ iv - 1 ].y, v[ iv - 1 ].z );
            break;
        }

        // kolejny wierzcho³ek
        vertex++;
        pos += strlen( tmpstr ) + 1;
        sscanf( str + pos, "%s", tmpstr );
    }

    // koniec definicji wielok¹ta
    glEnd();
}

// odczyt tekstury w formacie TARGA
// filename - nazwa pliku

GLboolean load_texture( const char * filename )
{
    // zmienne u¿yte przy obs³udze plików TARGA
    GLsizei width, height;
    GLenum format, type;
    GLvoid * pixels;

    // tryb upakowania bajtów danych tekstury
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // wczytanie tekstury grass_color.tga
    GLboolean error = load_targa( filename, width, height, format, type, pixels );

    // b³¹d odczytu pliku
    if( error == GL_FALSE )
    {
        delete[]( unsigned char * ) pixels;
        return GL_FALSE;
    }

    // utworzenie tekstury wraz z mipmapami
    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, width, height, format, type, pixels );

    // ustawienie parametów œrodowiska tekstur
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    // filtr powiêkszaj¹cy
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // filtr pomniejszaj¹cy
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    // porz¹dki
    delete[]( unsigned char * ) pixels;

    // sukces
    return GL_TRUE;
}

// odczyt pliku graficznego w formacie OBJ
// filename - nazwa pliku
// object_list - numer listy wyœwietlania

GLboolean load_obj( const char * filename, GLuint & object_list )
{
    // otwarcie pliku do odczytu
    FILE * obj = fopen( filename, "rt" );

    // sprawdzenie poprawnoœci otwarcia pliku
    if( !obj )
         return GL_FALSE;

    // generowanie identyfikatora listy wyœwietlania
    object_list = glGenLists( 1 );

    // lista wyœwietlania z obiektem
    glNewList( object_list, GL_COMPILE );

    // odczyt danych pliku
    char str[ 512 ];
    while( !feof( obj ) )
    {
        // pobranie linii pliku
        fgets( str, 512, obj );

        // sprawdzenie co zawiera linia
        switch( str[ 0 ] )
        {
            // dane wierzcho³ków
        case 'v':
            load_vertex_data( str );
            break;

            // œciana wielok¹ta
        case 'f':
            load_face( str );
            break;

            // mtllib - plik z bibliotek¹ materia³ów
        case 'm':
            load_materials( str );
            break;

            // usemtl - u¿ycie wybranego materia³u
        case 'u':
            char name[ 128 ];
            sscanf( str, "%s", name );
            if( !strcmp( name, "usemtl" ) )
            {
                sscanf( str + 7, "%s", name );
                for( unsigned int i = 0; i < materials.size(); i++ )
                if( !strcmp( materials[ i ].name, name ) )
                {
                    // wy³¹czenie teksturowania dwuwymiarowego
                    glDisable( GL_TEXTURE_2D );

                    // wy³¹czenie mieszania kolorów
                    glDisable( GL_BLEND );

                    // w³aœciwoœci materia³u
                    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[ i ].Ka );
                    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, materials[ i ].Kd );
                    if( materials[ i ].illum == 2 )
                         glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, materials[ i ].Ks );

                    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[ i ].Ns );

                    // sprawdzenie przezroczystoœci
                    if( materials[ i ].d < 1.0 )
                    {
                        // w³¹czenie mieszania kolorów
                        glEnable( GL_BLEND );

                        // wspó³czynniki mieszania kolorów
                        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

                    }

                    // sprawdzenie czy jest tekstura
                    if( materials[ i ].map_Kd[ 0 ] )
                    {
                        // je¿eli tak, to w³¹czenie teksturowania dwuwymiarowego
                        if( load_texture( materials[ i ].map_Kd ) )
                             glEnable( GL_TEXTURE_2D );

                    }
                    break;
                }
            }
            break;

            // domyœlnie - pominiêcie linii pliku
            default:
            break;
        }
    }

    // koniec listy wyœwietlania z obiektem
    glEndList();

    // porz¹dki
    v.erase( v.begin(), v.end() );
    vt.erase( vt.begin(), vt.end() );
    vn.erase( vn.begin(), vn.end() );
    vp.erase( vp.begin(), vp.end() );
    materials.erase( materials.begin(), materials.end() );

    // zamkniêcie pliku
    fclose( obj );

    // sukces
    return GL_TRUE;
}
