#include "graphics.hpp"
#include <cmath>

using namespace genv;

void draw_astrois(int cx, int cy, int radius, int steps, double angle_rad) {
    auto rotate_and_draw = [&](double x1, double y1, double x2, double y2) {
        float rx1 = x1 * cos(angle_rad) - y1 * sin(angle_rad);
        float ry1 = x1 * sin(angle_rad) + y1 * cos(angle_rad);
        float rx2 = x2 * cos(angle_rad) - y2 * sin(angle_rad);
        float ry2 = x2 * sin(angle_rad) + y2 * cos(angle_rad);
        gout << move_to(cx + rx1, cy + ry1) << line_to(cx + rx2, cy + ry2);
    };

    for (int i = 0; i <= steps; i++) {
        double d1 = (double)i / steps * radius;
        double d2 = radius - d1;

        if (i % 2 == 0) gout << color(255, 50, 50);
        else gout << color(80, 80, 255);

        rotate_and_draw(d1, 0, 0, d2);
        rotate_and_draw(-d1, 0, 0, d2);
        rotate_and_draw(-d1, 0, 0, -d2);
        rotate_and_draw(d1, 0, 0, -d2);
    }
}

int main() {
    const int W = 800;
    const int H = 600;
    const int CX = W / 2;
    const int CY = H / 2;
    gout.open(W, H);

    event ev;
    double current_angle = 0;
    int current_radius = 200;

    bool rotate_dragging = false;
    bool scale_dragging = false;

    double angle_offset = 0;
    int last_mouse_y = 0;

    auto redraw = [&]() {
        gout << move_to(0,0) << color(0,0,0) << box(W, H);
        draw_astrois(CX, CY, current_radius, 30, current_angle);
        gout << refresh;
    };

    redraw();

    while(gin >> ev && ev.keycode != key_escape) {
        if (ev.type == ev_mouse) {
            double mouse_angle = atan2(ev.pos_y - CY, ev.pos_x - CX);

            if (ev.button == btn_left) {
                rotate_dragging = true;
                angle_offset = mouse_angle - current_angle;
            }
            if (ev.button == -btn_left) {
                rotate_dragging = false;
            }

            if (ev.button == btn_right) {
                scale_dragging = true;
                last_mouse_y = ev.pos_y;
            }
            if (ev.button == -btn_right) {
                scale_dragging = false;
            }

            if (rotate_dragging) {
                current_angle = mouse_angle - angle_offset;
                redraw();
            }

            if (scale_dragging) {
                int diff_y = last_mouse_y - ev.pos_y;
                current_radius += diff_y;
                if (current_radius < 10) current_radius = 10;
                last_mouse_y = ev.pos_y;
                redraw();
            }

            if (ev.button == btn_wheelup) {
                current_radius += 10;
                redraw();
            }
            if (ev.button == btn_wheeldown) {
                if (current_radius > 20) current_radius -= 10;
                redraw();
            }
        }
    }
    return 0;
}