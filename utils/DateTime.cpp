#include "DateTime.h"
using namespace lvki;

/**
 * Constructor - initialises the datetime to NOW
 * 
 * @param timezone Date/time's timezone
 */
DateTime::DateTime(string timezone)
{
	_TimeZone = timezone;
	time ( &_RawTime );
}

/**
 * Constructor - initialises the datetime to the time_t value
 *
 * @param datetime A time_t value representing a particular datetime.
 * @param timezone Date/time's timezone
 */
DateTime::DateTime(time_t datetime, string timezone)
{
	_TimeZone = timezone;
	SetDateTime(datetime);
}

/**
 * Constructor - initialises the datetime to the value represented by the provided string
 *
 * @param datetime A string representing a particular datetime in the format YYYY-mm-dd HH:MM:SS
 * @param timezone Date/time's timezone
 */
DateTime::DateTime(const string &datetime, string timezone)
{
	_TimeZone = timezone;
	//accepts the mysql standard datetime format: 
	SetDateTime(datetime, "%Y-%m-%d %H:%M:%S");
}

/**
 * Constructor - initialises the datetime to the value represented by the provided string.
 * Valid options for the format string are:
 *
 *  %a	Abbreviated weekday name *	Thu
 *  %A	Full weekday name * 	Thursday
 *  %b	Abbreviated month name *	Aug
 *  %B	Full month name *	August
 *  %c	Date and time representation *	Thu Aug 23 14:55:02 2001
 *  %d	Day of the month (01-31)	23
 *  %H	Hour in 24h format (00-23)	14
 *  %I	Hour in 12h format (01-12)	02
 *  %j	Day of the year (001-366)	235
 *  %m	Month as a decimal number (01-12)	08
 *  %M	Minute (00-59)	55
 *  %p	AM or PM designation	PM
 *  %S	Second (00-61)	02
 *  %U	Week number with the first Sunday as the first day of week one (00-53)	33
 *  %w	Weekday as a decimal number with Sunday as 0 (0-6)	4
 *  %W	Week number with the first Monday as the first day of week one (00-53)	34
 *  %x	Date representation *	08/23/01
 *  %X	Time representation *	14:55:02
 *  %y	Year, last two digits (00-99)	01
 *  %Y	Year	2001
 *  %Z	Timezone name or abbreviation	CDT
 *  %%	A % sign	%
 *
 * @param datetime A string representing a particular datetime.
 * @param format A string defining the format of the provided datetime string.
 * @param timezone Date/time's timezone
 */
DateTime::DateTime(const string &datetime, string format, string timezone)
{
	_TimeZone = timezone;	
	time ( &_RawTime );
	
	SetDateTime(datetime, format);	
}

/**
 * Constructor - initialises the datetime to the value represented by the provided values
 *
 * @param year Year value
 * @param month Month value
 * @param day Day value
 * @param hour Hour value
 * @param minute Minute value
 * @param second Second value
 * @param timezone Date/time's timezone
 */
DateTime::DateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second, string timezone)
{
	_TimeZone = timezone;

	//need to initialise _RawTime
	time ( &_RawTime );
	
	SetDateTime(year,month,day,hour,minute,second);
}

/**
 * Destructor
 */
DateTime::~DateTime()
{
	
}

/**
 * Static function which returns a DateTime instance initialised to NOW
 *
 * @param timezone Date/time's timezone 
 * @return A new DateTime instance.
 */
DateTime* DateTime::Now(string timezone)
{
	DateTime* dt = new DateTime(timezone);
	return dt;
}

/**
 * Static function which returns a string representation of NOW
 *
 * @param format The format of the datetime to be returned.
 * @param timezone Date/time's timezone
 * @return A new string representing the current datetime in the format specified
 */
string DateTime::NowString(string format, string timezone)
{
	DateTime dt = DateTime(timezone);
	string result = dt.ToString(format, timezone);	
	return result;
}

/**
 * Gets the day for the current datetime instance
 *
 * @param timezone Date/time's timezone
 * @return The day value for the current datetime instance.
 */
