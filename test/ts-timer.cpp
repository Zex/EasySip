#include <signal.h>
#include <time.h>
#include <string.h>
#include <error.h>
#include <iostream>
#include <sys/time.h>

//union sigval {          /* Data passed with notification */
//           int     sival_int;         /* Integer value */
//           void   *sival_ptr;         /* Pointer value */
//       };
//
//       struct sigevent {
//           int          sigev_notify; /* Notification method */
//           int          sigev_signo;  /* Notification signal */
//           union sigval sigev_value;  /* Data passed with
//                                         notification */
//           void       (*sigev_notify_function) (union sigval);
//                            /* Function used for thread
//                               notification (SIGEV_THREAD) */
//           void        *sigev_notify_attributes;
//                            /* Attributes for notification thread
//                               (SIGEV_THREAD) */
//           pid_t        sigev_notify_thread_id;
//                            /* ID of thread to signal (SIGEV_THREAD_ID) */
//       };

extern int errno;

void sigev_notify_cb(union sigval sv)
{
	std::cout << "sigev_notify_function: " << sv.sival_int << '\n';
}

//           struct timespec {
//               time_t tv_sec;                /* Seconds */
//               long   tv_nsec;               /* Nanoseconds */
//           };
//
//           struct itimerspec {
//               struct timespec it_interval;  /* Timer interval */
//               struct timespec it_value;     /* Initial expiration */
//
//       int timer_settime(timer_t timerid, int flags,
//                         const struct itimerspec *new_value,
//                         struct itimerspec * old_value);
//		int timer_gettime(timer_t timerid, struct itimerspec *curr_value);

//int main()
//{
//	int ret = 0;
//
//	struct sigevent sige;
//	timer_t tid;
//
//	sige.sigev_notify_function = sigev_notify_cb;
//	sige.sigev_notify = SIGEV_THREAD;//SIGEV_SIGNAL;
//	sige.sigev_signo = SIGRTMIN;
//
//    std::cout << "timer_create: " << (ret = timer_create(CLOCK_REALTIME, &sige, &tid)) << '\n';
//
//	struct itimerspec itmspec, *itmspec_cur = new itimerspec;
//	struct timespec tmspec_intv, tmspec_expir;
//
//	tmspec_intv.tv_sec = 3;
//	tmspec_intv.tv_nsec = 0;
//	tmspec_expir.tv_sec = 3;
//	tmspec_expir.tv_nsec = 0;
//
//	itmspec.it_interval = tmspec_intv;
//	itmspec.it_value = tmspec_expir;
//
//	std::cout << "timer_gettime: " << (ret = timer_gettime(tid, itmspec_cur)) << '\n';
//	std::cout << "timer_settime: " << (ret = timer_settime(tid, TIMER_ABSTIME, &itmspec, itmspec_cur)) << '\n';
//	std::cout << "timer_delte: " << (ret = timer_delete(tid)) << '\n';
//
//	delete itmspec_cur;
//
//	return ret;
//}
//

//struct itimerval {
//               struct timeval it_interval; /* next value */
//               struct timeval it_value;    /* current value */
//           };
//
//           struct timeval {
//               time_t      tv_sec;         /* seconds */
//               suseconds_t tv_usec;        /* microseconds */
//           };
//	bool cb1()
//	{
//		std::cout << "cb1 signo received, time's up\n";
//		return false;
//	}
//
struct itimerval it_a;

void sigalrm_cb(int signo)
{
	std::cout << signo << " signo received, time's up\n";
	//signal(SIGALRM, SIG_DFL);
	std::cout << "settimer: " << setitimer(ITIMER_REAL, 0, &it_a) << '\n';
}

int main()
{
	signal(SIGALRM, sigalrm_cb);

	struct itimerval it_a;
	struct timeval tm_cur, tm_next;

	tm_cur.tv_sec = 1;
	tm_cur.tv_usec = 0;

	tm_next.tv_sec = 3;
	tm_next.tv_usec = 0;

	it_a.it_interval = tm_next;
	it_a.it_value = tm_cur;

//	std::cout << "settimer: " << setitimer(ITIMER_REAL, &it_a, 0) << '\n';
	char c;
	std::cin.get(c);
//	while(1);
	std::cout << "timercmp(&tm_cur, tm_next, ==) " << timercmp(&tm_cur, &tm_next, ==) << '\n';

	return 0;
}

