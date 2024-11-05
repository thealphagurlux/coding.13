#include <stdio.h>  
#include <stdlib.h> 
#include <time.h>   
#include <unistd.h> 
//Homework answers//
//
// #1) CLOCK_PROCESS_CPUTIME_ID has resolution of 1 nanosecond on Linux
//
// #5) Using gcc's optimize option gcc -O0 to disable optimization. This is the default setting.
//
// #6) Use sched_setaffinity(2), pthread_setaffinity_np(3), taskset(1) or sudo systemd-run -p AllowedCPUs=0 ./tlb.out on Linux, cpuset_setaffinity(2) or cpuset(1) on FreeBSD.
//  Or use hwloc-bind package:0.pu:0 -- ./tlb.out.
//
// #7) Use calloc(3) to initialize array then measure time.
////////////////////

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s pages trials\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  long PAGESIZE = sysconf(_SC_PAGESIZE); // 4096
  long jump = PAGESIZE / sizeof(int);    // 1024
  int pages = atoi(argv[1]);
  int trials = atoi(argv[2]);
  if (pages <= 0 || trials <= 0) {
    fprintf(stderr, "Invalid input\n");
    exit(EXIT_FAILURE);
  }
  int *a = calloc(pages, PAGESIZE);
  struct timespec start, end;
  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start) == -1)
    handle_error("clock_gettime");

  for (int j = 0; j < trials; j++) {
    for (int i = 0; i < pages * jump; i += jump)
      a[i] += 1;
  }

  if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end) == -1)
    handle_error("clock_gettime");

  // nanoseconds
  printf("%f\n",
         ((end.tv_sec - start.tv_sec) * 1E9 + end.tv_nsec - start.tv_nsec) /
             (trials * pages));
  free(a);
  return 0;
}