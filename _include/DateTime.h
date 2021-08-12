#ifndef DateTime_H
#define DateTime_H

#include <ctime>
#include <cstdlib>
#include <time.h> //required for strptime

#include <iostream>
#include <string>

using namespace std;

namespace lvki
{		
		/**
		 * A utility class for dealing with DateTime details
		 */
		class DateTime
		{
			public:
				DateTime(string timezone);
				DateTime(time_t datetime, string timezone);
				DateTime(const string &datetime, string timezone);
				DateTime(const string &datetime, string format, string timezone);
				DateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second, string timezone);
				~DateTime();
				
				static DateTime* Now(string timezone);
				static string NowString(string format, string timezone);

				unsigned int GetDay(string timezone = "");
				unsigned int GetMonth(string timezone = "");
				unsigned int GetYear(string timezone = "");
				unsigned int GetHour(string timezone = "");
				unsigned int GetMinute(string timezone = "");
				unsigned int GetSecond(string timezone = "");
				
				void SetDay(unsigned int day);
				void SetMonth(unsigned int month);
				void SetYear(unsigned int year);
				void SetHour(unsigned int hour);
				void SetMinute(unsigned int minute);
				void SetSecond(unsigned int second);
				
				void SetDateTime(time_t datetime);
				void SetDateTime(const string &datetime, string format);
				void SetDateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
				
				string ToString(string format, string timezone = "");
				
				time_t ToAnsiTime() const;
				
				void AddSeconds(int seconds);
				void AddMinutes(int minutes);
				void AddHours(int hours);
				void AddDays(int days);
				void AddMonths(int months);
				void AddYears(int years);
				
				long DiffSeconds(DateTime *datetime);
				double DiffDays(DateTime *datetime);

			private:
				time_t _RawTime;

				string _TimeZone;
				
		};
	
}

#endif
