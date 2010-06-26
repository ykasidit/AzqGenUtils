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

//  Include Files

#include "AzqDebugLogger.h"
#include "f32file.h"
#include "eikfutil.h"

#include <aknnotewrappers.h>


const TInt KMaxLogFileSize = 10*1024;
const TInt KTimeStampMillisecStrLen = 19;
_LIT8(KTimeStampMillisecFormat8,"%02d%02d%02d_%02d:%02d:%02d.%03d");

void TDebugLog::RmFileIfBiggerThan(const TDesC& fp, TInt aMaxSize)
	{



		RFs fs = CCoeEnv::Static()->FsSession();
		RFile file;
		TInt fsz=0;

		TRAPD(err,
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
		User::LeaveIfError(file.Open(fs,fp,EFileRead));
		CleanupClosePushL(file);
		User::LeaveIfError(file.Size(fsz));
		CleanupStack::PopAndDestroy();//file
		CleanupStack::PopAndDestroy();//fs
		);

		if(fsz>KMaxLogFileSize)
				{
					EikFileUtils::DeleteFile(fp);
				}


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
			HBufC* buf = HBufC::NewLC(aVarName.Length()+20);
			buf->Des().Format(_L(": %d"),aVal);
			buf->Des().Insert(0,aVarName);

			LogToFile(aLogFile,*buf);
			CleanupStack::PopAndDestroy();

		};

void TDebugLog::LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TReal aVal)
		{
			HBufC* buf = HBufC::NewLC(aVarName.Length()+20);
			buf->Des().Format(_L(": %f"),aVal);
			buf->Des().Insert(0,aVarName);

			LogToFile(aLogFile,*buf);
			CleanupStack::PopAndDestroy();


		};


void TDebugLog::LogToFile(const TDesC& aLogFile,const TDesC& aVarName, const TDesC& aVal)
		{
			HBufC* buf = HBufC::NewLC(aVarName.Length()+aVal.Length()+10);
			buf->Des() += _L(": ");
			buf->Des() += aVal;

			LogToFile(aLogFile,*buf);
			CleanupStack::PopAndDestroy();


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
				//User::Leave(err);
			}
			CleanupStack::PopAndDestroy();//fs



};
