#include <stdio.h>
#include <sys/resource.h>

#include "print.h"

void rusage_print(struct rusage *r, int pretty) {

	double t_usr, t_sys;

    t_usr = r->ru_utime.tv_sec + (r->ru_utime.tv_usec / 1000000.0);
    t_sys = r->ru_stime.tv_sec + (r->ru_stime.tv_usec / 1000000.0);

	if ( pretty )
		rusage_print_pretty(r, t_usr, t_sys);
	else
		rusage_print_plain(r, t_usr, t_sys);
}

void rusage_print_plain(struct rusage *r, double t_usr, double t_sys) {
    printf("%.6f", t_usr+t_sys);
    printf(" %.6f", t_usr);
    printf(" %.6f", t_sys);
    printf(" %ld", r->ru_maxrss);
    printf(" %ld", r->ru_minflt);
    printf(" %ld", r->ru_majflt);
    printf(" %ld", r->ru_inblock);
    printf(" %ld", r->ru_oublock);
    printf(" %ld", r->ru_nvcsw);
    printf(" %ld", r->ru_nivcsw);
	printf("\n");
}

void rusage_print_pretty(struct rusage *r, double t_usr, double t_sys) {
    printf("total    %.6f\n", t_usr+t_sys);
    printf("user     %.6f\n", t_usr);
    printf("sys      %.6f\n", t_sys);
    printf("max rss  %ld\n", r->ru_maxrss);
    printf("minflt   %ld\n", r->ru_minflt);
    printf("majflt   %ld\n", r->ru_majflt);
    printf("inblock  %ld\n", r->ru_inblock);
    printf("oublock  %ld\n", r->ru_oublock);
    printf("nvcsw    %ld\n", r->ru_nvcsw);
    printf("nivcsw   %ld\n", r->ru_nivcsw);
}
