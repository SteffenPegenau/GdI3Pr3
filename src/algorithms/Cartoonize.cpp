#include "Cartoonize.h"

#include <omp.h>
#include <cmath>


int Cartoonize::toValidCoord(int &i, const int &max) {
    if (i < 0) return 0;
    if (i >= max) return max - 1;
    return i;
}

/**
 * Erwartet ein Bild mit den Grauwerten in R!
 * 
 * Führt Kantendetektion durch, anschließend befindet sich das Kantenbild in b
 */
void Cartoonize::kantendetektion(Image& src, const int tau) {
    
    const int Y = src.height();
    const int X = src.width();
    
    // x - 1 und x + 1
    int xm1,xp1;
    // y - 1 und y + 1
    int ym1, yp1;
    
    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            Pixel &p = src[y][x];
            xm1 = x - 1;
            xp1 = x + 1;
            ym1 = y - 1;
            yp1 = y + 1;
            
            // Ableitung in x-Richtung in .g
            p.g = src[toValidCoord(ym1, Y)][toValidCoord(xm1, X)].r;
            p.g += src[toValidCoord(ym1, Y)][toValidCoord(xp1, X)].r * (-1);
            p.g += src[toValidCoord(y, Y)][toValidCoord(xm1, X)].r * 2;
            p.g += src[toValidCoord(y, Y)][toValidCoord(xp1, X)].r * (-2);
            p.g += src[toValidCoord(yp1, Y)][toValidCoord(xm1, X)].r;
            p.g += src[toValidCoord(yp1, Y)][toValidCoord(xp1, X)].r * (-1);
            
            // Ableitung in y-Richtung in .b
            p.b = src[toValidCoord(ym1, Y)][toValidCoord(xm1, X)].r;
            p.b += src[toValidCoord(ym1, Y)][toValidCoord(x, X)].r * 2;
            p.b += src[toValidCoord(ym1, Y)][toValidCoord(xp1, X)].r;
            p.b += src[toValidCoord(yp1, Y)][toValidCoord(xm1, X)].r * (-1);
            p.b += src[toValidCoord(yp1, Y)][toValidCoord(x, X)].r * (-2);            
            p.b += src[toValidCoord(yp1, Y)][toValidCoord(xp1, X)].r * (-1);
            
            // G in .g
            p.g = sqrt(pow(p.g,2) + pow(p.b,2));
            // Binaerkantenbild in .b
            p.b = (p.g < tau) ? 0 : p.g;
        }
    }
}

/**
 * Speichert den Grauwert in R.
 * 
 * G und B sind 0.0
 */
Image Cartoonize::greyscale(const Image& src) {
    Image grey = src;
    // get dimensions
    const int Y = src.height();
    const int X = src.width();

    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            Pixel &p = grey[y][x];
            p.r = 0.2989 * p.r + 0.587 * p.g + 0.114 * p.b;
            p.g = 0.0;
            p.b = 0.0;
        }
    }
    return grey;
}



/**
 * Berechnet die raeumliche Charakteristik zweier Koordinaten
 */
double Cartoonize::c_domain(int sigmaD, int x1, int y1, int x2, int y2) {
    double result = pow((x1 - x2), 2) + pow((y1 - y2), 2); // Zähler
    result = result / (2 * pow(sigmaD, 2)); // Nenner
    result *= -1;
    return result; // Vorzeichen
}

/**
 * Berechnet die farbliche Distanz zweier Pixel
 */
double Cartoonize::colorDistance(const Pixel p1, const Pixel p2) {
    return sqrt(
            pow(p1.r - p2.r, 2) +
            pow(p1.g - p2.g, 2) +
            pow(p1.b - p2.b, 2)
            );
}

/**
 * Berechnet die farbliche Charakteristik zweier Pixel
 */
double Cartoonize::c_edge(const Image &f, const int sigmaR, int x1, int y1, int x2, int y2) {
    const Pixel p1 = f[y1][x1];
    const Pixel p2 = f[y2][x2];
    double cd = colorDistance(p1, p2);
    return  -pow(cd, 2) / (2 * pow(sigmaR, 2));
}

/**
 * Berechnet das Filtergewicht eines Pixelpaares 
 */
double Cartoonize::filterWeight(const Image &f, const int sigmaD, const int sigmaR, int x1, int y1, int x2, int y2) {
    //return exp(c_domain(sigmaD, x1, y1, x2, y2));
    double cdomain = c_domain(sigmaD, x1, y1, x2, y2);
    double cedge = c_edge(f, sigmaR, x1, y1, x2, y2);
    //double cedge = 0;
    return exp(cdomain + cedge);
}

/**
 * Berechnet das Ergebnisbild nach bilateralem Filtern
 */
Image Cartoonize::bilateralFilter(const Image& src, const int& filterSize, const int& sigmaD, const int& sigmaR) {
    Image g = src;
    // get dimensions
    const int Y = src.height();
    const int X = src.width();
    int range = (filterSize - 1) / 2;
    double w = 0;
    double wSum = 0;
    double red, green, blue;
    int tmpX, tmpY;

    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            // Init für neue Pixelberechnung
            Pixel &p = g[y][x];
            red = 0;
            green = 0;
            blue = 0;
            wSum = 0;
            for (int ry = y - range; ry <= y + range; ry++) {
                for (int rx = x - range; rx <= x + range; rx++) {
                    w = filterWeight(src, sigmaD, sigmaR, toValidCoord(x, X), toValidCoord(y, Y), toValidCoord(rx, X), toValidCoord(ry, Y));
                    tmpX = toValidCoord(rx, X);
                    tmpY = toValidCoord(ry, Y);
                    const Pixel &f = src[tmpY][tmpX];

                    // Zähler
                    red += f.r * w;
                    green += f.g * w;
                    blue += f.b * w;

                    // Nenner
                    wSum += w;

                }
            }
            p.r = red / wSum;
            p.g = green / wSum;
            p.b = blue / wSum;
        }
    }
    return g;
}

void Cartoonize::process(const Parameters &params, const Image &src, Image &dst) {
    //************************************************************************
    // Access image data
    //************************************************************************
    //dst = src;

    const int filterSize = params.filterSize;
    const int sigmaD = params.sigmaD;
    const int sigmaR = params.sigmaR;
    const int tau = params.tau;

    // get dimensions
    const int Y = src.height();
    const int X = src.width();


    //************************************************************************
    // How to use parameters from gui
    //************************************************************************
    printf("Filtergroesse: %i\n", filterSize);
    ;
    printf("Sigma D: %i\n", sigmaD);
    printf("Sigma R: %i\n", sigmaR);
    printf("Sigma R: %i\n", tau);

    dst = bilateralFilter(src, filterSize, sigmaD, sigmaR);
    
    Image kanten = greyscale(dst);
    kantendetektion(kanten, tau);
    
    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            Pixel &pk = kanten[y][x];
            if(pk.b != 0) {
                Pixel &p = dst[y][x];
                p.r = 0;
                p.g = 0;
                p.b = 0;
            }
        }
    }
}

