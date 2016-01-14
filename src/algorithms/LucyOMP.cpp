#include "LucyOMP.h"
#include <QTime>
#include <omp.h>
#include <cmath>

void LucyOMP::process(const Parameters &params, const Image &src, Image &dst) {
    QTime timer;
    timer.start();
    //************************************************************************
    // Access image data
    //************************************************************************
    dst = src;
    // get n
    const int n = params.lucyN;

    Image b, d;
    for (int i = 1; i <= n; i++) {
        printf("(%i / %i)\n", i, n);
        b = applyConstKernelOn(dst);
        division(src, b);
        d = applyConstKernelOn(b);
        multiplication(dst, d);
    }
    int laufzeitMS = timer.elapsed();
    double laufzeitPerIt = laufzeitMS / (double) params.lucyN;

    printf("Iterationen: %i\n", params.lucyN);
    printf("Dauer: %i ms\n", laufzeitMS);
    printf("=> ~%f ms/Iteration\n", laufzeitPerIt);
}

Image LucyOMP::applyConstKernelOn(const Image& src) {
    // get dimensions
    const int height = src.height();
    const int width = src.width();
    // Result Image
    Image result = src;


    // pseudo kernel Value
    const int k = 51;
    const int range = (51 - 1) / 2;

    #pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel &p = result[y][x];

            // Alle Werte im Kernel Bereich zu new-Werten addieren
            for (int r = x - range; r <= x + range; r++) {
                // Pixel liegt ausserhalb des Bildes
                if (r < 0 || r >= width) {
                    //printf("Pixel %i|%i liegt ausserhalb des Bildes (height=%i\twidht=%i)\n", y, x, height, width);
                    p.r += 255.0;
                    p.g += 255.0;
                    p.b += 255.0;
                } else {
                    // Bezugspixel im Original
                    const Pixel &pixel = src[y][r];
                    p.r += pixel.r;
                    p.g += pixel.g;
                    p.b += pixel.b;
                }
            }

            // Arith. Mittel bilden
            p.r = p.r / k;
            p.b = p.b / k;
            p.g = p.g / k;
            //printf("%f\t%f\t%f\n", p.r, p.b, p.g);
        }
    }
    return result;
}

/**
 * Dividiert den Dividenden durch den Divisor
 * 
 * Ergebnis in Divisor
 */
void LucyOMP::division(const Image& dividend, Image& divisor) {
    // get dimensions
    const int height = dividend.height();
    const int width = dividend.width();

    #pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel &r = dividend[y][x];
            Pixel &f = divisor[y][x];
            f.r = r.r / f.r;
            if(f.r > 255) f.r = 255;
            
            f.g = r.g / f.g;
            if(f.g > 255) f.g = 255;
            
            f.b = r.b / f.b;
            if(f.b > 255) f.b = 255;
        }
    }
}

/**
 * Multipliziert auf factor1 den faktor2 (Ergebnis in factor 1)
 */
void LucyOMP::multiplication(Image& factor1, const Image& factor2) {
    // get dimensions
    const int height = factor1.height();
    const int width = factor1.width();

    
    #pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel &r = factor1[y][x];
            const Pixel &f = factor2[y][x];
            r.r *= f.r;
            if(r.r > 255) r.r = 255;
            
            r.g *= f.g;
            if(r.g > 255) r.g = 255;
            
            r.b *= f.b;
            if(r.b > 255) r.b = 255;
        }
    }
}
