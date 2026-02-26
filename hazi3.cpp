#include "graphics.hpp"
#include "vector"
#include "fstream"

using namespace genv;
using namespace std ;

const int width = 800;
const int height = 600;

struct Ho{
    Ho(int _x, int _y): x(_x), y(_y)
    {};
    void rajzol () {


        gout<<color(255,255,255)<<move_to(x,y)<<dot;
    }

    bool leert() {
        if (y >= height) {
            return true;
        }
        return false;

    }
    void mozog(int dx, int dy){
        x = (x + dx + width) % width;
        y = (y + dy) + dy;


        if(x>width || y>height||x<0)
        {

            x=rand() % width;
            y=0;
        };
    }
private:
    int x,y;


};

int main()
{

    int szamlalo = 0;
    int szel = 0;



    int n = 20;

    vector<Ho*> ho;



    gout.open(width,height);
    event ev;





    gin.timer(50);

    gout<<color(255,255,255);
    for (int i = 0; i < n; ++i) {
        ho.push_back(new Ho(rand() % width, rand() % height));

    };
    for (int i = 0; i < ho.size(); ++i) {
        ho[i]->rajzol();
    };


    while(gin >> ev) {

        if(ev.type == ev_mouse){
            szel = (ev.pos_x - width/2) / 20;
        }
        else if(ev.type == ev_timer){

            // gout<<refresh;
            // gout << move_to(40,15) << text(std::to_string(szamlalo));
            // gout<<refresh;

            gout<<color(0,0,0)<<move_to(0,0)<<box(width,height);


            for (int i = 0; i < n; ++i) {
                ho[i]->mozog(rand() % 9 -4+szel, rand() % 5 + 3) ;
                ho[i]->rajzol();

                if (ho[i]->leert()) {
                    szamlalo++;
                }



            };






        }





    }
    return 0;
}
