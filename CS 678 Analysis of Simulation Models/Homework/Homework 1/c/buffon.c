/* -------------------------------------------------------------------------
 * A Monte Carlo simulation of Buffon's needle experiment. 
 * 
 * Name              : buffon.c 
 * Author            : Steve Park & Dave Geyer 
 * Language          : ANSI C
 * Latest Revision   : 9-11-98 
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>
#include "rng.h"

#define HALF_PI (2.0 * atan(1.0))          /* 1.5707963...           */
#define R       1.0                        /* length of the needle   */


   double Uniform(double a, double b)      
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{
  return (a + (b - a) * Random());
}

  void run(long N)
{ 
  long   i;                                /* replication index     */
  long   crosses = 0;                      /* number of crosses     */
  double p;                                /* estimated probability */
  double u, v;                             /* endpoints             */
  double theta;                            /* angle                 */
  long   seed;                             /* the initial rng seed  */   

  PutSeed(-1);                 /* any negative integer will do      */
  GetSeed(&seed);              /* trap the value of the intial seed */

  for (i = 0; i < N; i++) {                
    u     = Random();                                   
    theta = Uniform(-HALF_PI, HALF_PI);
    v     = u + R * cos(theta);
    if (v > 1.0)
      crosses++;
  }

  p = (double) crosses / N;                /* estimate the probability */

  printf("%ld\t%lf\n", N, p);
  
}

int main(void){

	run(100);
	run(1000);
	run(10000);
	run(100000);
	run(1000000);
	
	return 0;
}