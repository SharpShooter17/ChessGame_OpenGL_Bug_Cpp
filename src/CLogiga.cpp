#include "CLogika.h"
#include <typeinfo>
#include <cstdlib>
#include <conio.h>
#include "windows.h"

CLogika::CLogika() : Zaznaczony_pionek_id( -1 ),
                     RuchMa( PIONEK_KOLOR_CZARNY ),
                     bPionekDotarlDoKonicaPlanszy( false ),
                     bZamianaGracza( false ),
                     STATE( NORMALNY_RUCH )
{
    int id = 0;
    for ( int i = 0; i < 8; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            Szachownica[i][j].id = id;
            Szachownica[i][j].bCzyPionekJest = false;
            Szachownica[i][j].Pionek.ktoTo = NONE;
            Szachownica[i][j].Pionek.KOLOR = NO_COLOR;
            Szachownica[i][j].Pionek.Ilosc_Ruchow = 0;
            ++id;
        }
    }

    ///Ustawienie pionkow na szachownicy
    ///Zwykle pionki
     for ( int i = 0; i < 8; ++i )
     {
         Szachownica[1][i].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
         Szachownica[1][i].Pionek.ktoTo = PIONEK;
         Szachownica[1][i].bCzyPionekJest = true;

         Szachownica[6][i].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
         Szachownica[6][i].Pionek.ktoTo = PIONEK;
         Szachownica[6][i].bCzyPionekJest = true;
     }
    ///Wieze
    Szachownica[0][0].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][0].Pionek.ktoTo = WIEZA;
    Szachownica[0][0].bCzyPionekJest = true;

    Szachownica[0][7].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][7].Pionek.ktoTo = WIEZA;
    Szachownica[0][7].bCzyPionekJest = true;

    Szachownica[7][0].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][0].Pionek.ktoTo = WIEZA;
    Szachownica[7][0].bCzyPionekJest = true;

    Szachownica[7][7].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][7].Pionek.ktoTo = WIEZA;
    Szachownica[7][7].bCzyPionekJest = true;
    ///Konik
    Szachownica[0][1].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][1].Pionek.ktoTo = KONIK;
    Szachownica[0][1].bCzyPionekJest = true;

    Szachownica[0][6].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][6].Pionek.ktoTo = KONIK;
    Szachownica[0][6].bCzyPionekJest = true;

    Szachownica[7][1].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][1].Pionek.ktoTo = KONIK;
    Szachownica[7][1].bCzyPionekJest = true;

    Szachownica[7][6].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][6].Pionek.ktoTo = KONIK;
    Szachownica[7][6].bCzyPionekJest = true;
    ///SKOCZEK
    Szachownica[0][2].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][2].Pionek.ktoTo = SKOCZEK;
    Szachownica[0][2].bCzyPionekJest = true;

    Szachownica[0][5].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][5].Pionek.ktoTo = SKOCZEK;
    Szachownica[0][5].bCzyPionekJest = true;

    Szachownica[7][2].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][2].Pionek.ktoTo = SKOCZEK;
    Szachownica[7][2].bCzyPionekJest = true;

    Szachownica[7][5].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][5].Pionek.ktoTo = SKOCZEK;
    Szachownica[7][5].bCzyPionekJest = true;
    ///KROL
    Szachownica[0][4].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][4].Pionek.ktoTo = KROL;
    Szachownica[0][4].bCzyPionekJest = true;

    Szachownica[7][4].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][4].Pionek.ktoTo = KROL;
    Szachownica[7][4].bCzyPionekJest = true;
    ///DAMA
    Szachownica[0][3].Pionek.KOLOR = PIONEK_KOLOR_CZARNY;
    Szachownica[0][3].Pionek.ktoTo = DAMA;
    Szachownica[0][3].bCzyPionekJest = true;

    Szachownica[7][3].Pionek.KOLOR = PIONEK_KOLOR_BIALY;
    Szachownica[7][3].Pionek.ktoTo = DAMA;
    Szachownica[7][3].bCzyPionekJest = true;
}

CLogika::~CLogika() {}

void CLogika::Logika_gry()
{
    if ( bZamianaGracza )
    {
        Zamiana_gracza_aktywnego();             ///dokonuje wszystkich sprawdzen stanu gry!
        bZamianaGracza = false;
    }
}

