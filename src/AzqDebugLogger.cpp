/*
 ============================================================================
 Name		: AzqDebugLogger.cpp
 Author	  : Kasidit Yusuf
 Copyright   : Copyright (c) Freewill FX Co., Ltd.
 Description : Static lib source file
 ============================================================================
 */

//  Include Files

#include "AzqDebugLogger.h"
#include "f32file.h"
#include "eikfutil.h"


const TInt KMaxLogFileSize = 10*1024;
const TInt KTimeStampMillisecStrLen = 19;
_LIT8(KTimeStampMillisecFormat8,"%02d%02d%02d_%02d:%02d:%02d.%03d");

void TDebugLog::RmFileIfBiggerThan(const TDesC& fp, TInt aMaxSize)
	{
		TRAPD(err,
		RFs fs = CCoeEnv::Static()->FsSession();
		RFile file;
		TInt fsz=0;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
		User::LeaveIfError(file.Open(fs,fp,EFileRead));
		CleanupClosePushL(file);
		User::LeaveIfError(file.Size(fsz));
		CleanupStack::PopAndDestroy();//file
		CleanupStack::PopAndDestroy();//fs

		if(fsz>KMaxLogFileSize)
				{
					EikFileUtils::DeleteFile(fp);
				}

		);
	}

void TDebugLog::InsertMakeTimeStrMilli8(TTime &time,TDes8& str) //str should be at least 19 in length
{
	const TInt KThousand = 1000;

	TDateTime date = time.DateTime();
	TBuf8<24> buf;
	buf.Format(KTimeStampMillisecFormat8,date.Year()%2000,date.Month()+1,date.Day()+1,date.Hour(),date.Minute(),date.Second(),date.MicroSecond()*KThousand);
	str.Insert(0,buf);
}

void TDebugLog::LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TInt aVal)
		{
		    #ifdef __SYMBIAN32__
	TRAPD(err,
			HBufC* buf = HBufC::NewLC(aVarName.Length()+20);
			buf->Des().Format(_L(": %d"),aVal);
			buf->Des().Insert(0,aVarName);

			LogToFile(aLogFile,*buf);
			CleanupStack::PopAndDestroy();
			);
			#endif

		};

void TDebugLog::LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TReal aVal)
		{
		    #ifdef __SYMBIAN32__
	TRAPD(err,
			HBufC* buf = HBufC::NewLC(aVarName.Length()+20);
			buf->Des().Format(_L(": %f"),aVal);
			buf->Des().Insert(0,aVarName);

			LogToFile(aLogFile,*buf);
			CleanupStack::PopAndDestroy();
			);
			#endif

		};


void TDebugLog::LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TDesC& aVal)
		{
			#ifdef __SYMBIAN32__
	TRAPD(err,
			HBufC* buf = HBufC::NewLC(aVarName.Length()+aVal.Length()+10);
			buf->Des() += _L(": ");
			buf->Des() += aVal;

			LogToFile(aLogFile,*buf);
			CleanupStack::PopAndDestroy();
			);
			#endif
		};

void TDebugLog::LogToFile(const TDesC& aLogFile,const TDesC8& aAzqLogLine)
{
	HBufC16* buf = HBufC16::NewLC(aAzqLogLine.Length());
	buf->Des().Copy(aAzqLogLine);
	LogToFile(aLogFile,*buf);
	CleanupStack::PopAndDestroy();
}

void TDebugLog::LogToFile(const TDesC& aLogFile,const TDesC& aAzqLogLine)
{
	#ifdef __SYMBIAN32__
	TRAPD(err,
			//this does not write headers since it's not a CAzqLogLine = not a csv...
			RmFileIfBiggerThan(aLogFile,KMaxLogFileSize);

			RFs fs = CCoeEnv::Static()->FsSession();
			User::LeaveIfError(fs.Connect());
			CleanupClosePushL(fs);
			RFile file;

			TInt err=KErrNone;
			err = file.Open(fs,aLogFile,EFileWrite);
			if(err!=KErrNone)
			{
				err = file.Create(fs,aLogFile,EFileWrite);
			}

			if(err==KErrNone)
			{
				CleanupClosePushL(file);
				TInt offset = 0;
				TInt fsize=0;
				file.Size(fsize);
				file.Seek(ESeekEnd,offset);


				HBufC8* buf =  HBufC8::NewLC(KTimeStampMillisecStrLen+aAzqLogLine.Length()+1);
					_LIT8(KSpace," ");
					TTime now;
					now.HomeTime();
					TPtr8 ptr(0,0);
					ptr.Set(buf->Des());
					ptr.Copy(aAzqLogLine);
					ptr.Insert(0,KSpace);
					InsertMakeTimeStrMilli8(now,ptr);

				file.Write(*buf);
				file.Write(_L8("\r\n"));
				file.Flush();
				CleanupStack::PopAndDestroy(buf);
				CleanupStack::PopAndDestroy();
			}
			else
			{
				User::Leave(err);
			}
			CleanupStack::PopAndDestroy();//fs
	);
	#endif
};
