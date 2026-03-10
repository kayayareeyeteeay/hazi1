#include "graphics.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace genv;

const int XX = 800;
const int YY = 600;
const int NUM_PARTICLES = 3000;

struct Particle {
    float x, y;
    float vx, vy;
};

int main() {
    gout.open(XX, YY);
    srand(time(nullptr));

    std::vector<Particle> particles(NUM_PARTICLES);

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        particles[i].x = rand() % XX;
        particles[i].y = rand() % YY;
        particles[i].vx = 0;
        particles[i].vy = 0;
    }

    event ev;
    float mouse_x = XX / 2.0f;
    float mouse_y = YY / 2.0f;
    bool exploding = false;

    gin.timer(16);

    while (gin >> ev && ev.keycode != key_escape) {
        if (ev.type == ev_mouse) {
            mouse_x = ev.pos_x;
            mouse_y = ev.pos_y;

            if (ev.button == btn_left) {
                exploding = true;
            } else if (ev.button == -btn_left) {
                exploding = false;
            }
        }

        if (ev.type == ev_timer) {
            gout << move_to(0, 0) << color(0, 0, 0) << box(XX, YY);
            gout << color(200, 150, 255);

            for (auto& p : particles) {
                float dx = mouse_x - p.x;
                float dy = mouse_y - p.y;
                float dist_sq = dx * dx + dy * dy;
                float dist = std::sqrt(dist_sq);

                if (dist > 0.1f) {
                    float nx = dx / dist;
                    float ny = dy / dist;

                    if (exploding) {
                        if (dist < 250.0f) {
                            float force = 400.0f / (dist + 10.0f);
                            p.vx -= nx * force;
                            p.vy -= ny * force;
                        }
                    } else {
                        float force = 3000.0f / (dist_sq + 500.0f);
                        p.vx += nx * force;
                        p.vy += ny * force;

                        p.vx -= ny * (force * 0.3f);
                        p.vy += nx * (force * 0.3f);
                    }
                }

                p.vx *= 0.96f;
                p.vy *= 0.96f;
                p.x += p.vx;
                p.y += p.vy;

                gout << move_to(p.x, p.y) << dot;
            }

            gout << refresh;
        }
    }
    return 0;
}
