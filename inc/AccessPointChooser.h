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

#ifndef ACCESSPOINTCHOOSER_H
#define ACCESSPOINTCHOOSER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <commdb.h>
#include <commdbconnpref.h>

#include <badesca.h>

class TAccessPointNameMatchSet
{

public:

TAccessPointNameMatchSet(CDesC16ArrayFlat& iOperatorNameVariations, CDesC16ArrayFlat& iAPTypeNameVariations);

//for example, "net", "web"
CDesC16ArrayFlat& iAPTypeNameVariations;

};

class TAccessPointChooser
{
public:
	static void ChooseAccessPointL(TBool aWebOrWap, TCommDbConnPref& aPrefRet, TDes& aErrStr);
	static void ChooseAccessPointL(const TDesC& aName, TCommDbConnPref& aPrefRet, TDes& aErrStr);


	static void SetAccessPointTypeMatchSetL(TBool aWebOrWap, TAccessPointNameMatchSet& aAPMatchSet);

	static void ChooseAccessPointL(TAccessPointNameMatchSet& aAPMatchSet, TCommDbConnPref& aPrefRet, TDes& aErrStr);


};


#endif // ACCESSPOINTCHOOSER_H

