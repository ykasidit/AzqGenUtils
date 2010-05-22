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

#ifndef __AZENQOSENGINEUTILS_H__
#define __AZENQOSENGINEUTILS_H__


//  Include Files

#include <e32base.h>



class TAzenqosEngineUtils
{
	public:

		static void MakeTimeStrMilli(TTime &time,TDes& str); //str should be at least 19 in length
		static void MakeTimeStrFile(TTime &time,TDes& str);// for file name composition

		static void ParseTimeStrMilliL(const TDesC& str,TTime &time);//convert time str to time object
		static void ParseTimeStrMilliL(const TDesC8& str,TTime &time);//convert time str to time object

		static void CompleteWithPrivatePathL(TDes& des);

		static TBool TokenizeCSV(const TDesC16& whole,TPtrC16& ret, TPtrC16& remainder, TChar aDelim = ',');

		static TBool TokenizeCSV8(const TDesC8& whole,TPtrC8& ret, TPtrC8& remainder, TChar aDelim = ',');

		static TBool AskTestDescription(TInt rscid, TDes& descr);

};

#endif  // __AZENQOSENGINEUTILS_H__


