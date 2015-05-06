// Win32++   Version 8.0 Alpha
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// Acknowledgement:
//
// The original author of CTime and CTimeSpan is:
//
//      Robert C. Tausworthe
//      2812 Rancho Costero
//      Carlsbad, CA 92009
//
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//
//	Declaration of the CTime class
//
////////////////////////////////////////////////////////

#ifndef _WIN32XX_CTIME_H_
#define _WIN32XX_CTIME_H_

#include "wxx_wincore.h"
#include "wxx_archive.h"
#include <errno.h>
#include <time.h>


namespace Win32xx
{

	////////////////////////////////////////////////////////
	//
	//	Local types, constants, etc.

	// define an alias for struct tm
	typedef struct tm time_tm;

	// define the CTimeSpan data type
	typedef long timespan_t;

	// standard/daylight type, to avoid ambiguity in constructor declarations
	enum dst_t {decide = -1, STD, DST};

	// standard time display format (see strftime for %x meanings)
	const CString stdTForm   = TEXT("%d-%b-%Y [%j] (%a) %H:%M:%S %z");
	const CString stdTDate   = TEXT("%d-%b-%Y");
	const CString stdTHMS    = TEXT("%I:%M:%S %p");

	// forward declaration
	class CTimeSpan;

	// declaration of a non-CTime-class (i.e., global) time conversion utility
	time_t UTCtime(time_tm *atm);


	//============================================================================
	class CTime
	//	The CTime class is based internally on the time_t data type, which is
	//	measured in seconds past the January 1, 00:00:00 1970 UTC epoch. Thus,
	//	any given time_t instant is strictly a UTC epoch, which may then be
	//	rendered, using standard C library functions, as either a UTC or local
	//	time.  Local instants may further appear as standard or daylight times.
	//	Thus, the base epoch on a system in the PST zone will be displayed as
	//	December 31, 1969 16:00:00. Any attempt to construct or evaluate a date
	//	before this epoch will throw an exception or be in error. Even though
	//	the time_t type is implemented (in MinGW, anyway) as a signed integer
	//	type, negative values are not allowed.

	//	On systems where time_t is defined as a 32-bit integer, there is an
	//	upper date limit of January 18, 19:14:07, 2038. On 64-bit systems,
	//	there is no such upper date limit.

	//	Windows also has other time types that also interface with the CTime
	//	type. These are FILETIME, SYSTEMTIME, and MS-DOS date and time, each
	//	of which are described in the MSDN API documentation. Basically:

	//	FILETIME is a structure containing a 64-bit value representing the
	//	number of 100-nanosecond intervals since January 1, 1601 UTC.

	//	SYSTEMTIME is a structure representing a date and  time using individual
	//	values for the month, day, year, weekday, hour, minute, second, and
	//	millisecond. This is not useful in itself, but must be translated into
	//	a FILETIME or time_t.

	//	MS-DOS date and time are two 16-bit words packed with day, month, year
	//  and second, minute, hour fields.  The format may be found in the MSDN
	//	Library article on DosDateTimeToFileTime().
	{
	public:

		// Constructors
		CTime();
		CTime(const CTime&);
		CTime(time_t);
		CTime(time_tm&);
		CTime(UINT, UINT, UINT, UINT, UINT, UINT, UINT,
			dst_t eDST = decide);
		CTime(UINT, UINT, UINT, UINT, UINT, UINT, dst_t eDST = decide);
		CTime(UINT, UINT, UINT, UINT, UINT, dst_t eDST = decide);
		CTime(WORD, WORD, dst_t eDST = decide);
		CTime(const SYSTEMTIME&, dst_t eDST = decide);
		CTime(const FILETIME&,  dst_t eDST = decide);
		CTime(const CString&, dst_t eDST = decide);

		// Method members
		bool 	   	GetAsFileTime(FILETIME&) const;
		bool 	   	GetAsSystemTime(SYSTEMTIME&) const;
		int		GetDay(bool local = true) const;
		int		GetDayOfWeek(bool local = true) const;
		int		GetDayOfYear(bool local = true) const;
		time_tm* 	GetGmtTm(time_tm* ptm = NULL) const;
		int		GetHour(bool local = true) const;
		time_tm* 	GetLocalTm(time_tm* ptm = NULL) const;
		int		GetMinute(bool local = true) const;
		int		GetMonth(bool local = true) const;
		int		GetSecond(bool local = true) const;
		int		GetYear(bool local = true) const;
		time_t		GetTime() const;

		// Assignment operators
		const CTime& 	operator=(const CTime& timeSrc);
		const CTime& 	operator=(time_t t);

		// Computational operators
		CTimeSpan 	operator-(CTime) const;
		CTime 	  	operator-(CTimeSpan) const;
		CTime 	  	operator+(CTimeSpan) const;
		const CTime& 	operator+=(CTimeSpan);
		const CTime& 	operator-=(CTimeSpan);
		bool 		operator==(CTime) const;
		bool 		operator!=(CTime) const;
		bool 		operator<(CTime) const;
		bool 		operator>(CTime) const;
		bool 		operator<=(CTime) const;
		bool 		operator>=(CTime) const;

		// CString conversion
		CString 	Format(const CString &) const;
		CString 	Format(LPCTSTR) const;
		CString 	Format(UINT) const;
		CString 	StdFormat(void) const;

		CString 	FormatGmt(const CString &) const;
		CString 	FormatGmt(LPCTSTR) const;
		CString 	FormatGmt(UINT) const;

		// Static methods
		static	CTime 	GetCurrentTime();
		static	FILETIME FileTimePlus(const FILETIME &, double );

		// global friends
		friend  CArchive& operator<<(CArchive&, CTime&);
		friend  CArchive& operator>>(CArchive&, CTime&);

	protected:
		time_t 		m_time;
	};

