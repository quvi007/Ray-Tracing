#include <bits/stdc++.h>
using namespace std;

#include "bitmap_image.hpp"

int main() {
    bitmap_image w_bmp(256, 256), b_bmp(256, 256);
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            w_bmp.set_pixel(j, i, 0, 255, 191);
            b_bmp.set_pixel(j, i, 255, 0, 64);
        }
    }
    w_bmp.save_image("texture_w.bmp");
    b_bmp.save_image("texture_b.bmp");
    return 0;
}