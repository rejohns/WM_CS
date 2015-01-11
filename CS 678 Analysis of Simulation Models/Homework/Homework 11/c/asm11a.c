/* ------------------------------------------------------------------------- 
 * This program is a next-event simulation of a single-server FIFO service
 * node using Exponentially distributed interarrival times and Uniformly 
 * distributed service times (i.e., a M/U/1 queue).  The service node is 
 * assumed to be initially idle, no arrivals are permitted after the 
 * terminal time STOP, and the service node is then purged by processing any 
 * remaining jobs in the service node.
 *
 * Name            : ssq3.c  (Single Server Queue, version 3)
 * Author          : Steve Park & Dave Geyer
 * Language        : ANSI C
 * Latest Revision : 10-19-98
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rngs.h"                      /* the multi-stream generator */

#define START         0.0              /* initial time                   */
#define STOP      20000.0              /* terminal (close the door) time */
#define INFINITY   (100.0 * STOP)      /* must be much larger than STOP  */



double NOBM(double array[], int batch) {
  int i;
  double mean = 0;
  for (i = batch*401; i < (batch+1)*401; i++)
    mean += array[i]/401.0;
  return(mean);
}

double OBM(double array[], int index) {
  int i;
  double mean = 0;
  for (i = index; i < index + 401; i++)
    mean += array[i]/401.0;
  return(mean);
}



   double Min(double a, double c)
/* ------------------------------
 * return the smaller of a, b
 * ------------------------------
 */
{ 
  if (a < c)
    return (a);
  else
    return (c);
} 


   double Exponential(double m)
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{
  return (-m * log(1.0 - Random()));
}


   double Uniform(double a, double b)
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{
  return (a + (b - a) * Random());
}


   double GetArrival()
/* ---------------------------------------------
 * generate the next arrival time, with rate 1/2
 * ---------------------------------------------
 */ 
{
  static double arrival = START;

  SelectStream(0); 
  arrival += Exponential(2.0);
  return (arrival);
} 


   double GetService()
/* --------------------------------------------
 * generate the next service time with rate 2/3
 * --------------------------------------------
 */ 
{
  SelectStream(1);
  return (Uniform(1.0, 2.0));
}  

/*------------------------------------------------------------------------*/
int cmpfunc (const void * x, const void * y)    /* for qsort              */
{
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy)
    return -1;
  if (xx > yy)
    return 1;
  else
    return 0;
}
/*------------------------------------------------------------------------*/


  int main(void)
{
  struct {
    double arrival;                 /* next arrival time                   */
    double completion;              /* next completion time                */
    double current;                 /* current time                        */
    double next;                    /* next (most imminent) event time     */
    double last;                    /* last arrival time                   */
  } t;
  struct {
    double node;                    /* time integrated number in the node  */
    double queue;                   /* time integrated number in the queue */
    double service;                 /* time integrated number in service   */
  } area      = {0.0, 0.0, 0.0};
  long index  = 0;                  /* used to count departed jobs         */
  long number = 0;                  /* number in the node                  */
  /*-----------------------------------------------------------------------*/
  int i       = 0;                  /* index for arrivals array            */
  int j       = 0;                  /* index for completions array         */
  int k       = 0;                  /* index for wait times array          */
  int l       = 0;                 
  int m       = 0;                 

  double arrivals[10025];
  double completions[10025];
  double waits[10025];

  double avg_wait;
  double S;
  double S2;
  double nobm;
  double obm;
  double V;
  double A;
  /*-----------------------------------------------------------------------*/

  PlantSeeds(123456789);
  t.current    = START;           /* set the clock                         */
  t.arrival    = GetArrival();    /* schedule the first arrival            */
  t.completion = INFINITY;        /* the first event can't be a completion */

  while ((t.arrival < STOP) || (number > 0)) {
    t.next          = Min(t.arrival, t.completion);  /* next event time   */
    if (number > 0)  {                               /* update integrals  */
      area.node    += (t.next - t.current) * number;
      area.queue   += (t.next - t.current) * (number - 1);
      area.service += (t.next - t.current);
    }
    t.current       = t.next;                    /* advance the clock */

    if (t.current == t.arrival)  {               /* process an arrival */
    /*---------------------------------*/
      arrivals[i] = t.current;
      i++;
    /*---------------------------------*/
      number++;
      t.arrival     = GetArrival();
      if (t.arrival > STOP)  {
        t.last      = t.current;
        t.arrival   = INFINITY;
      }
      if (number == 1)
        t.completion = t.current + GetService();
    }

    else {                                        /* process a completion */
    /*---------------------------------*/
      completions[j] = t.current;
      j++;
    /*---------------------------------*/
      index++;
      number--;
      if (number > 0)
        t.completion = t.current + GetService();
      else
        t.completion = INFINITY;
    }
  }

  /*---------------------------------*/       
  for (k = 0; k <= 10025; k++)                   /* Generates wait times*/
    waits[k] = completions[k] - arrivals[k];

  avg_wait = area.node / index;                  /* Compute average wait time*/

  for (k = 0; k < 10025; k++){                   /* Classic Confidence Interval */
    S += pow((waits[k] - avg_wait), 2)/10024.0;
    S = sqrt(S);
  }

  for (k = 0; k < 25; k++){                      /* NOBM */
    nobm = NOBM(waits, k);
    S2 += pow((nobm - avg_wait), 2) / 24.0;
  }

  for (k = 0; k < 9625; k++){                    /* OBM */
    obm = OBM(waits, k);
    V += pow((obm - avg_wait), 2) * 401.0 / (9624.0 * 9625.0);
  }

  for (k = 0; k < 25; k++){                      /* Std Time Series */
    nobm = NOBM(waits, k);
    for (l = 0; l < 401; l++){
      for (m = 0; m <= l; m++){
	A += (12.0 / (pow(401.0, 3.0) - 401.0)) * pow((nobm - waits[m+k*401]), 2);
      }
    }
  }
    
  printf("\n");
  printf("(i)   Classic Confidence Interval:\n");
  printf("      (%f,%f)\n",
    avg_wait - 2.063899*S/sqrt(25.0), avg_wait + 2.063899*S/sqrt(25.0));
  printf("\n");
  printf("(ii)  Nonoverlapping Batch Mean interval:\n");
  printf("      (%f,%f)\n",
    avg_wait - 2.063899*sqrt(S2/25.0), avg_wait + 2.063899*sqrt(S2/25.0));
  printf("\n");
  printf("(iii) Overlapping Batch Mean interval:\n");
  printf("      (%f,%f)\n", avg_wait - 2.026192*sqrt(V), avg_wait + 2.026192*sqrt(V));
  printf("\n");
  printf("(vii) Standardized Time Series interval:\n");
  printf("      (%f,%f)\n", 
    avg_wait - 2.059539*sqrt(A/(index*25)), avg_wait + 2.059539*sqrt(A/(index*25)));
  printf("\n");
  printf("mean wait: %f\n", avg_wait);
}