void CLogika::KontrolaRuchu( int id )
{
    if ( !bPionekDotarlDoKonicaPlanszy )
    {
        Wybor_pionka_Na_planszy( id );
    }
    else if ( bPionekDotarlDoKonicaPlanszy )
    {
        Wybor_pionka_zbitego( id );
    }
}
void CLogika::Wybor_pionka_Na_planszy( int id )
{
    const int x = id / 8, y = id % 8;

    if ( Zaznaczony_pionek_id == -1 &&
            Szachownica[x][y].bCzyPionekJest &&
            id >= 0 &&
            Szachownica[x][y].Pionek.KOLOR == RuchMa )
    {
        Zaznaczony_pionek_id = id;
        OkreslRuchDlaPionka( Zaznaczony_pionek_id, RuchMa, DozwolonyRuchNaPoleId );
    }
    else if ( id == Zaznaczony_pionek_id )
    {
        Zaznaczony_pionek_id = -1;
        DozwolonyRuchNaPoleId.clear();
    }
    else if ( Zaznaczony_pionek_id != -1 &&
              id != Zaznaczony_pionek_id &&
              Szachownica[x][y].bCzyPionekJest && Szachownica[x][y].Pionek.KOLOR == RuchMa )
    {
        DozwolonyRuchNaPoleId.clear();
        Zaznaczony_pionek_id = id;
        OkreslRuchDlaPionka( Zaznaczony_pionek_id, RuchMa, DozwolonyRuchNaPoleId );
    }
    else if ( id != Zaznaczony_pionek_id &&
              Zaznaczony_pionek_id != -1 )
    {
        ///Odpowiada za usuwanie wroga :D
        if ( Szachownica[x][y].bCzyPionekJest && Szachownica[x][y].Pionek.KOLOR == RuchMa )
            return;

        bool czyRuchMozliwy = false;
        for ( int i = 0; i < DozwolonyRuchNaPoleId.size(); ++i )
        {
            if ( id == DozwolonyRuchNaPoleId[i] )
            {
                czyRuchMozliwy = true;
                break;
            }
            else
            {
                continue;
            }
        }

        if ( czyRuchMozliwy )
        {
            PrzesunPionek( id );
            OkreslRuchDlaPionka( -1, RuchMa, DozwolonyRuchNaPoleId );
        }
    }

    if ( Zaznaczony_pionek_id != -1 )
        CzyRuchDozwolonyNaDanePolePodczasSzachu( getIdKrola(RuchMa), Zaznaczony_pionek_id, DozwolonyRuchNaPoleId );
}

void CLogika::Wybor_pionka_zbitego( int id )
{
    sPionek Pionek;

    switch ( id )
    {
    case WIEZA:
        Pionek.KOLOR = RuchMa;
        Pionek.ktoTo = WIEZA;
        break;
    case SKOCZEK:
        Pionek.KOLOR = RuchMa;
        Pionek.ktoTo = SKOCZEK;
        break;
    case KONIK:
        Pionek.KOLOR = RuchMa;
        Pionek.ktoTo = KONIK;
        break;
    case DAMA:
        Pionek.KOLOR = RuchMa;
        Pionek.ktoTo = DAMA;
        break;
    default:
        return;
        break;
    }

    Szachownica[ Zaznaczony_pionek_id/8 ][ Zaznaczony_pionek_id%8 ].Pionek = Pionek;

    bPionekDotarlDoKonicaPlanszy = false;
    bZamianaGracza = true;
}

void CLogika::PrzesunPionek( int id )
{
    int xp = Zaznaczony_pionek_id / 8,
        yp = Zaznaczony_pionek_id % 8,
        xk = id / 8,
        yk = id % 8;

    ///Czy zbijam pionka?
    if ( Szachownica[xk][yk].bCzyPionekJest && Szachownica[xk][yk].Pionek.ktoTo != PIONEK )
    {
        Zbite_pionki.push_back( Szachownica[xk][yk].Pionek );
    }

    Szachownica[xk][yk].bCzyPionekJest = true;
    Szachownica[xk][yk].Pionek = Szachownica[xp][yp].Pionek;
    Szachownica[xk][yk].Pionek.Ilosc_Ruchow++;

    Zaznaczony_pionek_id = Szachownica[xk][yk].id;

    Szachownica[xp][yp].bCzyPionekJest = false;
    Szachownica[xp][yp].Pionek.ktoTo = NONE;
    Szachownica[xp][yp].Pionek.KOLOR = NO_COLOR;

    if ( Szachownica[xk][yk].Pionek.ktoTo == PIONEK && ( xk == 7 || xk == 0 ) && Zbite_pionki.size() > 0 )
    {
        bPionekDotarlDoKonicaPlanszy = true;
    }
    else
    {
        bZamianaGracza = true;
    }
}

