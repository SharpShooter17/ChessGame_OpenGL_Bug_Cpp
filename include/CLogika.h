#ifndef CLOGIGA_H
#define CLOGIGA_H

#include <vector>

///ten enum nie moze zmienic wartosci
///poniewaz jest wykorzystywany w if'ie
///czarny musi byc 0 a bialy rozny od 0!

enum PIONEK_KOLOR
{
    NO_COLOR,
    PIONEK_KOLOR_CZARNY,
    PIONEK_KOLOR_BIALY,
};

enum RODZAJ_PIONKA { NONE,
                     PIONEK,
                     WIEZA,
                     KONIK,
                     SKOCZEK,
                     KROL,
                     DAMA
                   };

struct sPionek
{
    PIONEK_KOLOR KOLOR;
    RODZAJ_PIONKA ktoTo;
    int Ilosc_Ruchow;
};

struct Pola
{
    short id;
    sPionek Pionek;
    bool bCzyPionekJest;
};

enum status {
                NORMALNY_RUCH,
                SZACH,
                PAT,
                MAT,
                QUIT
            };

class CLogika
{
public:
    CLogika();
    ~CLogika();

    void Logika_gry();

    Pola Szachownica[8][8];
    std::vector <int> DozwolonyRuchNaPoleId;
    void KontrolaRuchu( int id );
    void ZaminaPionka( int id );

    std::vector <sPionek> Zbite_pionki;

    bool bPionekDotarlDoKonicaPlanszy; /// Wymiana pionka na figure!

    PIONEK_KOLOR RuchMa;

    status STATE;

protected:
private:

    void PrzesunPionek( int id );
    void OkreslRuchDlaPionka( int id, PIONEK_KOLOR kolor, std::vector<int> & c, bool SprawdzajCzySzachujeKrola = true );
    void CzyRuchDozwolonyNaDanePolePodczasSzachu( int id_krola_szachowanego, int id_pionka_do_sprawdzenia, std::vector<int> &Dozwolone_ruchy_pionka );

    int Zaznaczony_pionek_id;

    ///zasady gry
    void Zamiana_gracza_aktywnego();

    bool SprawdzCzySzach( int id_krola, PIONEK_KOLOR kolor );
    bool SprawdzCzyMat( PIONEK_KOLOR kolor );
    std::vector< int > PionkiKtorymiMoznaSieRuszacPodczasSzachu;
    std::vector< int > PolaNaKtorePionekMozeStanacPodczasSzachu;
    std::vector< int > PolaNaKtoreMozeUciekacKrolPodczasSzachu;
    std::vector<int> SprawdzajPionkiUnikanieMata( PIONEK_KOLOR kolor ); ///Zwraca pusta tablice jezeli zaden pionek nie zasloni krola lub tablice z id pionkow ktore zaslonia krola
    bool SprawdzCzyPat();

    bool bZamianaGracza;

    ///Okreslanie ruchow
    void RuchyPionka( int id, std::vector<int> & c );
    void BiciePionka( int id, std::vector<int> & c );
    void RuchWiezy( int id, PIONEK_KOLOR kolor, std::vector<int> & c );
    void RuchKrol( int id, PIONEK_KOLOR kolor, std::vector<int> & c );
    int CzySzachNaKrol( int id, PIONEK_KOLOR kolor, std::vector<int> & c ); ///Zwraca dozwolona liczbe ruchow krola!
    void RuchSkoczek( int id, PIONEK_KOLOR kolor, std::vector<int> & c );
    void RuchDama( int id, PIONEK_KOLOR kolor, std::vector<int> & c );
    void RuchKonik( int id, PIONEK_KOLOR kolor, std::vector<int> & c );

    int getIdKrola( PIONEK_KOLOR kolor );

    ///Kontrola ruchow
    void Wybor_pionka_Na_planszy( int id );
    void Wybor_pionka_zbitego( int id );
};

#endif // CLOGIGA_H