unsigned int DateTime::GetDay(string timezone)
{
	if (timezone != "")
		setenv("TZ", timezone.c_str(), 1);
	else
		setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
	
	return dt->tm_mday;
}

/**
 * Gets the month for the current datetime instance
 *
 * @param timezone Date/time's timezone
 * @return The month value for the current datetime instance.
 */
unsigned int DateTime::GetMonth(string timezone)
{
	if (timezone != "")
		setenv("TZ", timezone.c_str(), 1);
	else
		setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
	
	return (dt->tm_mon + 1);
}

/**
 * Gets the year for the current datetime instance
 *
 * @param timezone Date/time's timezone
 * @return The year value for the current datetime instance.
 */
unsigned int DateTime::GetYear(string timezone)
{
	if (timezone != "")
		setenv("TZ", timezone.c_str(), 1);
	else
		setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
	
	return (dt->tm_year + 1900);
}

/**
 * Gets the hour for the current datetime instance
 *
 * @param timezone Date/time's timezone
 * @return The hour value for the current datetime instance.
 */
unsigned int DateTime::GetHour(string timezone)
{
	if (timezone != "")
		setenv("TZ", timezone.c_str(), 1);
	else
		setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
	
	return dt->tm_hour;
}

/**
 * Gets the minute for the current datetime instance
 *
 * @param timezone Date/time's timezone
 * @return The minute value for the current datetime instance.
 */
unsigned int DateTime::GetMinute(string timezone)
{
	if (timezone != "")
		setenv("TZ", timezone.c_str(), 1);
	else
		setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
	
	return dt->tm_min;
}

/**
 * Gets the second for the current datetime instance
 *
 * @param timezone Date/time's timezone
 * @return The second value for the current datetime instance.
 */
unsigned int DateTime::GetSecond(string timezone)
{
	if (timezone != "")
		setenv("TZ", timezone.c_str(), 1);
	else
		setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
	
	return dt->tm_sec;
}

/**
 * Sets the day portion of the datetime to the value provided
 *
 * @param day Day value
 */
void DateTime::SetDay(unsigned int day)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	//record what the current date time values are which we need to preserve
	struct tm *prevDT = localtime(&_RawTime); 
	int year = prevDT->tm_year;
	int month = prevDT->tm_mon;
	int hour = prevDT->tm_hour;
	int minute = prevDT->tm_min;
	int second = prevDT->tm_sec;	
	
	//build the date time
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime); 
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;	

	//make it
	_RawTime = mktime ( dt );
	
	//build it again, setting the dst flag correctly
	dt = localtime(&_RawTime); 

	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;	
	
	_RawTime = mktime ( dt );	
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Sets the month portion of the datetime to the value provided
 *
 * @param month Month value
 */
void DateTime::SetMonth(unsigned int month)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	//record what the current date time values are which we need to preserve
	struct tm *prevDT = localtime(&_RawTime); 
	int year = prevDT->tm_year;
	int day = prevDT->tm_mday;
	int hour = prevDT->tm_hour;
	int minute = prevDT->tm_min;
	int second = prevDT->tm_sec;	
	
	//build the date time
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime); 
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = (month-1);
	dt->tm_year = year;		

	//make it
	_RawTime = mktime ( dt );
	
	//build it again, setting the dst flag correctly
	dt = localtime(&_RawTime); 	
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = (month-1);
	dt->tm_year = year;
	
	_RawTime = mktime ( dt );	
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Sets the year portion of the datetime to the value provided
 *
 * @param year Year value
 */
void DateTime::SetYear(unsigned int year)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	//record what the current date time values are which we need to preserve
	struct tm *prevDT = localtime(&_RawTime); 
	int month = prevDT->tm_mon;
	int day = prevDT->tm_mday;
	int hour = prevDT->tm_hour;
	int minute = prevDT->tm_min;
	int second = prevDT->tm_sec;	
	
	//build the date time
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime); 
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = (year-1900);		

	//make it
	_RawTime = mktime ( dt );
	
	//build it again, setting the dst flag correctly
	dt = localtime(&_RawTime); 	
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = (year-1900);
	
	_RawTime = mktime ( dt );	
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Sets the hour portion of the datetime to the value provided
 *
 * @param hour Hour value
 */
