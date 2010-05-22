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

#ifndef CALLBACKTIMER_H_
#define CALLBACKTIMER_H_

#include <e32base.h>

class CCallbackTimer : public CTimer {
public:
    static CCallbackTimer* NewL( const TInt aPriority, TCallBack& aCallBack );
    virtual ~CCallbackTimer();

private:
	CCallbackTimer(const TInt aPriority, TCallBack& aCallBack);
	void ConstructL();

protected:
	void RunL();
	TInt RunError( TInt aErr);


	TCallBack iCallBack;
};

#endif /* CALLBACKTIMER_H_ */
