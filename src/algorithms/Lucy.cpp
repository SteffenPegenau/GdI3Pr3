#include "Lucy.h"
#include <QTime>
#include <omp.h>
#include <cmath>

void Lucy::process(const Parameters &params, const Image &src, Image &dst) {
    
    QTime timer;
    timer.start();
    //************************************************************************
    // Access image data
    //************************************************************************
    dst = src;

    // get dimensions
    const int height = src.height();
    const int width = src.width();

    // get n
    const int n = params.lucyN;

    Image u, b, d;
    for (int i = 1; i <= n; i++) {
        printf("(%i / %i)\n", i, n);
        b = applyConstKernelOn(dst);
        u = division(src, b);
        d = applyConstKernelOn(u);
        dst = multiplication(dst, d);
    }
    int laufzeitMS = timer.elapsed();
    double laufzeitPerIt = laufzeitMS / (double) params.lucyN;

    printf("Iterationen: %i\n", params.lucyN);
    printf("Dauer: %i ms\n", laufzeitMS);
    printf("=> ~%f ms/Iteration\n", laufzeitPerIt);
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

Image Lucy::division(const Image& dividend, const Image& divisor) {
    // get dimensions
    const int height = dividend.height();
    const int width = dividend.width();
    // Result Image
    double r, g, b;

    Image result = Image(height, width);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel &f1 = dividend[y][x];
            const Pixel &f2 = divisor[y][x];
            r = (f1.r / f2.r > 255) ? 255 : f1.r / f2.r;
            g = (f1.g / f2.g > 255) ? 255 : f1.g / f2.g;
            b = (f1.b / f2.b > 255) ? 255 : f1.b / f2.b;
            result[y][x] = Pixel(r, g, b);
        }
    }
    return result;
}

Image Lucy::multiplication(const Image& factor1, const Image& factor2) {
    // get dimensions
    const int height = factor1.height();
    const int width = factor1.width();
    // Result Image
    double r, g, b;
    
    Image result = Image(height, width);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel &f1 = factor1[y][x];
            const Pixel &f2 = factor2[y][x];
            r = (f1.r * f2.r > 255) ? 255 : f1.r * f2.r;
            g = (f1.g * f2.g > 255) ? 255 : f1.g * f2.g;
            b = (f1.b * f2.b > 255) ? 255 : f1.b * f2.b;
            result[y][x] = Pixel(r, g, b);
        }
    }
    return result;
}