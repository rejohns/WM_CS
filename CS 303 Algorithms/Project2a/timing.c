#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>

/*****************************************************************************
 * user_time() and system_time() return the elapsed user and system times
 * for a process as computed a la Unix.
 *
 * From the time(3V) man page...
 *
 *    struct tms {
 *         clock_t   tms_utime;          // user time
 *         clock_t   tms_stime;          // system time
 *         clock_t   tms_cutime;         // user time, children
 *         clock_t   tms_cstime;         // system time, children
 *    };
 *
 *    This information comes from the calling process and each  of
 *    its  terminated  child processes for which it has executed a
 *    wait(2V).
 *
 *    tms_utime is the CPU time used while executing instructions
 *    in the user space of the calling process.
 *
 *    tms_stime is the CPU time used by the system on behalf of
 *    the calling process.
 *
 *    tms_cutime is the sum of the tms_utimes and  tms_cutimes  of
 *    the child processes.
 *
 *    tms_cstime is the sum of the tms_stimes and  tms_cstimes  of
 *    the child processes.
 *
 *    From: Robert Michael Lewis
 *
 */

double user_time()
{
  struct tms now;

  times(&now);
  return( (double) ((double) now.tms_utime / (double) HZ) );
}

double system_time()
{
  struct tms now;

  times(&now);
  return( (double) ((double) now.tms_stime / (double) HZ) );
}
