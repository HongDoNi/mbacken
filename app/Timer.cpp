#include "Timer.h"
using namespace b2be::utils;

/**
 * Constructor
 */
Timer::Timer()
{
	_Running = false;	
	_ElapsedSec = 0;
	_ElapsedMSec = 0;
	_ElapsedUSec = 0;
}

/**
 * Destructor
 */
Timer::~Timer()
{
	
}

/**
 * Starts the current timer.
 */
void Timer::Start()
{
	_Running = true;
	_ElapsedSec = 0;
	_ElapsedMSec = 0;
	_ElapsedUSec = 0;
	gettimeofday(&startTime, NULL);	
}

/**
 * Stops the current timer.
 */
void Timer::Stop()
{
	gettimeofday(&stopTime, NULL);
	_Running = false;
}

/**
 * Gets the current elapsed seconds for the timer, whether it is still running or not.
 * If the timer has been stopped it will return how long it was running before it was stopped.
 * If the timer is still running the it will return the current elapsed seconds.
 *
 * @return Double containing the elapsed number of seconds for the current timer.
 */
double Timer::GetElapsedSeconds()
{
	long ms = GetElapsedMilliseconds();
	
	return (ms / 1000.00);
}

/**
 * Gets the current elapsed milli-seconds for the timer, whether it is still running or not.
 * If the timer has been stopped it will return how long it was running before it was stopped.
 * If the timer is still running the it will return the current elapsed milli-seconds.
 *
 * @return Long containing the elapsed number of milli-seconds for the current timer.
 */
long Timer::GetElapsedMilliseconds()
{
	if (false == _Running)
	{
		_ElapsedSec  = stopTime.tv_sec  - startTime.tv_sec;
		_ElapsedUSec = stopTime.tv_usec - startTime.tv_usec;

		_ElapsedMSec = ((_ElapsedSec) * 1000 + _ElapsedUSec/1000);
	}
	else
	{
		// if the timer is still running, just take a temporary time reading
		struct timeval nowTime;
		gettimeofday(&nowTime, NULL);
		
		_ElapsedSec  = nowTime.tv_sec  - startTime.tv_sec;
		_ElapsedUSec = nowTime.tv_usec - startTime.tv_usec;

		_ElapsedMSec = ((_ElapsedSec) * 1000 + _ElapsedUSec/1000);	
	}
	
	return _ElapsedMSec;
}
