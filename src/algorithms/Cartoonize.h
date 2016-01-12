#ifndef CARTOONIZE_H
#define CARTOONIZE_H

#include "Algorithm.h"

class Cartoonize : public Algorithm {
public:
    void process(const Parameters &params, const Image &src, Image &dst);

    Image bilateralFilter(const Image& src, const int& filterSize, const int& sigmaD, const int& sigmaR);

    double c_domain(int &x1, int &y1, int &x2, int &y2);
    double c_edge(const Image &f, int &x1, int &y1, int &x2, int &y2);

    double regionalDistanceSquare(int &x1, int &x2);
    double colorDistanceSquare(Pixel &p1, Pixel &p2);

    const Pixel &getPixel(const Image &pic, const int &width, const int &height, int x, int y);
    
    int toValidCoord(int &i, const int &max);

    double filterWeight(const Image &f, int &x1, int &y1, int &x2, int &y2);
};

#endif
