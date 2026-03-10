#include <iostream>

#include "graphics.hpp"
#include "vector"
using namespace genv;
using namespace std;
const int szel = 800;
const int mag = 600;

struct Cetli {
// Mezok
private:
    int _x, _y, _size_x, _size_y;
    int _dx,_dy;


public:
    bool _kijelolve;
    //Konstuktor
    Cetli(int x, int y, int sx, int sy): //x, y koordinatak es a cetlik szelessege, magassaga
    _x(x),
    _y(y),
    _size_x(sx),
    _size_y(sy), //Mezők inicializalasa
    _kijelolve(0)
    {
        if (_x > szel - _size_x) {
            _x = szel - _size_x;
        }
        if (_y > mag - _size_y) {
            _y = mag - _size_y;
        }
    }


    void rajzol()
    {
        gout << move_to(_x, _y);
        gout << color(255, 0, 0);
        gout << box(_size_x, _size_y);
        gout << move_to(_x+5, _y+5);
        gout << color(0, 0, 255);
        gout << box(_size_x-10, _size_y-10);

        if (_kijelolve) {
            gout << move_to(_x, _y);
            gout << color(0, 0, 255);
            gout << box(_size_x, _size_y);
            gout << move_to(_x+5, _y+5);
            gout << color(255, 0, 0);
            gout << box(_size_x-10, _size_y-10);
        }}



    bool folotte_van_a_kurzor(int egerx, int egery) {
        return egerx >= _x and egerx <= _size_x + _x and egery >= _y and egery <= _y +_size_y;


    }

    void kijelol() {
        _kijelolve = true;
    }

    void visszavon() {
        _kijelolve = false;
    }

    void mozgatas(int egerx, int egery) {
        _x = egerx + _dx;
        _y = egery + _dy;

    }
    void megragad(int egerx, int egery) {
        _dx = _x - egerx;
        _dy = _y - egery;
    }



};



int main()
{
    vector<Cetli*> v;
    int n = 10;

    gout.open(szel,mag);
    gout << font("LiberationSans-Regular.ttf",20);

    for (int i = 0; i < n; i++) {
        v.push_back(new Cetli(rand()%szel,  rand()%mag, rand() % 30 + 50, rand() % 20 + 30));
    }

    for (Cetli *c:v)
        c->rajzol();




    gout << refresh;
    event ev;
    // ... a struct Cetli marad, de adjunk hozzá egy mutatót a főprogramban ...
    Cetli* aktualis_kijelolt = nullptr; // Tároljuk, kit mozgatunk éppen

    while(gin >> ev)
    {
        if (ev.type == ev_mouse) {

            // BAL KLIKK LENYOMÁSA
            if (ev.button == btn_left) {
                bool talalt = false;

                // Megnézzük, rákattintottunk-e valamelyikre (hátulról előre a Z-order miatt)
                for (int i = v.size() - 1; i >= 0; i--) {
                    if (v[i]->folotte_van_a_kurzor(ev.pos_x, ev.pos_y)) {

                        // Ha találtunk egyet, mindenki másról levesszük a kijelölést
                        for (Cetli* c : v) c->visszavon();

                        v[i]->kijelol();
                        v[i]->megragad(ev.pos_x, ev.pos_y);
                        aktualis_kijelolt = v[i];
                        talalt = true;
                        break; // Megvan a legfelső, megállunk
                    }
                }

                // Ha üres területre kattintottunk, minden kijelölést törlünk
                if (!talalt) {
                    for (Cetli* c : v) c->visszavon();
                    aktualis_kijelolt = nullptr;
                }
            }

            // MOZGATÁS (ha van lenyomott/kijelölt elem és mozog az egér)
            if (ev.button == 0 && aktualis_kijelolt != nullptr) {
                aktualis_kijelolt->mozgatas(ev.pos_x, ev.pos_y);
            }

            // EGÉRGOMB FELENGEDÉSE
            if (ev.button == -btn_left) {
                // Itt CSAK a mozgató mutatót nullázzuk,
                // de a Cetli belső _kijelolve változója true marad!
                aktualis_kijelolt = nullptr;
            }
        }

        // Képernyő frissítése
        gout << color(0, 0, 0) << move_to(0, 0)  << box(szel, mag);
        for (Cetli *c : v) {
            c->rajzol();
        }
        gout << refresh;
    }
    return 0;
}
