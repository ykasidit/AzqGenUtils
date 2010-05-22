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
#ifdef EKA2//3rd edition
#include <e32cmn.h>
#else
#include <aknutils.h>
#endif

#include <f32file.h>
#include "AzenqosEngineUtils.h"

#include <aknquerydialog.h>
#include <aknnotewrappers.h>

//_LIT8(KComma8,",");
_LIT8(KQuote8,"\"");

//_LIT(KComma16,",");
_LIT(KQuote16,"\"");



_LIT(KTimeStampFileFormat,"%02d%02d%02d_%02d%02d%02d");
_LIT(KTimeStampMillisecFormat,"%02d%02d%02d_%02d:%02d:%02d.%03d");
const TInt KTimeStampMillisecStrLen = 19;




void TAzenqosEngineUtils::CompleteWithPrivatePathL(TDes& des)
{
#ifdef EKA2//3rd edition
	RFs rfs;
	TFileName fn;

	User::LeaveIfError(rfs.Connect());
	CleanupClosePushL(rfs);
	User::LeaveIfError(rfs.PrivatePath(fn));
	CleanupStack::PopAndDestroy();
	fn += des;
	des = fn;
#else
	CompleteWithAppPath(des);
#endif
}

void TAzenqosEngineUtils::MakeTimeStrFile(TTime &time,TDes& str) //str should be at least 19 in length
	{
		TDateTime date = time.DateTime();
		str.Format(KTimeStampFileFormat,date.Year()%2000,date.Month()+1,date.Day()+1,date.Hour(),date.Minute(),date.Second());
	}


void TAzenqosEngineUtils::MakeTimeStrMilli(TTime &time,TDes& str) //str should be at least 19 in length
{

	const TInt KThousand = 1000;

	TDateTime date = time.DateTime();
	str.Format(KTimeStampMillisecFormat,date.Year()%2000,date.Month()+1,date.Day()+1,date.Hour(),date.Minute(),date.Second(),date.MicroSecond()*KThousand);

}

void TAzenqosEngineUtils::ParseTimeStrMilliL(const TDesC8& str,TTime &time)
	{
		HBufC* buf = HBufC::NewLC(str.Length());

		buf->Des().Copy(str);
		ParseTimeStrMilliL(*buf,time);

		CleanupStack::PopAndDestroy(buf);
	}

void TAzenqosEngineUtils::ParseTimeStrMilliL(const TDesC& str,TTime &time)
	{
		TInt instrlen = str.Length();
		if(str.Length() != KTimeStampMillisecStrLen )
			User::Leave(KErrBadName);

		TDateTime date;

		TPtrC ptr(0,0);
		TInt pos=0;
		TLex lex;
		TInt val;

		//_LIT(KTimeStampMillisecFormat,"%02d%02d%02d_%02d:%02d:%02d.%03d");

		///year
		ptr.Set(str.Mid(pos,2));
		pos+=2;
		lex.Assign(ptr);
		val=0;
		User::LeaveIfError(lex.Val(val));
		val+=2000;
		User::LeaveIfError(date.SetYear(val));

		//month
		ptr.Set(str.Mid(pos,2));
		pos+=2;
		lex.Assign(ptr);
		val=0;
		User::LeaveIfError(lex.Val(val));
		if(val<1 || val > 12)//month should be at least 1 so we can set as symbian format: month-1
			User::Leave(KErrGeneral);
		val--;//Symbian format EJanuary is 0
		User::LeaveIfError(date.SetMonth((TMonth)val));

		//day
		ptr.Set(str.Mid(pos,2));
		pos+=2;
		lex.Assign(ptr);
		val=0;
		User::LeaveIfError(lex.Val(val));
		if(val<1 || val > 31)//day limit
			User::Leave(KErrGeneral);
		val--;//Symbian format EJanuary is 0
		User::LeaveIfError(date.SetDay(val));


		//skip '_'
		pos+=1;

		//HH
		ptr.Set(str.Mid(pos,2));
		pos+=2;
		lex.Assign(ptr);
		val=0;
		User::LeaveIfError(lex.Val(val));
		if(val<0 || val > 23)//0 to 23 Hrs
			User::Leave(KErrGeneral);
		User::LeaveIfError(date.SetHour(val));

		//skip ':'
		pos+=1;

		//MM
		ptr.Set(str.Mid(pos,2));
		pos+=2;
		lex.Assign(ptr);
		val=0;
		User::LeaveIfError(lex.Val(val));
		if(val<0 || val > 59)//0 to 59 mins
			User::Leave(KErrGeneral);
		User::LeaveIfError(date.SetMinute(val));

		//skip ':'
		pos+=1;

		//SS
		ptr.Set(str.Mid(pos,2));
		pos+=2;
		lex.Assign(ptr);
		val=0;
		User::LeaveIfError(lex.Val(val));
		if(val<0 || val > 59)//0 to 59 Seconds
			User::Leave(KErrGeneral);
		User::LeaveIfError(date.SetSecond(val));

		//skip '.'
		pos+=1;

		//SSS
		ptr.Set(str.Mid(pos,3));
		pos+=3;
		lex.Assign(ptr);
		val=0;
		User::LeaveIfError(lex.Val(val));
		if(val<0 || val > 999999)//0 to 999999 MicroSeconds
			User::Leave(KErrGeneral);
		User::LeaveIfError(date.SetMicroSecond(val));

		time = date;

	}