void DateTime::SetHour(unsigned int hour)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	//record what the current date time values are which we need to preserve
	struct tm *prevDT = localtime(&_RawTime); 
	int year = prevDT->tm_year;
	int month = prevDT->tm_mon;
	int day = prevDT->tm_mday;
	int minute = prevDT->tm_min;
	int second = prevDT->tm_sec;	
	
	//build the date time
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime); 
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;	

	//make it
	_RawTime = mktime ( dt );
	
	//build it again, setting the dst flag correctly
	dt = localtime(&_RawTime); 	
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;	
	
	_RawTime = mktime ( dt );	
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Sets the minute portion of the datetime to the value provided
 *
 * @param minute Minute value
 */
void DateTime::SetMinute(unsigned int minute)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();

	//record what the current date time values are which we need to preserve
	struct tm *prevDT = localtime(&_RawTime); 
	int year = prevDT->tm_year;
	int month = prevDT->tm_mon;
	int day = prevDT->tm_mday;
	int hour = prevDT->tm_hour;
	int second = prevDT->tm_sec;
	
	//build the date time
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime);
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;

	//make it
	_RawTime = mktime ( dt );
	
	//build it again, setting the dst flag correctly
	dt = localtime(&_RawTime); 	
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;
			
	_RawTime = mktime ( dt );	
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Sets the second portion of the datetime to the value provided
 *
 * @param second Second value
 */
void DateTime::SetSecond(unsigned int second)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	//record what the current date time values are which we need to preserve
	struct tm *prevDT = localtime(&_RawTime); 
	int year = prevDT->tm_year;
	int month = prevDT->tm_mon;
	int day = prevDT->tm_mday;
	int hour = prevDT->tm_hour;
	int minute = prevDT->tm_min;
	
	//build the date time
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime); 
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;

	//make it
	_RawTime = mktime ( dt );
	
	//build it again, setting the dst flag correctly
	dt = localtime(&_RawTime); 	
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;
	
	_RawTime = mktime ( dt );	
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Sets the datetime to the time_t value
 *
 * @param datetime A time_t value representing a particular datetime.
 */
void DateTime::SetDateTime(time_t datetime)
{	
	_RawTime = datetime;
}

/**
 * Sets the datetime to the value represented by the provided string
 *
 * @param datetime A string representing a particular datetime.
 * @param format A string defining the format of the provided datetime string.
 */
void DateTime::SetDateTime(const string &datetime, string format)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime); //need to use localtime instead of gmtime so the tm struct is initialised correctly for the locale of the server it's running on
	strptime(datetime.c_str(), format.c_str(), dt);
	
	//remember the original time values
	int year = dt->tm_year;
	int month = dt->tm_mon;
	int day = dt->tm_mday;
	int hour = dt->tm_hour;
	int min = dt->tm_min;
	int sec = dt->tm_sec;
		
	//this will set the dst flag correctly but may change the hour
	_RawTime = mktime ( dt );
	
	//reset the dt values to the original values
	dt->tm_sec = sec;
	dt->tm_min = min;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = month;
	dt->tm_year = year;
	
	//make the time which now has the correct original time values AND the correct dst flag
	_RawTime = mktime ( dt );
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Sets the datetime to the value represented by the provided values
 *
 * @param year Year value
 * @param month Month value
 * @param day Day value
 * @param hour Hour value
 * @param minute Minute value
 * @param second Second value
 */
