/* ------------------------------------------------------------------------
 * Name              : cdh.c  (Continuous Data Histogram)
 * Authors           : Steve Park & Dave Geyer (modified by Robert Johns)
 * Language          : ANSI C
 * Latest Revision   : 10-16-2013
 * Modified code surrounded by inline comments
 * ------------------------------------------------------------------------ 
 */

#include <stdio.h>
#include <math.h> 
#include "rngs.h"

 // new code
#define  MIN     0.0
#define  MAX     2.0
#define  K       20							 /* number of histogram bins   */       
 // end new code
#define  DELTA   ((MAX - MIN) / K)           /* histogram bin size (width) */
#define  sqr(x)  ((x) * (x))

  int main(void)
{
  double x;                                      /* data value          */
  long   j;                                      /* histogram bin index */
  long   index    = 0;                           /* sample size         */
  long   count[K] = {0.0};                       /* bin count           */
  double midpoint[K];                            /* bin midpoint        */
  struct {                                       
    long lo;
    long hi;
  } outliers      = {0, 0};
  double sum      = 0.0;
  double sumsqr   = 0.0;
  double mean;
  double stdev;

  for (j = 0; j < K; j++) 
    midpoint[j] = MIN + (j + 0.5) * DELTA;

	  // new code
	int counting = 0;
  	while (counting < 10000) {                         /* tally the data */
    	x = Random() + Random();
    	index++;
   	 if ((x >= MIN) && (x < MAX)) {
   	   j = (long) ((x - MIN) / DELTA);
    	  count[j]++;
   	 }
   	 else if (x < MIN) {
    	  outliers.lo++;
    	  }
   	 else{
    	  outliers.hi++;
    	  }
		counting++;
  }
    // end new code

  for (j = 0; j < K; j++)                        /* histogram mean */
    sum += midpoint[j] * count[j];
  mean   = sum / index;

  for (j = 0; j < K; j++)                        /* histogram stdev */
    sumsqr += sqr(midpoint[j] - mean) * count[j];
  stdev     = sqrt(sumsqr / index);

	// new code
  printf("  bin     midpoint     count   proportion    density\n\n");
  for (j = 0; j < K; j++) { 
    printf("%5ld &", j + 1);                                    /* bin        */
    printf("%12.3f &", midpoint[j]);                            /* midpoint   */
    printf("%10ld &", count[j]);                                /* count      */
    printf("%12.3f &", (double) count[j] / index);              /* proportion */
    printf("%12.3f\\\\\n", (double) count[j] / (index * DELTA));  /* density    */
  }
    // end new code
  printf("\nsample size .... = %7ld\n", index);
  printf("mean ........... = %7.3f\n", mean);
  printf("stdev .......... = %7.3f\n\n", stdev);
  if (outliers.lo > 0)
    printf("NOTE: there were %ld low outliers\n", outliers.lo);
  if (outliers.hi > 0)
    printf("NOTE: there were %ld high outliers\n", outliers.hi);

  return (0);
}
