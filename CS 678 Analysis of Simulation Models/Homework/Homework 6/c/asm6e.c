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
/* NEW CODE */
#include <stdlib.h>
/* END NEW CODE */
#include <math.h>
#include "rngs.h"                      /* the multi-stream generator */

#define START         0.0              /* initial time                   */
#define STOP      20000.0              /* terminal (close the door) time */
#define INFTY   (100.0 * STOP)      /* must be much larger than STOP  */


/* NEW CODE */
 int sort (const void * x, const void * y) {
    double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) return -1;
  if (xx > yy) return  1;
  return 0;
}
/* END NEW CODE */

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


// idea for wait times: use queue to keep track of the arrival times of jobs to be
//  completed
// yeah, that'll work
// but queues in c are a pain in the ass...
// linked lists especially are a pain in the ass
// maybe I should type this up in Python?
// nah, then I'd have to rewrite rngs.c
// and no one wants that
// blerg

  int run(void)
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
  
  /* NEW CODE */
  long jobs = STOP;
  double arrivals[jobs];
  double completions[jobs];
  double waits[jobs];  
  long arrivalcounter = 0;
  long completioncounter = 0;
  /* END NEW CODE */
  
  t.current    = START;           /* set the clock                         */
  t.arrival    = GetArrival(0.0);    /* schedule the first arrival            */
  t.completion = INFTY;        /* the first event can't be a completion */

  while ((t.arrival < STOP) || (number > 0)) {
    t.next          = Min(t.arrival, t.completion);  /* next event time   */
    if (number > 0)  {                               /* update integrals  */
      area.node    += (t.next - t.current) * number;
      area.queue   += (t.next - t.current) * (number - 1);
      area.service += (t.next - t.current);
    }
    t.current       = t.next;                    /* advance the clock */

    if (t.current == t.arrival)  {               /* process an arrival */
      /* NEW CODE */
      arrivals[arrivalcounter] = t.current;
      arrivalcounter++;
      /* END NEW CODE */
      number++;
      t.arrival     = GetArrival(t.arrival);
      if (t.arrival > STOP)  {
        t.last      = t.current;
        t.arrival   = INFTY;
      }
      if (number == 1)
        t.completion = t.current + GetService();
    }

    else {                                        /* process a completion */
      /* NEW CODE */
      completions[completioncounter] = t.current;
      completioncounter++;
      /* END NEW CODE */
      index++;
      number--;
      if (number > 0)
        t.completion = t.current + GetService();
      else
        t.completion = INFTY;
    }
  } 
  
  /* NEW CODE */
  jobs = index;
  long j;
  for(j = 0; j < jobs; j++)
  	waits[j] = completions[j] - arrivals[j];

  qsort(waits, jobs, sizeof(double), sort);
  
  long median;
  double medval;
  
  if (jobs % 2 == 0) {
  	double temp = waits[jobs / 2];
  	double temp2 = waits[(jobs / 2) - 1];
  	medval = (temp + temp2) / 2;
  	
  }
  else {
  	median =  jobs / 2;
    medval = waits[median];
  }
  
  printf("%ld\t%lf\n", jobs, medval);

  return (0);
}

int main(void) {
	
	PlantSeeds(123456789);
	
	int i;
	for(i = 0; i < 500; i++) {
		run();
	}
	
	return 0;
}
