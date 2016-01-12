#include "Cartoonize.h"

#include <omp.h>
#include <cmath>

void Cartoonize::process(const Parameters &params, const Image &src, Image &dst)
{
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
   printf("Filtergroesse: %i\n", filterSize);;
   printf("Sigma D: %i\n", sigmaD);
   printf("Sigma R: %i\n", sigmaR);
   printf("Sigma R: %i\n", tau);


}
