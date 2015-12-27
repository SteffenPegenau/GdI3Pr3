#ifndef LUCY_H
#define LUCY_H

#include "Algorithm.h"

class Lucy : public Algorithm {
public:
    void process(const Parameters &params, const Image &src, Image &dst);

private:
    Image applyConstKernelOn(const Image &src);
    Image division(const Image &dividend, const Image &divisor);
    Image multiplication(const Image& factor1, const Image& factor2);
    
};

#endif
