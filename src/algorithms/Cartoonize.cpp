#include "Cartoonize.h"

#include <omp.h>
#include <cmath>

int Cartoonize::toValidCoord(int &i, const int &max) {
    if(i < 0) return 0;
    if(i >= max) return max;
    return i;
}

/**
 * Berechnet das Filtergewicht eines Pixelpaares 
 */
double Cartoonize::filterWeight(const Image &f, int &x1, int &y1, int &x2, int &y2) {
    return 1.0;
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
    Pixel sum;
    int tmpX, tmpY;
    
    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            // Init für neue Pixelberechnung
            sum.r = 0;
            sum.g = 0;
            sum.b = 0;
            wSum = 0;
            for (int ry = y - range; ry <= y + range; ry++) {
                for (int rx = x - range; rx <= x+range; rx++) {
                    w = filterWeight(src, x, y, rx, ry);
                    //const Pixel &f = getPixel(src, X, Y,rx, ry);
                    tmpX = toValidCoord(rx, X);
                    tmpY = toValidCoord(ry, Y);
                    const Pixel &f = src[tmpY][tmpX];
                    
                    // Zähler
                    sum.r += f.r * w;
                    sum.g += f.g * w;
                    sum.b += f.b * w;
                    
                    // Nenner
                    wSum += w;
                }
            }
            Pixel &p = g[y][x];
            g[y][x].r = sum.r / wSum;
            g[y][x].g = sum.g / wSum;
            g[y][x].b = sum.b / wSum;
        }
    }
    return g;
}

void Cartoonize::process(const Parameters &params, const Image &src, Image &dst) {
    //************************************************************************
    // Access image data
    //************************************************************************
    dst = src;

    const int filterSize = params.filterSize;
    const int sigmaD = params.sigmaD;
    const int sigmaR = params.sigmaR;
    const int tau = params.tau;

    // get dimensions
    const int M = src.height();
    const int N = src.width();


    //************************************************************************
    // How to use parameters from gui
    //************************************************************************
    printf("Filtergroesse: %i\n", filterSize);
    ;
    printf("Sigma D: %i\n", sigmaD);
    printf("Sigma R: %i\n", sigmaR);
    printf("Sigma R: %i\n", tau);

    Image g = bilateralFilter(src, filterSize, sigmaD, sigmaR);

    dst = g;
}


/**
 * Gibt korrekten Pixel zurück, 
 * auch wenn x oder y außerhalb des Bildes liegen

const Pixel &Cartoonize::getPixel(const Image &pic, const int &width, const int &height, int x, int y) {
    x = (x < 0) ? 0 : x;
    x = (x >= width) ? width : x;
    y = (y < 0) ? 0 : y;
    y = (y >= height) ? height : y;
    Pixel &f = pic[y][x];
    return f;
}
*/



/**
 * Berechnet die räumliche Distanz zweier Koordinatenkomponenten
 */
double regionalDistanceSquare(int &x1, int &x2) {
    
}

/**
 * Berechnet die farbliche Distanz zweier Pixel
 */
double Cartoonize::colorDistanceSquare(Pixel &p1, Pixel & p2) {
    
}

/**
 * Berechnet die raeumliche Charakteristik zweier Koordinaten
 */
double Cartoonize::c_domain(int &x1, int &y1, int &x2, int &y2) {

}

/**
 * Berechnet die farbliche Charakteristik zweier Pixel
 */
double Cartoonize::c_edge(const Image &f, int &x1, int &y1, int &x2, int &y2) {

}

