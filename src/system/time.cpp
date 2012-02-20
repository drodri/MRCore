#include "time.h"
#include <sys/timeb.h>

namespace mr{

bool MRTime::microPrecision=false;

ostream& operator<<(ostream& os, const MRTime& t)
{
	if(MRTime::microPrecision)
		os<<t.seconds<<" "<<t.microseconds;
	else
		os<<t.seconds<<" "<<t.microseconds/1000;
	return os;
}
void MRTime::tic()
{
	struct timeb t1;
	ftime(&t1);
	seconds=(int)t1.time;
	microseconds=t1.millitm*1000;
}
	
long int MRTime::toc(bool micros)
{
	struct timeb t2;
	ftime(&t2);
	long int secs=(int)t2.time;
	long int mics=t2.millitm*1000;

	long int elapsed_time =((int)((secs-seconds)*1e6+(mics-microseconds)));
	if(!micros)
		elapsed_time/=1000;
	
	return elapsed_time;

}

#ifndef _WIN32 
	#ifdef HAVE_CLOCK_GETTIME
		#ifdef CLOCK_HIGHRES
			#define SAMPLED_CLOCK CLOCK_HIGHRES
		#else
			#define SAMPLED_CLOCK CLOCK_REALTIME
		#endif

		void MRTime::precistic() {
			
			if (clock_gettime(SAMPLED_CLOCK, &tv)) 
				tv.tv_sec = tv.tv_nsec = -1;
		}

		double MRTime::precistoc() {
			struct timespec tv2;

			if (clock_gettime(SAMPLED_CLOCK, &tv2)) 
			tv2.tv_sec = tv2.tv_nsec = -1;

			double  sec = static_cast<double>(tv2.tv_sec - tv.tv_sec);
			double nsec = static_cast<double>(tv2.tv_nsec - tv.tv_nsec);

			return (sec + 1.0e-9 * nsec);
		}

	#else

		#ifdef HAVE_GETTIMEOFDAY

			void MRTime::precistic() {	
				gettimeofday(&tv, 0L);
			}

			double MRTime::precistoc() {
				
				tictoc tv2;

				gettimeofday(&tv2, 0L);
				double  sec = static_cast<double>(tv2.tv_sec - tv.tv_sec);
				double usec = static_cast<double>(tv2.tv_usec - tv.tv_usec);

				return (sec + 1.0e-6 * usec);
			}

		#else
			// Fall back to C/C++ low resolution time function.

			void MRTime::precistic() {
				time(&tv);
			}

			double MRTime::precistoc() {
				tictoc tv2;
				time(&tv2);
				return difftime(tv2, tv);
			}
		#endif
	#endif
#else

	// Windows.
	//#pragma comment(lib,"Coredll.lib")

	void MRTime::precistic() {
		tv = GetTickCount();
	}

	double MRTime::precistoc() {
		tictoc tv2;
		tv2 = GetTickCount();
		return 1.0e-3 * (tv2 - tv);
	}

#endif
}
