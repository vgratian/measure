#include <stdio.h>
#include <sys/resource.h>

#include "print.h"

#define MILLION 1000000.0

void rusage_print(struct rusage *r, double t, int pretty) {

	double t_usr, t_sys;

    t_usr = r->ru_utime.tv_sec + (r->ru_utime.tv_usec / MILLION);
    t_sys = r->ru_stime.tv_sec + (r->ru_stime.tv_usec / MILLION);

	if ( pretty )
		rusage_print_pretty(r, t, t_usr, t_sys);
	else
		rusage_print_plain(r, t, t_usr, t_sys);
}

void rusage_print_plain(struct rusage *r, double t_wall, double t_usr, double t_sys) {
    printf("%.6lf", t_usr+t_sys);
    printf(" %.6lf", t_usr);
    printf(" %.6lf", t_sys);
    printf(" %.6lf", t_wall);
    printf(" %ld", r->ru_maxrss);
    printf(" %ld", r->ru_minflt);
    printf(" %ld", r->ru_majflt);
    printf(" %ld", r->ru_inblock);
    printf(" %ld", r->ru_oublock);
    printf(" %ld", r->ru_nvcsw);
    printf(" %ld", r->ru_nivcsw);
	printf("\n");
}

void rusage_print_pretty(struct rusage *r, double t_wall, double t_usr, double t_sys) {
    printf("cpu time  %.6lf\n", t_usr+t_sys);
    printf(" user     %.6lf\n", t_usr);
    printf(" sys      %.6lf\n", t_sys);
    printf("wall time %.6lf\n", t_wall);
    printf("max rss   %ld\n", r->ru_maxrss);
    printf("minflt    %ld\n", r->ru_minflt);
    printf("majflt    %ld\n", r->ru_majflt);
    printf("inblock   %ld\n", r->ru_inblock);
    printf("oublock   %ld\n", r->ru_oublock);
    printf("nvcsw     %ld\n", r->ru_nvcsw);
    printf("nivcsw    %ld\n", r->ru_nivcsw);
}
