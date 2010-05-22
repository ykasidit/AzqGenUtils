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

#include "AzenqosCSVUtils.h"

_LIT8(KComma8,",");
_LIT16(KComma16,",");
_LIT8(KNumFormat8,"%d");
_LIT16(KNumFormat16,"%d");

void TAzenqosCSVUtils::WriteCommaAndIntToStream8L(RWriteStream &stream, TInt val)
	{
		TBuf8<32> buf;
		buf.Format(KNumFormat8,val);
		stream.WriteL(KComma8);
		stream.WriteL(buf);
	}

void TAzenqosCSVUtils::WriteCommaAndIntToStream16L(RWriteStream &stream, TInt val)
	{
		TBuf16<32> buf;
		buf.Format(KNumFormat16,val);
		stream.WriteL(KComma16);
		stream.WriteL(buf);
	}


void TAzenqosCSVUtils::WriteCommaAndBufToStreamL(RWriteStream &stream, const TDesC8& buf)
	{
		stream.WriteL(KComma8);
		stream.WriteL(buf);
	}

void TAzenqosCSVUtils::WriteCommaAndBufToStreamL(RWriteStream &stream, const TDesC16& buf)
	{
		stream.WriteL(KComma16);
		stream.WriteL(buf);
	}


TInt TAzenqosCSVUtils::ReadCSVColumnIntFromStream8L(RReadStream &stream)
	{
		TBuf8<32> buf;
		TChar comma = ',';

		stream.ReadL(buf,comma);

		TInt commapos = buf.Find(KComma8);

		if(commapos>=0)
			buf.Delete(commapos,1);

		TInt ret =0;
		TLex8 lex;
		lex.Assign(buf);
		User::LeaveIfError(lex.Val(ret));

		return ret;

	}

TInt TAzenqosCSVUtils::ReadCSVColumnIntFromStream16L(RReadStream &stream)
	{
		TBuf<32> buf;
		TChar comma = ',';

		stream.ReadL(buf,comma);

		TInt commapos = buf.Find(KComma16);

		if(commapos>=0)
			buf.Delete(commapos,1);

		TInt ret =0;
		TLex16 lex;
		lex.Assign(buf);
		User::LeaveIfError(lex.Val(ret));

		return ret;

	}

void TAzenqosCSVUtils::ReadCSVColumnFromStreamL(RReadStream &stream, TDes8& retbuf)
	{

		HBufC8* buf = HBufC8::NewLC(retbuf.MaxSize());

		TChar comma = ',';

		TPtr8 rptr(0,0);
		rptr.Set(buf->Des());

		stream.ReadL(rptr,comma);

		TInt commapos = (*buf).Find(KComma8);

		if(commapos>=0)
			buf->Des().Delete(commapos,1);

		retbuf = *buf;

		CleanupStack::PopAndDestroy();

	}


void TAzenqosCSVUtils::ReadCSVColumnFromStreamL(RReadStream &stream, TDes16& retbuf)
	{

		HBufC16* buf = HBufC16::NewLC(retbuf.MaxSize());

		TChar comma = ',';

		TPtr16 rptr(0,0);
		rptr.Set(buf->Des());

		stream.ReadL(rptr,comma);

		TInt commapos = (*buf).Find(KComma16);

		if(commapos>=0)
			buf->Des().Delete(commapos,1);

		retbuf = *buf;

		CleanupStack::PopAndDestroy();

	}














////Last, don't read with delim seek

//If I don't use minibuf then the last read data would not be transferred into my buf - I suppost it's because it had a Leave with KErrEof before transferring data to buf

TInt TAzenqosCSVUtils::ReadLastCSVColumnIntFromStream8L(RReadStream &stream)
	{
		TBuf8<32> buf;

		TRAPD(rerr,

				TBuf8<1> minibuf;

			for(TInt i=0;i<buf.MaxSize();i++)
				{
					stream.ReadL(minibuf);
					buf+=minibuf;
				}
		);

		if(rerr == KErrEof)//no problem, we expected this
			;
		else
			User::Leave(rerr);

		TInt commapos = buf.Find(KComma8);

		if(commapos>=0)
			User::Leave(KErrBadName); //this is supposed to be the last column

		TInt ret =0;
		TLex8 lex;
		lex.Assign(buf);
		User::LeaveIfError(lex.Val(ret));

		return ret;

	}

TInt TAzenqosCSVUtils::ReadLastCSVColumnIntFromStream16L(RReadStream &stream)
	{
		TBuf<32> buf;

		TRAPD(rerr,

				TBuf16<1> minibuf;

			for(TInt i=0;i<buf.MaxSize();i++)
				{
					stream.ReadL(minibuf);
					buf+=minibuf;
				}
		);

				if(rerr == KErrEof)//no problem, we expected this
					;
				else
					User::Leave(rerr);

		TInt commapos = buf.Find(KComma16);

		if(commapos>=0)
			User::Leave(KErrBadName); //this is supposed to be the last column


		TInt ret =0;
		TLex16 lex;
		lex.Assign(buf);
		User::LeaveIfError(lex.Val(ret));

		return ret;

	}

void TAzenqosCSVUtils::ReadLastCSVColumnFromStreamL(RReadStream &stream, TDes8& retbuf)
	{

		HBufC8* buf = HBufC8::NewLC(retbuf.MaxSize());



		TPtr8 rptr(0,0);
		rptr.Set(buf->Des());

		TRAPD(rerr,

				TBuf8<1> minibuf;

			for(TInt i=0;i<rptr.MaxSize();i++)
				{
					stream.ReadL(minibuf);
					rptr+=minibuf;
				}
		);
				if(rerr == KErrEof)//no problem, we expected this
					;
				else
					User::Leave(rerr);

		TInt commapos = (*buf).Find(KComma8);

		if(commapos>=0)
			User::Leave(KErrBadName); //this is supposed to be the last column

		retbuf = *buf;

		CleanupStack::PopAndDestroy();

	}


void TAzenqosCSVUtils::ReadLastCSVColumnFromStreamL(RReadStream &stream, TDes16& retbuf)
	{

		HBufC16* buf = HBufC16::NewLC(retbuf.MaxSize());



		TPtr16 rptr(0,0);
		rptr.Set(buf->Des());

		TRAPD(rerr,

						TBuf16<1> minibuf;

					for(TInt i=0;i<rptr.MaxSize();i++)
						{
							stream.ReadL(minibuf);
							rptr+=minibuf;
						}
				);

				if(rerr == KErrEof)//no problem, we expected this
					;
				else
					User::Leave(rerr);

		TInt commapos = (*buf).Find(KComma16);

		if(commapos>=0)
			User::Leave(KErrBadName); //this is supposed to be the last column

		retbuf = *buf;

		CleanupStack::PopAndDestroy();

	}

