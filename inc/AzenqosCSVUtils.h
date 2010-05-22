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

#ifndef AZENQOSCSVUTILS_H
#define AZENQOSCSVUTILS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <s32strm.h>

class TAzenqosCSVUtils
{
public:

static void WriteCommaAndBufToStreamL(RWriteStream &stream, const TDesC8& buf);
static void WriteCommaAndBufToStreamL(RWriteStream &stream, const TDesC16& buf);

static void WriteCommaAndIntToStream8L(RWriteStream &stream, TInt val);
static void WriteCommaAndIntToStream16L(RWriteStream &stream, TInt val);

static void ReadCSVColumnFromStreamL(RReadStream &stream, TDes8& buf);
static void ReadCSVColumnFromStreamL(RReadStream &stream, TDes16& buf);

static void ReadLastCSVColumnFromStreamL(RReadStream &stream, TDes8& buf);
static void ReadLastCSVColumnFromStreamL(RReadStream &stream, TDes16& buf);


static TInt ReadCSVColumnIntFromStream8L(RReadStream &stream);
static TInt ReadCSVColumnIntFromStream16L(RReadStream &stream);

static TInt ReadLastCSVColumnIntFromStream8L(RReadStream &stream);
static TInt ReadLastCSVColumnIntFromStream16L(RReadStream &stream);


};

#endif // AZENQOSCSVUTILS_H

