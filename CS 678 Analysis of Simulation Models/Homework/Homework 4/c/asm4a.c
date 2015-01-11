/* ------------------------------------------------------------------------- 
 * This program is a next-event simulation of a single-server FIFO service
 * node using Exponentially distributed interarrival times and Uniformly 
 * distributed service times (i.e., a M/U/1 queue).  The service node is 
 * assumed to be initially idle, no arrivals are permitted after the 
 * terminal time 20000.0, and the service node is then purged by processing any 
 * remaining jobs in the service node.
 *
 * Name            : ssq3.c  (Single Server Queue, version 3)
 * Author          : Steve Park & Dave Geyer
 * Language        : ANSI C
 * Latest Revision : 10-19-98
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
/* NEW CODE FOR HOMEWORK 1 */
#include <stdlib.h>
/* END NEW CODE FOR HOMEWORK 1 */
#include <math.h>
#include "rngs.h"                      /* the multi-stream generator */

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


   double GetArrival(double arrive)
/* ---------------------------------------------
 * generate the next arrival time, with rate 1/2
 * ---------------------------------------------
 */ 
{

	double arrival = arrive;

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

  double run(long seed)
{
  int firstTime = 1;
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

  PlantSeeds(seed);
  
  t.current    = 0.0;           /* set the clock                         */
  t.arrival    = GetArrival(0.0);    /* schedule the first arrival            */
  firstTime = 0;
  t.completion = 2000000.0;        /* the first event can't be a completion */

  while ((t.arrival < 20000.0) || (number > 0)) {
    t.next          = Min(t.arrival, t.completion);  /* next event time   */
    if (number > 0)  {                               /* update integrals  */
      area.node    += (t.next - t.current) * number;
      area.queue   += (t.next - t.current) * (number - 1);
      area.service += (t.next - t.current);
    }
    t.current       = t.next;                    /* advance the clock */

    if (t.current == t.arrival)  {               /* process an arrival */
      number++;
      t.arrival     = GetArrival(t.arrival);
      if (t.arrival > 20000.0)  {
        t.last      = t.current;
        t.arrival   = 2000000.0;
      }
      if (number == 1)
        t.completion = t.current + GetService();
    }

    else {                                        /* process a completion */
      index++;
      number--;
      if (number > 0)
        t.completion = t.current + GetService();
      else
        t.completion = 2000000.0;
    }
  } 

 return( area.node / (float) index);
  
}

int main(void) {
	double waits[100];
	double sumrunz;
	int i;
	double runz;
	for(i = 0; i < 100; i++) {
		runz = run(693142 - 79 * i);
		printf("%lf\n", runz);
		waits[i] = runz;
		sumrunz += runz;
	}
	printf("Average: %lf\n", sumrunz / 100.0);
	
	exit(0);
}