	//============================================================================
	class CTimeSpan
	//	The CTimeSpan class defines the data type for differences between two
	//	CTime values, measured in seconds of time. It also defines procedures
	//	for combining that data type with that of CTime to produce CTime
	//	elements that differ by a specified span of time and  methods to
	//	extract and  display CTimeSpan values.
	{
	public:
		// Constructors
		CTimeSpan();
		CTimeSpan(timespan_t);
		CTimeSpan(long, int, int, int);
		CTimeSpan(const CTimeSpan&);

		// Methods to extract items
		long		GetDays() const;
		long		GetTotalHours() const;
		int 		GetHours() const;
		long 		GetTotalMinutes() const;
		int 		GetMinutes() const;
		long 		GetTotalSeconds() const;
		int 		GetSeconds() const;

		// assignment operators
		const CTimeSpan& operator=(const CTimeSpan&);
		const CTimeSpan& operator=(timespan_t);

		// computational operators
		CTimeSpan       operator-() const;
		CTimeSpan 	operator-(CTimeSpan) const;
		CTimeSpan 	operator+(CTimeSpan) const;
		const CTimeSpan& operator+=(CTimeSpan);
		const CTimeSpan& operator-=(CTimeSpan);
		bool 	  	operator==(CTimeSpan) const;
		bool 	  	operator!=(CTimeSpan) const;
		bool 	  	operator<(CTimeSpan) const;
		bool 	  	operator>(CTimeSpan) const;
		bool 	  	operator<=(CTimeSpan) const;
		bool 	  	operator>=(CTimeSpan) const;

		// CString conversion
		CString   	Format(LPCTSTR) const;
		CString   	Format(const CString &) const;
		CString   	Format(UINT nID) const;

		// Global friends
		friend  CArchive& operator<<(CArchive&, CTimeSpan);
		friend  CArchive& operator>>(CArchive&, CTimeSpan);

	private:
		friend class CTime;

		timespan_t m_timespan;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	////////////////////////////////////////////////////////////////
	//
	//	Implementation of the CTime class
	//
	////////////////////////////////////////////////////////////////

	//	Local definitions, constants, and  defaults
	//  for string formatting
	static const size_t  maxTimeBufferSize = 128;

	static const CString Month[] =   {TEXT("January"), TEXT("February"),
					  TEXT("March"), TEXT("April"), TEXT("May"),
					  TEXT("June"), TEXT("July"), TEXT("August"),
					  TEXT("September"), TEXT("October"),
					  TEXT("November"), TEXT("December")};
	static const CString AbMonth[] = {TEXT("Jan"), TEXT("Feb"), TEXT("Mar"),
					  TEXT("Apr"), TEXT("May"), TEXT("Jun"),
					  TEXT("Jul"), TEXT("Aug"), TEXT("Sep"),
					  TEXT("Oct"), TEXT("Nov"), TEXT("Dec")};

	static const int sec_per_day   = 86400;
	static const int sec_per_hour  = 3600;
	static const int sec_per_min   = 60;
	static const int hours_per_day = 24;

	//============================================================================
	inline time_t UTCtime(time_tm *atm)
	//	Return the time_t t corresponding to the date given in atm as a UTC
	//	time. That is, gmtime(t) == atm.  This is equal to the local time_t
	//	of the atm plus the current time zone bias.
	{
		// compute the local time from atm
		time_t t0 = ::mktime(atm);     // atm = *localtime(t0)
		time_tm atm0 = *::gmtime(&t0); // atm0 = UTC time of atm
		time_t t1 = ::mktime(&atm0);   // atm0 = *localtime(t1)
		timespan_t zt = (timespan_t)(t0 - t1);  // time zone bias
		return t0 + zt;
	}

	//============================================================================
	inline CTime::CTime()
	//	Construct an CTime object initialized to the Jan 1, 1970 00:00:00 epoch.
	//	This constructor allows defining CTime object arrays, which may then
	//	be initialized prior to use.
	{
		m_time = 0;
	}

	//============================================================================
	inline CTime::CTime(const CTime &t)
	//	Construct a CTime object from another CTime object t.
	{
		m_time = t.m_time;
	}

	//============================================================================
	inline CTime::CTime(time_t t)
	//	Construct a CTime object from the time_t value t.
	{
		m_time = t;
	}

	//============================================================================
	inline CTime::CTime(time_tm &atm)
	//	Construct a CTime object from the time_tm atm.
	{
		// compute the object time_t
		m_time = ::mktime(&atm);

		// check for acceptable range
		assert(m_time != -1);
	}

	//============================================================================
	inline CTime::CTime(UINT yr, UINT mo, UINT wkday, UINT nthwk, UINT hr,
	    UINT min, UINT sec, dst_t nDST /* = -1 */)
	//	Construct a CTime of the nthwk occurrence of the given wkday (0..6)
	//	in the mo month of yr year, at hr:min:sec of that day, local time.
	//	Restrictions on yr, mo, hr, min, and  sec are the same as cited in the
	//	constructor CTime(yr, mo, da, hr, min, sec, nDST). Otherwise,
	//		wkday   0-6
	//		nthwk   no constraint
	{
		// validate parameters w.r.t. ranges
		assert(yr >= 1970);
		assert(wkday <= 6);
		assert(1 <= mo && mo <= 12);

		// This computation is tricky because adding whole days to a time_t
		// may result in date within the DST zone, which, when rendered into
		// calendar date form, appears off by the daylight bias. Rather, we
		// need to work in UTC calendar days and  add integer calendar days to
		// the first-of-month epoch in the given year to yield the desired
		// date.  To start, compute the first of the month in the given year
		// at the given hour, minute, and  second.
		time_tm atm = {(int)sec, (int)min, (int)hr, (int)1, (int)(mo - 1),
			(int)(yr - 1900), (int)0, (int)0, nDST};

		// get the local time of the UTC time corresponding to this
		time_t t1st = UTCtime(&atm);

		// recover the day of the week
		atm = *::gmtime(&t1st);

		// Compute number of days until the nthwk occurrence of wkday
		int nthwkday = (7 + wkday - atm.tm_wday) % 7 + (nthwk - 1) * 7;

		// add this to the first of the month
		time_t tnthwkdy = t1st + nthwkday * sec_per_day;
		atm = *::gmtime(&tnthwkdy);
		atm.tm_isdst = nDST;
		CTime t0(atm);
		m_time = t0.m_time;
	}

	//============================================================================
	inline CTime::CTime(UINT year, UINT month, UINT day, UINT hour, UINT min,
		UINT sec, dst_t nDST)
	//	Construct a CTime object from local time elements. Each element is
	//	constrained to lie within the following ranges:
	//		year 		1970�2038 (on 32-bit systems)
	//		month 		1�12
	//		day 		1�31
	//		hour, min, sec	no constraint

	//	The nDST value indicates whether daylight savings time is in effect.
	//	It can have one of three values, as follows:
	//              nDST == 0   Standard time is in effect.
	//              nDST >  0   Daylight savings time is in effect.
	//              nDST <  0   The default. Automatically computes whether
	//			    standard time or daylight time is in effect.
	{
		 // validate parameters w.r.t. ranges
		assert(1 <= day && day   <= 31);
		assert(1 <= month && month <= 12);
		assert(year  >= 1970);

		// fill out a time_tm with the calendar date
		time_tm atm = {(int)sec, (int)min, (int)hour, (int)day,
			(int)(month - 1), (int)(year - 1900), (int)0, (int)0, nDST};

		// compute the object time_t
		CTime t(atm);
		m_time = t.m_time;
	}

	//============================================================================
	inline CTime::CTime(UINT yr, UINT doy, UINT hr, UINT min, UINT sec,
		dst_t nDST /* = -1 */)
	//	Construct a CTime using the day-of-year doy, where doy = 1 is
	//	January 1 in the specified year.  Restrictions on yr, hr, min, and  sec
	//	are the same as in CTime(yr, mo, da, hr, min, sec, nDST). There is no
	//	constraint on doy.
	{
		 // fill out a time_tm with the calendar date for Jan 1, yr, hr:min:sec
		time_tm atm1st = {(int)sec, (int)min, (int)hr, (int)1,
			(int)0, (int)(yr - 1900), (int)0, (int)0, STD};

		// get the local time of the UTC time corresponding to this
		time_t Jan1 = UTCtime(&atm1st);
		time_t tDoy = Jan1 + (doy - 1) * sec_per_day;
		time_tm atm = *::gmtime(&tDoy);
		atm.tm_isdst = nDST;
		CTime t0(atm);
		m_time = t0.m_time;
	}

	//============================================================================
	inline CTime::CTime(WORD wDosDate, WORD wDosTime, dst_t nDST)
	//	Construct a CTime object from the MS-DOS wDosDate and  wDosTime values.
	//	These are formats used by MS-DOS. The date is a packed 16-bit value
	//	in which bits in the value represent the day, month, and  year. The
	//	time is a packed 16-bit value in which bits in the value represent
	//	the hour, minute, and  second.
	{
		FILETIME ft;
		VERIFY( ::DosDateTimeToFileTime(wDosDate, wDosTime, &ft) );
		CTime t(ft, nDST);
		m_time = t.m_time;
	}

	//============================================================================
	inline CTime::CTime(const SYSTEMTIME& st, dst_t nDST)
	//	 Construct a CTime object from a SYSTEMTIME structure st.
	{
		assert(st.wYear >= 1970);
		CTime t((UINT)st.wYear, (UINT)st.wMonth, (UINT)st.wDay, (UINT)st.wHour,
			(UINT)st.wMinute, (UINT)st.wSecond, nDST);

		m_time = t.m_time;
	}

	//============================================================================
	inline CTime::CTime(const FILETIME& ft, dst_t nDST)
	//	Construct a CTime object from a (UTC) FILETIME structure ft.
	{
		// start by converting ft (a UTC time) to local time
		FILETIME localTime;
		VERIFY( ::FileTimeToLocalFileTime(&ft, &localTime) );

		//  convert localTime to a SYSTEMTIME structure
		SYSTEMTIME st;
		VERIFY( ::FileTimeToSystemTime(&localTime, &st) );

		// then convert the system time to a CTime
		CTime t(st, nDST);
		m_time = t.m_time;
	}

	//============================================================================
	inline CTime::CTime(const CString &timestr,  dst_t nDST /* = -1 */)
	//	Construct a CTime as directed by the formatting CString timestr, whose
	//	specifications appear below. Any nonconformity between timestr  and
	//	these expected format standards will result in throwing an exception.
	//	Acceptable formats are:

	//		"yyyy/mo/da H:M:S"
	//		"da-Mon-yyyy H:M:S"
	//		"Month da, yyyy H:M:S"
	//		"yyyy+doy H:M:S"

	//	The year, month, day, hour, minute, and  second values must be consistent
	//	with the requirements given in the description of the constructor of
	//	CTime(year, month, day, hour, minute, second).  The date and  time
	//	specifications are presumed to apply to the local zone. If H:M:S is not
	//	present, then midnight is presumed, and  if just S is absent, zero
	//	seconds is the default. The first form is assumed when the given date
	//	string contains "/"; the second, when "-" appears; the third, when ","
	//	is present; and  the fourth, when "+" is found.
	{
		int  p1, p2, p3; // position indexes into timestr
		int	len = timestr.GetLength();
		int yyyy;   // year, 4 digits
		int	mo;   	// month 1 - 12
		int	da;   	// day 1-31
		int	doy;	// day of year 1 - 366
		int	H; 	// hour of day 0 - 23
		int	M;	// minute of hour 0 - 59
		int	S;	// seconds of minute 0 - 61 (leap years)

		// find  H:M:S values
		if ((p1 = MIN(timestr.Find(TEXT(":")), len)) >= 0)
		{	  // the time of day is present
			p2 = timestr.ReverseFind(TEXT(":"));
			if (p1 == p2) // H:M only
			{
				p2 = MAX(timestr.ReverseFind(TEXT(" "), p1), 0);
				p3 = MAX(timestr.Find(TEXT(" "), p1), (int)len);
				H = _ttoi(timestr.Mid(p2 + 1, p1 - p2).c_str());
				M = _ttoi(timestr.Mid(p1 + 1, p3 - p1).c_str());
				S = 0;
			}
			else // H:M:S
			{
				p3 = MAX(timestr.ReverseFind(TEXT(" "), p1), (int)0);
				H = _ttoi(timestr.Mid(p3, p1 - p3).c_str());
				M = _ttoi(timestr.Mid(p1 + 1, p2 - p1).c_str());
				p3 = MAX(timestr.Find(TEXT(" "), p1), (int)len);
				S = _ttoi(timestr.Mid(p2 + 1, p3 - p2).c_str());
			}

		}
		else // no ":" present
			H = M = S = 0;

		// now handle the year, month and  day formats
		if ((p1 = timestr.Find(TEXT("/"))) >= 0) // "yyyy/mo/da H:M:S"
		{
			if ((p2 = timestr.Find(TEXT("/"), p1 + 1)) > len)
			{
				errno = EINVAL;
				return;
			}

			p3   = MIN(timestr.Find(TEXT(" "), p2), (int)len);
			yyyy = _ttoi(timestr.Mid(0, p1).c_str());
			mo   = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
			da   = _ttoi(timestr.Mid(p2 + 1, p3 - p2 - 1).c_str());
			CTime t(yyyy, mo, da, H, M, S, nDST);
			m_time = t.m_time;
		}
		else if ((p1 = timestr.Find(TEXT("-"))) >= 0)  // "da-Mon-yyyy H:M:D"
		{
			if ((p2 = timestr.Find(TEXT("-"), p1 + 1)) > len)
			{
				errno = EINVAL;
				return;
			}

			p3   = MIN(timestr.Find(TEXT(" "), p2), (int)len);
			da   = _ttoi(timestr.Mid(0, p1).c_str());
			CString mon  = timestr.Mid(p1 + 1, p2 - p1 - 1);
			yyyy = _ttoi(timestr.Mid(p2 + 1, p3 - p2).c_str());
			for (mo = 0; mo < 12; mo++)
				if (mon == AbMonth[mo])
					break;
			if (mo == 12)
			{
				errno = EINVAL;
				return;
			}
			mo++;
			CTime t(yyyy, mo, da, H, M, S, nDST);
			m_time = t.m_time;
		}
		else if ((p2 = timestr.Find(TEXT(", "))) >= 0)  // "Month da, yyyy H:M:S"
		{
			if ((p1 = timestr.Find(TEXT(" "))) > p2)
			{
				errno = EINVAL;
				return;
			}

			p3   = MIN(timestr.Find(TEXT(" "), p2 + 2), (int)len);
			CString month = timestr.Mid(0, p1);
			da   = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
			yyyy = _ttoi(timestr.Mid(p2 + 1, p3 - p2 - 1).c_str());
			for (mo = 0; mo < 12; mo++)
				if (month == Month[mo])
					break;
			if (mo == 12)
			{
				errno = EINVAL;
				return;
			}
			mo++;
			CTime t(yyyy, mo, da, H, M, S, nDST);
			m_time = t.m_time;
		}
		else if ((p1 = timestr.Find(TEXT("+"))) >= 0)  // "yyyy+doy H:M:S"
		{
			p2 = MIN(timestr.Find(TEXT(" ")), (int)len);
			yyyy = _ttoi(timestr.Mid(0, p1).c_str());
			doy  = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
			CTime t(yyyy, doy, H, M, S, nDST);
			m_time = t.m_time;
		}
	}

	//============================================================================
	inline bool CTime::GetAsFileTime(FILETIME& ft) const
	//	Convert *this CTime object into a FILETIME structure and  store it
	//	in st. The FILETIME data structure initialized by this function will
	//	reflect the UTC time of this object. Return true if successful;
	//	otherwise throw an exception.
	{
		time_tm* ptm = GetGmtTm();
		assert(ptm != NULL);
		SYSTEMTIME st = {(WORD)(1900 + ptm->tm_year), (WORD)(1 + ptm->tm_mon),
			(WORD)ptm->tm_wday, (WORD)ptm->tm_mday, (WORD)ptm->tm_hour,
			(WORD)ptm->tm_min, (WORD)ptm->tm_sec, (WORD)0};
		SystemTimeToFileTime(&st, &ft);
		return true;
	}

	//============================================================================
	inline bool CTime::GetAsSystemTime(SYSTEMTIME& st) const
	//	Convert *this CTime object into a SYSTEMTIME structure and  store it
	//	in st. The SYSTEMTIME data structure initialized by this function will
	//	reflect the local time of this object and  will have its wMilliseconds
	//	member set to zero. Return true if successful; otherwise throw an
	//	exception.
	{
		time_tm* ptm = GetLocalTm();
		assert(ptm != NULL);
		st.wYear 	= (WORD) (1900 + ptm->tm_year);
		st.wMonth 	= (WORD) (1 + ptm->tm_mon);
		st.wDayOfWeek 	= (WORD) ptm->tm_wday;
		st.wDay 	= (WORD) ptm->tm_mday;
		st.wHour 	= (WORD) ptm->tm_hour;
		st.wMinute 	= (WORD) ptm->tm_min;
		st.wSecond 	= (WORD) ptm->tm_sec;
		st.wMilliseconds = 0;
		return true;
	}

	//============================================================================
	inline time_tm* CTime::GetGmtTm(time_tm* ptm) const
	//	Return a pointer to a time_tm that contains a decomposition of *this
	//	CTime object expressed in UTC. If ptm is non NULL, this decomposition
	//	is also copyed into ptm.
	{
		if (ptm != NULL)
		{
			*ptm = *::gmtime(&m_time);
			return ptm;
		}
		else
			return ::gmtime(&m_time);
	}

	//============================================================================
	inline time_tm* CTime::GetLocalTm(time_tm* ptm) const
	//	Return a pointer to a time_tm that contains a decomposition of *this
	//	CTime object expressed in the local time base. If ptm is non NULL, this
	//	decomposition is also copyed into ptm.
	{
		if (ptm != NULL)
		{
			time_tm* ptmTemp = ::localtime(&m_time);
			if (ptmTemp == NULL)
				return NULL;    // the m_time was not initialized!

			*ptm = *ptmTemp;
			return ptm;
		}
		else
			return ::localtime(&m_time);
	}

	//============================================================================
	inline time_t  CTime::GetTime() const
	//	Return *this time as a time_t value.
	{
		return m_time;
	}

	//============================================================================
	inline int	CTime::GetYear(bool local /* = true */) const
	//	Return the year of *this time object, local (true) or UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return 1900 + tmbuffer->tm_year;
	}