void CLogika::Zamiana_gracza_aktywnego()
{
    Zaznaczony_pionek_id = -1;
    RuchMa = RuchMa == PIONEK_KOLOR_CZARNY ? PIONEK_KOLOR_BIALY : PIONEK_KOLOR_CZARNY;

    if ( SprawdzCzySzach( RuchMa == PIONEK_KOLOR_CZARNY ? getIdKrola( PIONEK_KOLOR_CZARNY ) : getIdKrola( PIONEK_KOLOR_BIALY ), RuchMa ) )
    {
        STATE = SZACH;
    }
    else
    {
        STATE = NORMALNY_RUCH;
    }

    if ( STATE == SZACH )
    {
        if ( SprawdzCzyMat( RuchMa ) )
            STATE = MAT;
    }

    if ( SprawdzCzyPat() ) ///Jesli PAT to koniec gry!
    {
        STATE = PAT;
    }

    switch ( STATE )
    {
    case NORMALNY_RUCH:
        break;
    case SZACH:
        break;
    case MAT:



        break;
    case PAT:
        break;
    case QUIT:
        exit( 1 );
        break;
    }
}

void CLogika::OkreslRuchDlaPionka( int id, PIONEK_KOLOR kolor, std::vector<int> & c, bool SprawdzajCzySzachujeKrola )
{
    if ( id < 0 )
    {
        c.clear();
        return;
    }

    int x = id / 8,
        y = id % 8;

    switch ( Szachownica[x][y].Pionek.ktoTo )
    {
    case NONE:
        c.clear();
        break;
    case PIONEK:
        RuchyPionka( id, c );
        break;
    case WIEZA:
        RuchWiezy( id, kolor, c );
        break;
    case KONIK:
        RuchKonik( id, kolor, c );
        break;
    case SKOCZEK:
        RuchSkoczek( id, kolor, c );
        break;
    case KROL:
        RuchKrol( id, kolor, c );
        if ( SprawdzajCzySzachujeKrola )
            CzySzachNaKrol( id, kolor, c );
        break;
    case DAMA:
        RuchDama( id, kolor, c );
        break;
    default:
        break;
    }
}

