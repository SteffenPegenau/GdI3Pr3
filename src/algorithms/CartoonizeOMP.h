#ifndef CARTOONIZEOMP_H
#define CARTOONIZEOMP_H

#include "Algorithm.h"

class CartoonizeOMP : public Algorithm
{
public:
   void process(const Parameters &params, const Image &src, Image &dst);

    Image greyscale(const Image& src);
    void kantendetektion(Image& src, const int tau);
    
    Image bilateralFilter(const Image& src, const int& filterSize, const int& sigmaD, const int& sigmaR);
    
    double c_domain(int sigmaD, int x1, int y1, int x2, int y2);
    double c_edge(const Image &f, const int sigmaR, const int x1, const int y1, const int x2, const int y2);
    double colorDistance(const Pixel p1, const Pixel p2);

    const Pixel &getPixel(const Image &pic, const int &width, const int &height, int x, int y);

    int toValidCoord(int &i, const int &max);

    double filterWeight(const Image &f, const int sigmaD, const int sigmaR, int x1, int y1, int x2, int y2);
};

#endif
