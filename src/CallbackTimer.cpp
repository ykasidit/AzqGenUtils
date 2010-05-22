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

#include "CallbackTimer.h"

CCallbackTimer* CCallbackTimer::NewL( const TInt aPriority, TCallBack& aCallBack )
{
	CCallbackTimer* self = new (ELeave) CCallbackTimer( aPriority, aCallBack );
	self->ConstructL();
	return self;;
};

CCallbackTimer::CCallbackTimer(const TInt aPriority, TCallBack& aCallBack): CTimer( aPriority )
{
	iCallBack = aCallBack;
}

void CCallbackTimer::ConstructL()
{
	CTimer::ConstructL();
	CActiveScheduler::Add( this );
}

void CCallbackTimer::RunL()
{
	iCallBack.CallBack();
}

TInt CCallbackTimer::RunError( TInt aError )
{
	iCallBack.CallBack();
	return KErrNone;
}

CCallbackTimer::~CCallbackTimer() {
	Cancel();
}