bool CLogika::SprawdzCzySzach( int id_krola, PIONEK_KOLOR kolor )
{
    for ( int i = 0; i < 8; ++i )
    {
        for ( int j = 0; j < 8; ++j)
        {
            if ( Szachownica[i][j].Pionek.KOLOR != kolor &&
                    Szachownica[i][j].bCzyPionekJest &&
                    Szachownica[i][j].Pionek.ktoTo != PIONEK )
            {
                std::vector<int> Ruchy;
                OkreslRuchDlaPionka( Szachownica[i][j].id, Szachownica[i][j].Pionek.KOLOR, Ruchy, false );
                for ( int k = 0; k < Ruchy.size(); ++k )
                {
                    if ( Ruchy[k] == id_krola )
                    {
                        return true;
                    }
                }
            }
            else if ( Szachownica[i][j].Pionek.KOLOR != kolor &&
                      Szachownica[i][j].bCzyPionekJest &&
                      Szachownica[i][j].Pionek.ktoTo == PIONEK )
            {
                std::vector<int> Ruchy;
                BiciePionka( Szachownica[i][j].id, Ruchy );
                for ( int k = 0; k < Ruchy.size(); ++k )
                {
                    if ( Ruchy[k] == id_krola )
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool CLogika::SprawdzCzyPat()
{
    int countBlack = 0, id_Bkrol = getIdKrola( PIONEK_KOLOR_CZARNY );
    int countWhite = 0, id_Wkrol = getIdKrola( PIONEK_KOLOR_BIALY );

    for ( int i = 0; i < 8; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            if ( Szachownica[i][j].bCzyPionekJest )
            {
                if (  Szachownica[i][j].Pionek.KOLOR == PIONEK_KOLOR_BIALY )
                {
                    countWhite++;
                    continue;
                }
                else if (  Szachownica[i][j].Pionek.KOLOR == PIONEK_KOLOR_CZARNY )
                {
                    countBlack++;
                    continue;
                }
            }
        }
    }

    if ( countBlack > 1 && countWhite > 1 )
        return false;

    ///Jezeli zostal tylko jeden pionek danego kolorou rozpatrujemy pat!

    std::vector<int> RuchyKrola;

    if ( countBlack == 1 )
    {
        RuchKrol( id_Bkrol, PIONEK_KOLOR_CZARNY, RuchyKrola );
        int MozliweRuchyKrola = CzySzachNaKrol( id_Bkrol, PIONEK_KOLOR_CZARNY, RuchyKrola );

        if ( MozliweRuchyKrola == 0 )
        {
            return true;
        }

        RuchyKrola.clear();
    }

    if ( countWhite == 1 )
    {
        RuchKrol( id_Wkrol, PIONEK_KOLOR_BIALY, RuchyKrola );
        int MozliweRuchyKrola = CzySzachNaKrol( id_Wkrol, PIONEK_KOLOR_BIALY, RuchyKrola );

        if ( MozliweRuchyKrola == 0 )
        {
            return true;
        }
    }

    return false; ///Jakim cudem program tutaj doszedl? :O
}

bool CLogika::SprawdzCzyMat( PIONEK_KOLOR kolor )
{
    int id_Bkrol = getIdKrola( PIONEK_KOLOR_CZARNY );
    int id_Wkrol = getIdKrola( PIONEK_KOLOR_BIALY );

    ///Jezeli krola szachuje przeciwnik
    if ( SprawdzCzySzach( kolor == PIONEK_KOLOR_CZARNY ? id_Bkrol : id_Wkrol , kolor ) )
    {

        PionkiKtorymiMoznaSieRuszacPodczasSzachu = SprawdzajPionkiUnikanieMata( kolor );

        if ( PionkiKtorymiMoznaSieRuszacPodczasSzachu.size() != 0 )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
}

std::vector<int> CLogika::SprawdzajPionkiUnikanieMata( PIONEK_KOLOR kolor )
{
    PolaNaKtorePionekMozeStanacPodczasSzachu.clear();

    std::vector<int> id;
    for ( int i = 0; i < 8; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            if ( Szachownica[i][j].bCzyPionekJest &&
                    Szachownica[i][j].Pionek.KOLOR == kolor )
            {
                std::vector<int> c;
                OkreslRuchDlaPionka( Szachownica[i][j].id, kolor, c );

                Pola Back = Szachownica[i][j];
                Szachownica[i][j].bCzyPionekJest = false;
                Szachownica[i][j].Pionek.KOLOR = NO_COLOR;
                Szachownica[i][j].Pionek.ktoTo = NONE;

                for ( int k = 0; k < c.size(); ++k )
                {
                    Pola PoleBack = Szachownica[ c[k]/8 ][ c[k]%8 ];
                    Szachownica[ c[k]/8 ][ c[k]%8 ].bCzyPionekJest = true;
                    Szachownica[ c[k]/8 ][ c[k]%8 ].Pionek = Back.Pionek;

                    if ( !SprawdzCzySzach( getIdKrola( kolor ), kolor ) )
                    {
                        if ( Back.Pionek.ktoTo != KROL )
                        {
                            PolaNaKtorePionekMozeStanacPodczasSzachu.push_back( PoleBack.id );
                        }
                        else
                        {
                            PolaNaKtoreMozeUciekacKrolPodczasSzachu.push_back( PoleBack.id );
                        }

                        Szachownica[ c[k]/8 ][ c[k]%8 ] = PoleBack;
                        Szachownica[i][j] = Back;
                        id.push_back( Szachownica[i][j].id );
                    }

                    Szachownica[ c[k]/8 ][ c[k]%8 ] = PoleBack;
                }

                Szachownica[i][j] = Back;
            }
        }
    }

    return id;
}

int CLogika::CzySzachNaKrol( int id, PIONEK_KOLOR kolor, std::vector<int> & c )
{
    int x = id / 8,
        y = id % 8;

    Pola backKrol = Szachownica[x][y];

    Szachownica[x][y].bCzyPionekJest = false;
    Szachownica[x][y].Pionek.KOLOR = NO_COLOR;
    Szachownica[x][y].Pionek.ktoTo = NONE;

    for ( int i = 0; i < c.size(); ++i )
    {
        Pola backPoleSprawdzane = Szachownica[c[i]/8][c[i]%8];

        Szachownica[c[i]/8][c[i]%8].bCzyPionekJest = true;
        Szachownica[c[i]/8][c[i]%8].Pionek = backKrol.Pionek;

        if ( SprawdzCzySzach( c[i], kolor ) )
        {
            Szachownica[c[i]/8][c[i]%8] = backPoleSprawdzane;
            c.erase( c.begin() + i );
            --i;
        }
        else
        {
            Szachownica[c[i]/8][c[i]%8] = backPoleSprawdzane;
        }
    }

    Szachownica[x][y] = backKrol;

    return c.size();
}

void CLogika::CzyRuchDozwolonyNaDanePolePodczasSzachu( int id_krola_szachowanego,
        int id_pionka_do_sprawdzenia,
        std::vector<int> &Dozwolone_ruchy_pionka )
{

    if ( id_krola_szachowanego == id_pionka_do_sprawdzenia )
        return;

    Pola Back_id_pionka = Szachownica[id_pionka_do_sprawdzenia/8][id_pionka_do_sprawdzenia%8];
    Szachownica[id_pionka_do_sprawdzenia/8][id_pionka_do_sprawdzenia%8].bCzyPionekJest = false;
    Szachownica[id_pionka_do_sprawdzenia/8][id_pionka_do_sprawdzenia%8].Pionek.KOLOR = NO_COLOR;
    Szachownica[id_pionka_do_sprawdzenia/8][id_pionka_do_sprawdzenia%8].Pionek.ktoTo = NONE;

    for ( int i = 0; i < Dozwolone_ruchy_pionka.size(); ++i )
    {
        bool bUsunac = false;

        Pola Back = Szachownica[ Dozwolone_ruchy_pionka[i]/8 ][ Dozwolone_ruchy_pionka[i]%8 ];

        Szachownica[ Dozwolone_ruchy_pionka[i]/8 ][ Dozwolone_ruchy_pionka[i]%8 ].bCzyPionekJest = true;
        Szachownica[ Dozwolone_ruchy_pionka[i]/8 ][ Dozwolone_ruchy_pionka[i]%8 ].Pionek = Back_id_pionka.Pionek;

        if ( SprawdzCzySzach(id_krola_szachowanego, Szachownica[id_krola_szachowanego/8][id_krola_szachowanego%8].Pionek.KOLOR ) )
        {
            bUsunac = true;
        }

        Szachownica[ Dozwolone_ruchy_pionka[i]/8 ][ Dozwolone_ruchy_pionka[i]%8 ] = Back;

        if ( bUsunac )
        {
            Dozwolone_ruchy_pionka.erase( Dozwolone_ruchy_pionka.begin() + i );
            --i;
        }
    }

    Szachownica[id_pionka_do_sprawdzenia/8][id_pionka_do_sprawdzenia%8] = Back_id_pionka;
}

void CLogika::RuchDama( int id, PIONEK_KOLOR kolor, std::vector<int> & c )
{
    RuchWiezy( id, kolor, c );
    RuchSkoczek( id, kolor, c );
}

void CLogika::RuchKrol( int id, PIONEK_KOLOR kolor, std::vector<int> & c )
{
    int x = id / 8,
        y = id % 8;

    if ( y < 7 && Szachownica[ x ][ y + 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x ][ y + 1 ].id );

    if ( y > 0 && Szachownica[ x ][ y - 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x ][ y - 1 ].id );

    if ( x > 0 && Szachownica[ x - 1 ][ y ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x - 1 ][ y ].id );

    if ( x < 7 && Szachownica[ x + 1 ][ y ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x + 1 ][ y ].id );

    if ( x < 7 && y < 7 && Szachownica[ x + 1 ][ y + 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x + 1 ][ y + 1 ].id );

    if ( x < 7 && y > 0 && Szachownica[ x + 1 ][ y - 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x + 1 ][ y - 1 ].id );

    if ( x > 0 && y < 7 && Szachownica[ x - 1 ][ y + 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x - 1 ][ y + 1 ].id );

    if ( x > 0 && y > 0 && Szachownica[ x - 1 ][ y - 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x - 1 ][ y - 1 ].id );
}

void CLogika::RuchKonik( int id, PIONEK_KOLOR kolor, std::vector<int> & c )
{
    int x = id / 8,
        y = id % 8;

    if ( x - 2 >= 0 && y + 1 <= 7 && Szachownica[ x - 2 ][ y + 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x - 2 ][ y + 1 ].id );

    if ( x - 2 >= 0 && y - 1 >= 0 && Szachownica[ x - 2 ][ y - 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x - 2 ][ y - 1 ].id );

    if ( x + 2 <= 7 && y + 1 <= 7 && Szachownica[ x + 2 ][ y + 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x + 2 ][ y + 1 ].id );

    if ( x + 2 <= 7 && y - 1 >= 0 && Szachownica[ x + 2 ][ y - 1 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x + 2 ][ y - 1 ].id );

    if ( x - 1 >= 0 && y + 2 <= 7 && Szachownica[ x - 1 ][ y + 2 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x - 1 ][ y + 2 ].id );

    if ( x - 1 >= 0 && y - 2 >= 0 && Szachownica[ x - 1 ][ y - 2 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x - 1 ][ y - 2 ].id );

    if ( x + 1 <= 7 && y + 2 <= 7 && Szachownica[ x + 1 ][ y + 2 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x + 1 ][ y + 2 ].id );

    if ( x + 1 <= 7 && y - 2 >= 0 && Szachownica[ x + 1 ][ y - 2 ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x + 1 ][ y - 2 ].id );
}

void CLogika::RuchSkoczek( int id, PIONEK_KOLOR kolor, std::vector<int> & c )
{
    int x = id / 8,
        y = id % 8;

    int ya = y + 1;
    int xa = x - 1;

    while ( ya <= 7 && xa >= 0 &&
            !Szachownica[ xa ][ ya ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ xa ][ ya ].id );
        ++ya;
        --xa;
    }
    if ( ya <= 7 && xa >= 0 && Szachownica[xa][ya].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ xa ][ ya ].id );

    ya = y + 1;
    xa = x + 1;
    while ( ya <= 7 && xa <= 7 &&
            !Szachownica[ xa ][ ya ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ xa ][ ya ].id );
        ++ya;
        ++xa;
    }
    if ( ya <= 7 && xa <= 7 && Szachownica[xa][ya].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ xa ][ ya ].id );

    ya = y - 1;
    xa = x + 1;
    while ( ya >= 0 && xa <= 7 &&
            !Szachownica[ xa ][ ya ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ xa ][ ya ].id );
        --ya;
        ++xa;
    }
    if ( ya >= 0 && xa <= 7 && Szachownica[xa][ya].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ xa ][ ya ].id );

    ya = y - 1;
    xa = x - 1;
    while ( ya >= 0 && xa >= 0 &&
            !Szachownica[ xa ][ ya ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ xa ][ ya ].id );
        --ya;
        --xa;
    }
    if ( ya >= 0 && xa >= 0 && Szachownica[xa][ya].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ xa ][ ya ].id );

}

