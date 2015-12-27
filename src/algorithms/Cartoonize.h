#ifndef CARTOONIZE_H
#define CARTOONIZE_H

#include "Algorithm.h"

class Cartoonize : public Algorithm
{
public:
   void process(const Parameters &params, const Image &src, Image &dst);

};

#endif
