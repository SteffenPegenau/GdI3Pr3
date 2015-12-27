#include "Lucy.h"

#include <omp.h>
#include <cmath>

void Lucy::process(const Parameters &params, const Image &src, Image &dst) {
    //************************************************************************
    // Access image data
    //************************************************************************
    dst = src;

    // get dimensions
    const int height = src.height();
    const int width = src.width();

    // get n
    const int n = params.lucyN;
    
    
    Image x = src;
    Image b = src;
    for (int i = 1; i <= n; i++) {
        b = applyConstKernelOn(b);
    }
    dst = b;
    

    printf("lucyN (int): %i\n", params.lucyN);

}

Image Lucy::applyConstKernelOn(const Image& src) {
    // get dimensions
    const int height = src.height();
    const int width = src.width();
    // Result Image
    Image result = Image(height, width);
    

    // pseudo kernel Value
    const int k = 51;
    const int range = (51 - 1) / 2;
    
    // Farbwerte
    double red, green, blue;
    double new_red;
    double new_green;
    double new_blue;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            new_red = 0.0;
            new_green = 0.0;
            new_blue = 0.0;

            // Alle Werte im Kernel Bereich zu new-Werten addieren
            for (int r = x - range; r <= x + range; r++) {
                // Pixel liegt ausserhalb des Bildes
                if (r < 0 || r >= width) {
                    //printf("Pixel %i|%i liegt ausserhalb des Bildes (height=%i\twidht=%i)\n", y, x, height, width);
                    red = 255.0;
                    green = 255.0;
                    blue = 255.0;
                } else {
                    // Bezugspixel im Original
                    const Pixel &pixel = src[y][r];
                    red = pixel.r;
                    green = pixel.g;
                    blue = pixel.b;
                }
                new_red += red;
                new_green += green;
                new_blue += blue;
            }
            
            // Arith. Mittel bilden
            new_red = new_red / k;
            new_blue = new_blue / k;
            new_green = new_green / k;

            //const Pixel &p = src[y][x];
            //printf("Alte Werte:\tr=%f\tg=%f\tb=%f\n", p.r, p.g, p.b);
            //printf("Neue Werte:\tr=%f\tg=%f\tb=%f\n", new_red, new_green, new_blue);
            result[y][x] = Pixel(new_red, new_green, new_blue);
        }
    }
    return result;
}