void CLogika::RuchWiezy( int id, PIONEK_KOLOR kolor, std::vector<int> & c )
{
    int x = id / 8,
        y = id % 8;

    int ya = y + 1;
    while ( ya <= 7 &&
            !Szachownica[ x ][ ya ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ x ][ ya ].id );
        ++ya;
    }
    if ( ya <= 7 && Szachownica[x][ya].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x ][ ya ].id );

    ya = y - 1;
    while ( ya >= 0 &&
            !Szachownica[ x ][ ya ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ x ][ ya ].id );
        --ya;
    }
    if ( ya >= 0 && Szachownica[x][ya].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ x ][ ya ].id );

    int xa = x + 1;
    while ( xa <= 7 &&
            !Szachownica[ xa ][ y ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ xa ][ y ].id );
        ++xa;
    }
    if ( xa <= 7 && Szachownica[ xa ][ y ].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ xa ][ y ].id );

    xa = x - 1;
    while ( xa >= 0 &&
            !Szachownica[ xa ][ y ].bCzyPionekJest )
    {
        c.push_back( Szachownica[ xa ][ y ].id );
        --xa;
    }
    if ( xa >= 0 && Szachownica[xa][y].Pionek.KOLOR != kolor )
        c.push_back( Szachownica[ xa ][ y ].id );
}