void DateTime::SetDateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	//build the date time
	time(&_RawTime);
	struct tm *dt = localtime(&_RawTime); 
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = (month-1);
	dt->tm_year = (year-1900);

	//make it
	_RawTime = mktime ( dt );
	
	//build it again, setting the dst flag correctly
	dt = localtime(&_RawTime); 	
	
	dt->tm_sec = second;
	dt->tm_min = minute;
	dt->tm_hour = hour;
	dt->tm_mday = day;
	dt->tm_mon = (month-1);
	dt->tm_year = (year-1900);		
	
	_RawTime = mktime ( dt );	
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Gets the time_t value for the current datetime instance.
 *
 * @return The current time_t value for this datetime.
 */
time_t DateTime::ToAnsiTime() const
{
	return _RawTime;
}

/**
 * Returns a string representation of the current datetime instance
 *
 * @param format The format of the datetime to be returned.
 * @param timezone Date/time's timezone
 * @return A new string representing the current datetime in the format specified
 */
string DateTime::ToString(string format, string timezone)
{
	if (timezone != "")
		setenv("TZ", timezone.c_str(), 1);
	else
		setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
   
	struct tm *dt = localtime(&_RawTime);
	
	char* result = new char[format.length() + 100];
	strftime ( result, format.length() + 100, format.c_str(), dt );
	string dtstr = result;
	delete [] result;
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
	return dtstr;
}


/**
 * Adds the specified number of seconds to the current datetime intsance.
 *
 * @param seconds The number of seconds to add.
 */
void DateTime::AddSeconds(int seconds)
{
	struct tm *dt = localtime(&_RawTime);
	
	dt->tm_sec += seconds;
	_RawTime = mktime ( dt );
}

/**
 * Adds the specified number of minutes to the current datetime intsance.
 *
 * @param minutes The number of minutes to add.
 */
void DateTime::AddMinutes(int minutes)
{
	struct tm *dt = localtime(&_RawTime);
	
	dt->tm_min += minutes;
	_RawTime = mktime ( dt );
}

/**
 * Adds the specified number of hours to the current datetime intsance.
 *
 * @param hours The number of hours to add.
 */
void DateTime::AddHours(int hours)
{
	struct tm *dt = localtime(&_RawTime);
	
	dt->tm_hour += hours;
	_RawTime = mktime ( dt );
}

/**
 * Adds the specified number of days to the current datetime intsance.
 *
 * @param days The number of days to add.
 */
void DateTime::AddDays(int days)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	dt->tm_mday += days;
	//Ticket 857: determine DST
	dt->tm_isdst = -1;
	_RawTime = mktime ( dt );
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Adds the specified number of months to the current datetime intsance.
 *
 * @param months The number of months to add.
 */
void DateTime::AddMonths(int months)
{
	setenv("TZ", _TimeZone.c_str(), 1);
	tzset();
	
	struct tm *dt = localtime(&_RawTime);
	
	dt->tm_mon += months;
	//Ticket 857: determine DST
	dt->tm_isdst = -1;
	_RawTime = mktime ( dt );
	
	//ensure the TZ setting doesn't screw up other calls to localtime
	unsetenv("TZ");
	tzset();
}

/**
 * Adds the specified number of years to the current datetime intsance.
 *
 * @param years The number of years to add.
 */
void DateTime::AddYears(int years)
{
	struct tm *dt = localtime(&_RawTime);
	
	dt->tm_year += years;
	_RawTime = mktime ( dt );
}

/**
 * Gets the number of seconds difference between the current datetime and the one provided.
 *
 * @param datetime Another DateTime instance to compare to.
 * @return A long value indicating the absolute number of seconds difference between this datetime and the one provided (always positive)
 */
long DateTime::DiffSeconds(DateTime *datetime)
{
	long diff = (long)_RawTime - (long)datetime->ToAnsiTime();
	if (diff < 0)
		diff = diff*-1;
	
	return diff;
}

/**
 * Gets the number of days difference between the current datetime and the one provided.
 *
 * @param datetime Another DateTime instance to compare to.
 * @return A double value indicating the number of days difference between this datetime and the one provided (always positive)
 */
double DateTime::DiffDays(DateTime *datetime)
{
	double diff = DiffSeconds(datetime) / 60.00 / 60.00 / 24.00;
	return diff;
}

