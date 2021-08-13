#ifndef Timer_H
#define Timer_H

#include <ctime>
#include <sys/time.h>

using namespace std;

namespace b2be
{
	namespace utils
	{		
		/**
		 * A simple Timer class for measuring time differences
		 */
		class Timer
		{
			public:
				Timer();
				~Timer();
				
				void Start();
				void Stop();
				
				double GetElapsedSeconds();
				long GetElapsedMilliseconds();
				
			private:
				struct timeval startTime;
				struct timeval stopTime;
				
				bool _Running;
				
				long _ElapsedSec;
				long _ElapsedMSec;
				long _ElapsedUSec;
		};
	}
}

#endif
