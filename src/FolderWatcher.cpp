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

#include "FolderWatcher.h"
#include <aknnotewrappers.h>

CFolderWatcher::CFolderWatcher(const TDesC& aFolder, MFolderObserver& aObserver) :
	CActive(EPriorityStandard),iObserver(aObserver) // Standard priority
	{
	iFolder = aFolder;
	}

CFolderWatcher* CFolderWatcher::NewLC(const TDesC& aFolder, MFolderObserver& aObserver)
	{
	CFolderWatcher* self = new (ELeave) CFolderWatcher(aFolder,aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CFolderWatcher* CFolderWatcher::NewL(const TDesC& aFolder, MFolderObserver& aObserver)
	{
	CFolderWatcher* self = CFolderWatcher::NewLC(aFolder,aObserver);
	CleanupStack::Pop(); // self;
	return self;
	}

void CFolderWatcher::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	CActiveScheduler::Add(this); // Add to scheduler
	}

CFolderWatcher::~CFolderWatcher()
	{
	Cancel(); // Cancel any request, if outstanding
	iFs.Close();
	}

void CFolderWatcher::DoCancel()
	{
	iFs.NotifyChangeCancel();
	}

void CFolderWatcher::StartL()
	{
	Cancel(); // Cancel any request, just to be sure
	iFs.NotifyChange(ENotifyEntry,iStatus,iFolder);
	SetActive(); // Tell scheduler a request is active
	}

void CFolderWatcher::RunL()
	{
	if(iStatus.Int() == KErrNone)
		{
			//report
			iObserver.OnFolderChange(iFolder);

			//re-watch folder again
			StartL();
		}
	else
		{
		CAknErrorNote* informationNote = new (ELeave) CAknErrorNote(ETrue);
		informationNote->SetTimeout(CAknNoteDialog::ELongTimeout);
		informationNote->ExecuteLD(_L("Notify folder change failed: please re-start application"));
		}
	}

TInt CFolderWatcher::RunError(TInt aError)
	{
		CAknInformationNote* informationNote = new (ELeave) CAknInformationNote(EFalse);
		informationNote->SetTimeout(CAknNoteDialog::ELongTimeout);
		informationNote->ExecuteLD(_L("Minor unhandled error occured: fwofc"));

		return aError;
	}
