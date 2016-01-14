#ifndef LUCYOMP_H
#define LUCYOMP_H

#include "Algorithm.h"

class LucyOMP : public Algorithm {
public:
    void process(const Parameters &params, const Image &src, Image &dst);

private:
    void division(const Image &dividend, Image &divisor);
    void multiplication(Image& factor1, const Image& factor2);
    Image applyConstKernelOn(const Image& src);
};

#endif