void CLogika::BiciePionka( int id, std::vector<int> & c )
{
    int x = id / 8,
        y = id % 8;

    ///Mozliwosc bicia wroga po skosie

    ///Dla czarnych pionkow

    if ( Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_CZARNY &&              ///Jezeli czarny pionek
            x < 7 && y < 7 &&
            Szachownica[ x + 1 ][ y + 1 ].bCzyPionekJest &&
            Szachownica[ x + 1 ][ y + 1 ].Pionek.KOLOR == PIONEK_KOLOR_BIALY )
    {
        c.push_back( Szachownica[ x + 1 ][ y + 1 ].id );
    }

    if ( Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_CZARNY &&              ///Jezeli czarny pionek
            x < 7 && y > 0 &&
            Szachownica[ x + 1 ][ y - 1 ].bCzyPionekJest &&
            Szachownica[ x + 1 ][ y - 1 ].Pionek.KOLOR == PIONEK_KOLOR_BIALY )
    {
        c.push_back( Szachownica[ x + 1 ][ y - 1 ].id );
    }

    ///Dla bialych

    if ( Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_BIALY &&               ///Jezeli bialy pionek
            x > 0 && y < 7 &&
            Szachownica[ x - 1 ][ y + 1 ].bCzyPionekJest &&
            Szachownica[ x - 1 ][ y + 1 ].Pionek.KOLOR == PIONEK_KOLOR_CZARNY )
    {
        c.push_back( Szachownica[ x - 1 ][ y + 1 ].id );
    }

    if ( Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_BIALY &&               ///Jezeli bialy pionek
            x > 0 && y > 0 &&
            Szachownica[ x - 1 ][ y - 1 ].bCzyPionekJest &&
            Szachownica[ x - 1 ][ y - 1 ].Pionek.KOLOR == PIONEK_KOLOR_CZARNY )
    {
        c.push_back( Szachownica[ x - 1 ][ y - 1 ].id );
    }
    return;
}