	//============================================================================
	inline int	CTime::GetMonth(bool local /* = true */) const
	//	Return the month of *this time object (1 through 12), local (true)  or
	//	UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return tmbuffer->tm_mon + 1;
	}

	//============================================================================
	inline int	CTime::GetDay(bool local /* = true */) const
	//	Return the day of *this object (1 through 31), local (true)  or
	//	UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return tmbuffer->tm_mday ;
	}

	//============================================================================
	inline int	CTime::GetHour(bool local /* = true */) const
	//	Return the hour of *this object (0 through 23), local (true)  or
	//	UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return tmbuffer->tm_hour;
	}

	//============================================================================
	inline int	CTime::GetMinute(bool local /* = true */) const
	//	Return the minute of *this object (0 through 59), local (true)  or
	//	UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return tmbuffer->tm_min;
	}

	//============================================================================
	inline int	CTime::GetSecond(bool local /* = true */) const
	//	Return the second of *this object (0 through 61), local (true)  or
	//	UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return tmbuffer->tm_sec;
	}

	//============================================================================
	inline int	CTime::GetDayOfWeek(bool local /* = true */) const
	//	Return the day of the week of *this object (0�6, Sunday = 0), local
	//	(true) or UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return tmbuffer->tm_wday;
	}

	//============================================================================
	inline int	CTime::GetDayOfYear(bool local /* = true */) const
	//	Return the day of the year of *this object (1-366), local (true)  or
	//	UTC (false).
	{
		time_tm *tmbuffer = (local ? GetLocalTm(NULL) : GetGmtTm());
		return tmbuffer->tm_yday + 1;
	}

	//============================================================================
	inline const CTime& CTime::operator=(const CTime& t)
	//	Assign the CTime t value to *this
	{
		// cannot use assignment *this = t; here, as it is recursive
		// (i.e., "=" operator would call itself incessantly)
		m_time = t.m_time;
		return *this;
	}

	//============================================================================
	inline const CTime& CTime::operator=(time_t t)
	//	Assign the time_t value to *this time.
	{
		m_time = t;
		return *this;;
	}

	//============================================================================
	inline CTimeSpan CTime::operator-(CTime t) const
	//	Return the time span between *this time and  time t
	{
		timespan_t d = (timespan_t)(m_time - t.m_time);
		CTimeSpan t0(d);
		return  t0;
	}

	//============================================================================
	inline CTime CTime::operator-(CTimeSpan ts) const
	//	Return the CTime that is the time span ts before *this time.
	{
		time_t d = m_time - ts.m_timespan;
		CTime t(d);
		return t;
	}

	//============================================================================
	inline CTime CTime::operator+(CTimeSpan ts) const
	//	Return the CTime that is the time span ts after *this time.
	{
		time_t s = m_time + ts.m_timespan;
		CTime t(s);
		return t;
	}

	/*============================================================================*/
	inline const CTime& CTime::operator+=(CTimeSpan ts)
	//	Increment *this time by the time span ts and  return this CTime.
	{
		m_time += ts.m_timespan;
		return *this;
	}

	//============================================================================
	inline const CTime& CTime::operator-=(CTimeSpan ts)
	//	Decrement *this time by the time span ts and  return this CTime.
	{
		m_time -= ts.m_timespan;
		return *this;
	}

	//============================================================================
	inline bool CTime::operator==(CTime t) const
	//	Return true if *this and  t are the same times.
	{
		return m_time == t.m_time;
	}

	//============================================================================
	inline bool CTime::operator!=(CTime t) const
	//	Return true if *this and  t are not the same times.
	{
		return m_time != t.m_time;
	}

	//============================================================================
	inline bool CTime::operator<(CTime t) const
	//	Return true if *this time is less than time t.
	{
		return m_time < t.m_time;
	}

	//============================================================================
	inline bool CTime::operator>(CTime t) const
	//	Return true if *this time is greater than time t.
	{
		return m_time > t.m_time;
	}

	//============================================================================
	inline bool CTime::operator<=(CTime time) const
	//	Return true if *this time is less than or equal to time t.
	{
		return m_time <= time.m_time;
	}

	//============================================================================
	inline bool CTime::operator>=(CTime time) const
	//	Return true if *this time is greater than or equal to time t.
	{
		return m_time >= time.m_time;
	}

	//============================================================================
	inline CString CTime::Format(LPCTSTR pFormat) const
	//	Create a formatted representation of this date/time value as a local
	//	time. If  this CTime object is null or invalid, the return value is
	//	an empty CString. The pFormat string is converted using the conventions
	//	of the C function strftime(). Consult the C++ reference for details.
	{
		TCHAR szBuffer[maxTimeBufferSize];

		time_tm* ptm = ::localtime(&m_time);
		if (ptm == NULL ||
			!::_tcsftime(szBuffer, maxTimeBufferSize, pFormat, ptm))
			szBuffer[0] = '\0';
		return (CString)szBuffer;
	}

	//============================================================================
	inline CString CTime::Format(UINT nFormatID) const
	//	Create a formatted representation of this date/time value as a local
	//	time, in the same manner as an LPCTSTR argument, but nFormatID
	//	identifies a resource string.
	{
		CString strFormat;
		VERIFY( strFormat.LoadString(nFormatID) );
		return Format(strFormat);
	}

	//============================================================================
	inline CString CTime::Format(const CString &format) const
	//	Create a formatted representation of this date/time value as a local
	//	time, in the same manner as an LPCTSTR argument, but using a CString
	//	format instead.
	{
		return Format(format.c_str());
	}

	//============================================================================
	inline CString CTime::FormatGmt(LPCTSTR pFormat) const
	//	Create a formatted representation of this date/time value as a UTC
	//	time. If  this CTime object is null or invalid, the return value is
	//	an empty CString. The pFormat string is converted using the conventions
	//	of the C function strftime(). Consult the C++ reference for details.
	{
		TCHAR szBuffer[maxTimeBufferSize];
		CString fmt0 = pFormat;
		while (fmt0.Replace(TEXT("%Z"), TEXT("Coordinated Universal Time")))
			;
		while (fmt0.Replace(TEXT("%z"), TEXT("UTC")))
			;

		time_tm* ptmTemp = GetGmtTm();
		if (ptmTemp ==NULL || !::_tcsftime(szBuffer, maxTimeBufferSize,
			fmt0.c_str(), ptmTemp))
			szBuffer[0] = '\0';
		return CString(szBuffer);
	}

	//============================================================================
	inline CString CTime::FormatGmt(UINT nFormatID) const
	//	Create a formatted representation of this date/time value as a UTC
	//	time, in the same manner as an LPCTSTR argument, but nFormatID
	//	identifies a resource string.
	{
		CString strFormat;
		VERIFY( strFormat.LoadString(nFormatID) );
		return FormatGmt(strFormat);
	}

	//============================================================================
	inline CString CTime::FormatGmt(const CString &format) const
	//	Create a formatted representation of this date/time value as a UTC
	//	time, in the same manner as an LPCTSTR argument, but using a CString
	//	format instead.
	{
		return FormatGmt(format.c_str());
	}

	//============================================================================
	inline CString CTime::StdFormat() const
	//	Return a CString containing the elements of *this time in the standard
	//	form specified by the stdTForm string.
	{
		return Format(stdTForm);
	}


	//
	//	Static and  Friend Functions
	//

	//============================================================================
	inline CTime CTime::GetCurrentTime()
	//	This is a static method that return the current system time as a
	//	CTime object.
	{
		return CTime(::time(NULL));
	}

	//============================================================================
	inline FILETIME CTime::FileTimePlus(const FILETIME &ft, double addend)
	//	This is a static function to return a FILETIME structure containing the
	//	FILETIME ft increased by addend seconds.
	{
		// convert ft to unsigned long long
		ULONGLONG ftlong = (ULONGLONG)
			(ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
		ULONGLONG ftaddend = (ULONGLONG)(addend * 10000000);
		if (addend > 0.)
			ftlong += ftaddend;
		else
			ftlong -= ftaddend;
		FILETIME fts;
		fts.dwHighDateTime = (DWORD)(ftlong >> 32);
		fts.dwLowDateTime  = (DWORD)(ftlong & ~0);
		return fts;
	}

	//
	// Global functions within the Win32xx namespace
	//

	//============================================================================
	inline CArchive& operator>>(CArchive& ar, CTime t)
	//	Read a CTime from the archive and  store it in t.  Throw an exception if
	//	unable to do so correctly.
	{
		int size;
		ar.Read((char *)&size, sizeof(size_t));
		if (size != sizeof(CTime))
		{
			ar.SetError(ERROR_INVALID_DATA);
			throw CWinException(TEXT("Archive corruption: reading CTime."));
		}

			ar.Read((char *)&t, size);
		return ar;
	}

	//============================================================================
	inline CArchive& operator<<(CArchive &ar, CTime t)
	//	Write the time t into the archive file. Throw an exception if an
	//	error occurs.
	{
		int size = sizeof(t);
		ar.Write((char *)&size, sizeof(size_t));
		ar.Write((char *)&t, size);
		return ar;
	}


	///////////////////////////////////////////////////////////////
	//
	//	CTimeSpan class implementation
	//
	///////////////////////////////////////////////////////////////

	//============================================================================
	inline CTimeSpan::CTimeSpan()
	//	Construct an CTimeSpan object initialized to 0.
	{
		m_timespan = 0;
	}

	//============================================================================
	inline CTimeSpan::CTimeSpan(timespan_t t)
	//	Construct a CTimeSpan object from time_t type t. This value is the
	//	difference between two absolute time_t values.
	{
		m_timespan = t;
	}

	//============================================================================
	inline CTimeSpan::CTimeSpan(long lDays, int nHours, int nMins, int nSecs)
	//	Construct a CTimeSpan object from given time elements, where each
	//	element is constrained to the following ranges:
	//		lDays 	0�24,855 (approximately)
	//		nHours 	0�23
	//		nMins 	0�59
	//		nSecs 	0�59
	{
		m_timespan = lDays * sec_per_day + nHours * sec_per_hour +
			nMins * sec_per_min + nSecs;
	}

	//============================================================================
	inline CTimeSpan::CTimeSpan(const CTimeSpan& ts)
	//	Construct a CTimeSpan object from another CTimeSpan value.
	{
		m_timespan = ts.m_timespan;
	}

	//============================================================================
	inline long CTimeSpan::GetDays() const
	//	Return the number of complete days in this CTimeSpan.  This value may
	//	be negative if the time span is negative.
	{
		return m_timespan / sec_per_day;
	}

	//============================================================================
	inline long CTimeSpan::GetTotalHours() const
	//	Return the total number of complete hours in this CTimeSpan.
	{
		return m_timespan / sec_per_hour;
	}

	//============================================================================
	inline long CTimeSpan::GetTotalMinutes() const
	//	Return the total number of complete minutes in this CTimeSpan.
	{
		return m_timespan / sec_per_min;
	}

	//============================================================================
	inline long CTimeSpan::GetTotalSeconds() const
	//	Return the total number of complete seconds in this CTimeSpan.
	{
		return m_timespan;
	}

	//============================================================================
	inline int CTimeSpan::GetHours() const
	//	Return the number of hours in the day component of this time
	//	span (�23 through 23).
	{
		return (m_timespan / sec_per_hour) % hours_per_day;
	}

	//============================================================================
	inline int CTimeSpan::GetMinutes() const
	//	Return the number of minutes in the hour component of this time
	//	span (�59 through 59).
	{
		return (m_timespan / sec_per_min) % sec_per_min;
	}

	//============================================================================
	inline int CTimeSpan::GetSeconds() const
	//	Return the number of seconds in the minute component of this time
	//	span (�59 through 59).
	{
		return m_timespan % sec_per_min;
	}

	//============================================================================
	inline const CTimeSpan& CTimeSpan::operator=(const CTimeSpan& ts)
	//	Assign the CTimeSpan ts to *this time span
	{
		m_timespan = ts.m_timespan;
		return *this;
	}

	//============================================================================
	inline const CTimeSpan& CTimeSpan::operator=(timespan_t t)
	//	Assign the timespan_t t value to *this time span.
	{
		m_timespan = t;
		return *this;
	}

	//============================================================================
	inline CTimeSpan CTimeSpan::operator-() const
	//	Return the negated value of *this time span.
	{
		CTimeSpan t0(-m_timespan);
		return  t0;
	}

	//============================================================================
	inline CTimeSpan CTimeSpan::operator-(CTimeSpan ts) const
	//	Return the result of subtracting the time span ts from *this time span.
	{
		timespan_t d = m_timespan - ts.m_timespan;
		CTimeSpan tsp(d);
		return tsp;
	}

	//============================================================================
	inline CTimeSpan CTimeSpan::operator+(CTimeSpan ts) const
	//	Return the result of adding the time span ts to *this time span.
	{
		timespan_t s = m_timespan + ts.m_timespan;
		CTimeSpan tsp(s);
		return tsp;
	}

	//============================================================================
	inline const CTimeSpan& CTimeSpan::operator+=(CTimeSpan ts)
	//	Increase *this time span by the ts time span and  return the result.
	{
		m_timespan += ts.m_timespan;
		return *this;
	}

	//============================================================================
	inline const CTimeSpan& CTimeSpan::operator-=(CTimeSpan ts)
	//	Decrease *this time span by the ts time span and  return the result.
	{
		m_timespan -= ts.m_timespan;
		return *this;
	}

	//============================================================================
	inline bool CTimeSpan::operator==(CTimeSpan ts) const
	//	Return true if the time span ts equals *this one.
	{
		return m_timespan == ts.m_timespan;
	}

	//============================================================================
	inline bool CTimeSpan::operator!=(CTimeSpan ts) const
	//	Return true if the time span ts does not equal *this one.
	{
		return m_timespan != ts.m_timespan;
	}

	//============================================================================
	inline bool CTimeSpan::operator<(CTimeSpan ts) const
	//	Return true if *this time span is less than the ts time span.
	{
		return m_timespan < ts.m_timespan;
	}

	//============================================================================
	inline bool CTimeSpan::operator>(CTimeSpan ts) const
	//	Return true if *this time span is greater than the ts time span.
	{
		return m_timespan > ts.m_timespan;
	}

	//============================================================================
	inline bool CTimeSpan::operator<=(CTimeSpan ts) const
	//	Return true if *this time span is less than or equal to the ts
	//	time span.
	{
		return m_timespan <= ts.m_timespan;
	}

	//============================================================================
	inline bool CTimeSpan::operator>=(CTimeSpan ts) const
	//	Return true if *this time span is greater than or equal to the ts
	//	time span.
	{
		return m_timespan >= ts.m_timespan;
	}

	//============================================================================*/
	inline CString CTimeSpan::Format(LPCTSTR pFormat) const
	//	Return a rendering of *this CTimeSpan object in CString form using the
	//	pFormat as the template. The valid format directives are
	//	      %D - number of days
	//	      %H - hour (0-23)
	//	      %M - minute (0-59)
	//	      %S - seconds (0-59)
	{
		CString fmt0 = pFormat,
			insert;

		while (fmt0.Find(TEXT("%D")) != -1)  // number of days
		{
			insert.Format(TEXT("%ld"), GetDays());
			fmt0.Replace(TEXT("%D"), insert);
		}
		while (fmt0.Find(TEXT("%H")) != -1)  // hours (00 � 23)
		{
			insert.Format(TEXT("%02d"), GetHours());
			fmt0.Replace(TEXT("%H"), insert);
		}
		while (fmt0.Find(TEXT("%M")) != -1)  // minutes (00 � 59)
		{
			insert.Format(TEXT("%02d"), GetMinutes());
			fmt0.Replace(TEXT("%M"), insert);
		}
		while (fmt0.Find(TEXT("%S")) != -1)  // seconds (00 - 59)
		{
			insert.Format(TEXT("%02d"), GetSeconds());
			fmt0.Replace(TEXT("%S"), insert);
		}
		return fmt0;
	}

	//============================================================================
	inline CString CTimeSpan::Format(UINT nFormatID) const
	//	Return a rendering of *this CTimeSpan object in CString form using the
	//	string resource having the nFormatID identifer as the template. The
	//	valid format directives are
	//	      %D - number of days
	//	      %H - hour (0-23)
	//	      %M - minute (0-59)
	//	      %S - seconds (0-59)
	{
		CString strFormat;
		VERIFY( strFormat.LoadString(nFormatID) );
		return Format(strFormat);
	}

	//============================================================================
	inline CString CTimeSpan::Format(const CString &format) const
	//	Return a rendering of *this CTimeSpan object in CString form using the
	//	CString format as the template. The valid format directives are
	//	      %D - number of days
	//	      %H - hour (0-23)
	//	      %M - minute (0-59)
	//	      %S - seconds (0-59)
	{
		return Format(format.c_str());
	}

	//
	// Global functions within the Win32xx namespace
	//

	//============================================================================
	inline CArchive& operator>>(CArchive& ar, CTimeSpan t)
	//	Read a CTimeSpan object from the archive and  store it in t.  Throw an
	//	exception if unable to do so correctly.
	{
		int size;
		ar.Read((char *)&size, sizeof(size_t));
		if (size != sizeof(CTimeSpan))
		{
			ar.SetError(ERROR_INVALID_DATA);
			throw CWinException(
				TEXT("Archive corruption: reading CTimeSpan."));
		}

			ar.Read((char *)&t, size);
		return ar;
	}

	//============================================================================
	inline CArchive& operator<<(CArchive &ar, CTimeSpan s)
	//	Write the time span object s into the archive file. Throw an exception
	//	if an error occurs.
	{
		int size = sizeof(s);
		ar.Write((char *)&size, sizeof(size_t));
		ar.Write((char *)&s, size);
		return ar;
	}

} // namespace Win32XX


#endif // _WIN32XX_CTIME_H_

