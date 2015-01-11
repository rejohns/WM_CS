/* Confirmation of the probability calculation done by Till Rosenband,
 * using the mean & stdev values printed with his histogram, and
 * calculating the tail probability based on the bin boundary value 0.02.
 *
 * The output is 9.100498e-13.  Using 0.0223 in place of 0.02 produces
 * the output 1.665335e-16 (which is sort-of consistent with Rosenband's
 * value.
 */

#include <stdio.h>
#include <math.h>

#include "rvms.h"

int main(void)
{
  double u = 1.0 - cdfNormal(0.0052, 0.0021, 0.02);
  printf("%e\n", u);

  u = 1.0 - cdfBinomial(156361, 0.005034, 906);
  printf("%e\n", u);

  return(0);
} 