void CLogika::RuchyPionka( int id, std::vector<int> & c)
{
    int x = id / 8,
        y = id % 8;

    ///Jezeli pionek wykonal ruchow 0 - mozliwosc "podwojnego skoku"
    if ( Szachownica[x][y].Pionek.Ilosc_Ruchow == 0 )
    {
        if ( Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_CZARNY &&       ///Jezeli czrny pionek
                !Szachownica[ (id + 8) /8 ][(id + 8) % 8].bCzyPionekJest && ///i nie ma innego pionka pole przed nim
                !Szachownica[ (id + 16) /8 ][(id + 16) % 8].bCzyPionekJest )///i nie ma innego pionka 2 pola przed nim
        {
            c.push_back( id + 8 + 8 );
        }
        else if (Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_BIALY &&     ///Jezeli bialy pionek
                 !Szachownica[ (id - 8) /8 ][(id - 8) % 8].bCzyPionekJest&&  ///i nie ma innego pionka pole przed nim
                 !Szachownica[ (id - 16) /8 ][(id - 16) % 8].bCzyPionekJest )///i nie ma innego pionka 2 pola przed nim
        {
            c.push_back( id - 8 - 8 );
        }
    }

    ///Pionki sa na koncu planszy!
    if ( x == 0 || x == 7 )
        return;

    if ( Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_CZARNY &&       ///Jezeli czrny pionek
            !Szachownica[ (id + 8) /8 ][(id + 8) % 8].bCzyPionekJest)   ///i nie ma innego pionka pole przed nim
        c.push_back( id + 8 );
    else if (Szachownica[x][y].Pionek.KOLOR == PIONEK_KOLOR_BIALY &&    ///Jezeli bialy pionek
             !Szachownica[ (id - 8) /8 ][(id - 8) % 8].bCzyPionekJest ) ///i nie ma innego pionka pole przed nim
        c.push_back( id - 8 );

    BiciePionka( id, c );

    return;
}

int CLogika::getIdKrola( PIONEK_KOLOR kolor )
{
    for ( int i = 0; i < 8; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            if ( Szachownica[i][j].bCzyPionekJest &&
                    Szachownica[i][j].Pionek.ktoTo == KROL &&
                    Szachownica[i][j].Pionek.KOLOR == kolor )
            {
                return Szachownica[i][j].id;
            }
        }
    }

    return -1;
}