TBool TAzenqosEngineUtils::TokenizeCSV(const TDesC16& whole,TPtrC16& ret, TPtrC16& remainder, TChar aDelim)
	{
		TPtrC16 afterFristQuote(0,0);

		TBuf<3> aDelimStr;
		aDelimStr.Append(aDelim);

		TInt firstQuotePos = whole.Find(KQuote16);
		TInt CommaPos = whole.Find(aDelimStr);
		TInt secondQuotePos = -1;

		TBool encounteredQuote = EFalse;
		if(firstQuotePos>=0 && firstQuotePos<CommaPos)
		{
			encounteredQuote = ETrue;
			afterFristQuote.Set(whole.Right(whole.Length()-firstQuotePos-1));

			secondQuotePos = afterFristQuote.Find(KQuote16);

			if(secondQuotePos<0)
			{
				TBuf<32> countbuf;
				countbuf =_L("Parse Error: Mis-Quote");
		/*
			CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
	       	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
	       	informationNote->ExecuteLD(countbuf);*/
				return EFalse;//misquote
			}

			secondQuotePos += (firstQuotePos+1);
			CommaPos = secondQuotePos+1;



		}
		else
		{
			//csv.Set(incsv);
		}



		if(CommaPos>=0)
		{
			/*if(encounteredQuote)
			{
				ret.Set(incsv.Mid(1,CommaPos-1));
			}
			else
			{

			}*/
			ret.Set(whole.Left(CommaPos));

			if(ret.Length()>=2 && ret[0] == '"' && ret[ret.Length()-1] == '"' )
			{
				if(ret.Length()>2)
				{
					TPtrC16 tmp(0,0);
					tmp.Set(whole.Left(CommaPos));
					ret.Set(tmp.Mid(1,tmp.Length()-2));
				}
				else //ret==2
				{
					ret.Set(0,0);
				}

			}




			remainder.Set(whole.Right(whole.Length()-CommaPos-1));





			return ETrue;
		}


		//remainder.Set(incsv);
		return EFalse;
	}



TBool TAzenqosEngineUtils::TokenizeCSV8(const TDesC8& whole,TPtrC8& ret, TPtrC8& remainder, TChar aDelim)
	{
		TPtrC8 afterFristQuote(0,0);

		TBuf8<3> aDelimStr;
		aDelimStr.Append(aDelim);

		TInt firstQuotePos = whole.Find(KQuote8);
		TInt CommaPos = whole.Find(aDelimStr);
		TInt secondQuotePos = -1;

		TBool encounteredQuote = EFalse;
		if(firstQuotePos>=0 && firstQuotePos<CommaPos)
		{
			encounteredQuote = ETrue;

			afterFristQuote.Set(whole.Right(whole.Length()-firstQuotePos-1));

			secondQuotePos = afterFristQuote.Find(KQuote8);

			if(secondQuotePos<0)
			{
				TBuf<32> countbuf;
				countbuf =_L("Parse Error: Mis-Quote");

/*			CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
	       	informationNote->SetTimeout(CAknNoteDialog::EShortTimeout);
	       	informationNote->ExecuteLD(countbuf);*/
				return EFalse;//misquote
			}

			secondQuotePos += (firstQuotePos+1);
			CommaPos = secondQuotePos+1;



		}
		else
		{
			//csv.Set(incsv);
		}



		if(CommaPos>=0)
		{
			/*if(encounteredQuote)
			{
				ret.Set(incsv.Mid(1,CommaPos-1));
			}
			else
			{

			}*/
			ret.Set(whole.Left(CommaPos));

			if(ret.Length()>=2 && ret[0] == '"' && ret[ret.Length()-1] == '"' )
			{
				if(ret.Length()>2)
				{
					TPtrC8 tmp(0,0);
					tmp.Set(whole.Left(CommaPos));
					ret.Set(tmp.Mid(1,tmp.Length()-2));
				}
				else //ret==2
				{
					ret.Set(0,0);
				}

			}




			remainder.Set(whole.Right(whole.Length()-CommaPos-1));





			return ETrue;
		}


		//remainder.Set(incsv);
		return EFalse;
	}

TBool TAzenqosEngineUtils::AskTestDescription(TInt rscid, TDes& descr)
	{
							//descr.Zero();
							CAknTextQueryDialog* cldlg;
					        cldlg = new (ELeave) CAknTextQueryDialog(descr);
					        cldlg->PrepareLC( rscid );
					        if(!(cldlg->RunLD()))
					        {
					        	return EFalse;
					        }

					        descr.TrimAll();
					        if(descr.Length()==0)
					        {
			       				_LIT(emsg,"Test description not entered");
							    CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
							   	informationNote->ExecuteLD(emsg);
							   	return EFalse;
					        }

					        return ETrue;

	}

