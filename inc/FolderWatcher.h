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

#ifndef FOLDERWATCHER_H
#define FOLDERWATCHER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <f32file.h>

class MFolderObserver
	{
public:
	virtual void OnFolderChange(const TDesC& aFolder)=0;
	};

class CFolderWatcher : public CActive
	{
public:
	// Cancel and destroy
	virtual ~CFolderWatcher();

	// Two-phased constructor.
	static CFolderWatcher* NewL(const TDesC& aFolder, MFolderObserver& aObserver);

	// Two-phased constructor.
	static CFolderWatcher* NewLC(const TDesC& aFolder, MFolderObserver& aObserver);

public:
	// New functions
	// Function for making the initial request
	void StartL();

private:
	// C++ constructor
	CFolderWatcher(const TDesC& aFolder, MFolderObserver& aObserver);

	// Second-phase constructor
	void ConstructL();

	TFileName iFolder;
	MFolderObserver& iObserver;

	RFs iFs;

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);


	};

#endif // FOLDERWATCHER_H
