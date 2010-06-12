/*
    Copyright (C) 2010 Kasidit Yusuf.

    This file is part of AzqGenUtils.

    AzqGenUtils is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AzqGenUtils is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with AzqGenUtils.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef __AZQDEBUGLOGGER_H__
#define __AZQDEBUGLOGGER_H__

#ifdef __SYMBIAN32__
	#include <e32std.h>
    #include <e32base.h>
	#include <f32file.h>
    #include <coemain.h>
#endif

class TDebugLog{
public:
	static void LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TInt aVal);
	static void LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TReal aVal);

	static void LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TDesC& aVal);

	static void LogToFile(const TDesC& aLogFile,const TDesC& aAzqLogLine);

	static void LogToFile(const TDesC& aLogFile,const TDesC8& aAzqLogLine);

	static void RmFileIfBiggerThan(const TDesC& fp, TInt aMaxSize);

	static void InsertMakeTimeStrMilli8(TTime &time,TDes8& str); //str should be at least 19 in length

	};

#endif  // __AZQDEBUGLOGGER_H__

