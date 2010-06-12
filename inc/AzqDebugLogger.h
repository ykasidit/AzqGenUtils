/*
 ============================================================================
 Name		: AzqDebugLogger.h
 Author	  : Kasidit Yusuf
 Copyright   : Copyright (c) Freewill FX Co., Ltd.
 Description : Static lib header file
 ============================================================================
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

