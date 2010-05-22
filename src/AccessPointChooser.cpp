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

#include "AccessPointChooser.h"

#include <es_enum.h>


//////////////// generic match strings
_LIT(KWEBAPMatchStrLower,"web");
_LIT(KDATAAPMatchStrLower,"data");//example: 3gprs
_LIT(KGPRSAPMatchStrLower,"gprs");//example: 3gprs
_LIT(KInternetAPMatchStrLower,"net");
_LIT(KSurfAPMatchStrLower,"surf");
_LIT(KWAPAPMatchStrLower,"wap");
////////////////////////////////////


void TAccessPointChooser::SetAccessPointTypeMatchSetL(TBool aWebOrWap, TAccessPointNameMatchSet& aAPMatchSet)
	{
		if(aWebOrWap) //WEB
			{
				aAPMatchSet.iAPTypeNameVariations.AppendL(KWEBAPMatchStrLower);
				aAPMatchSet.iAPTypeNameVariations.AppendL(KDATAAPMatchStrLower);
				aAPMatchSet.iAPTypeNameVariations.AppendL(KGPRSAPMatchStrLower);
				aAPMatchSet.iAPTypeNameVariations.AppendL(KInternetAPMatchStrLower);
				aAPMatchSet.iAPTypeNameVariations.AppendL(KSurfAPMatchStrLower);

			}
		else //WAP
			{
				aAPMatchSet.iAPTypeNameVariations.AppendL(KWAPAPMatchStrLower);
			}
	}


void TAccessPointChooser::ChooseAccessPointL(const TDesC& aName, TCommDbConnPref& aPrefRet, TDes& aErrStr)
	{

	TBuf<64> name;
	TBuf<64> nameori;

	CCommsDatabase* commDB = CCommsDatabase::NewL(EDatabaseTypeIAP);
	CleanupStack::PushL(commDB);

	CCommsDbTableView* view = commDB->OpenIAPTableViewMatchingBearerSetLC( KCommDbBearerCSD|KCommDbBearerWcdma|KCommDbBearerCdma2000|ECommDbBearerGPRS, ECommDbConnectionDirectionOutgoing);

	// go to the first record in the view
	TInt ret = view->GotoFirstRecord();
	TUint32 id;
	TBool found = EFalse;

		while ( ret == KErrNone )
		    {

			    name.Zero();
			    nameori.Zero();

			    view->ReadUintL( TPtrC( COMMDB_ID ), id );
			    view->ReadTextL( TPtrC( COMMDB_NAME ), name );
			    nameori = name;

			    //name.LowerCase();

				if(name == aName)
					{
						found = ETrue;
						break;
					}

			    if(found)
					break;

			    ret = view->GotoNextRecord();
		    }


	if(!found)
		{
			_LIT(KNoAPFoundStr,"Can't find specified access point");
			aErrStr = KNoAPFoundStr;
			User::Leave(KErrNotFound);
		}

	aErrStr = nameori;//return the chosen access point name


	CleanupStack::PopAndDestroy( view );
	CleanupStack::PopAndDestroy(commDB);

	aPrefRet.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
	aPrefRet.SetIapId(id);

	}


void TAccessPointChooser::ChooseAccessPointL( TBool aWebOrWap, TCommDbConnPref& aPrefRet, TDes& aErrStr)
	{

		CDesC16ArrayFlat* aOpNames =  new (ELeave) CDesC16ArrayFlat(3);
		CleanupStack::PushL(aOpNames);

		CDesC16ArrayFlat* aTypeNames = new (ELeave) CDesC16ArrayFlat(3);
		CleanupStack::PushL(aTypeNames);


		TAccessPointNameMatchSet aAccessPointNameMatchSet(*aOpNames,*aTypeNames);
		aErrStr = _L("FailedIn: SetAccessPointTypeMatchSetL");
		SetAccessPointTypeMatchSetL(aWebOrWap,aAccessPointNameMatchSet);

		CleanupStack::PopAndDestroy(aTypeNames);
		CleanupStack::PopAndDestroy(aOpNames);

	}

TAccessPointNameMatchSet::TAccessPointNameMatchSet(CDesC16ArrayFlat& aOperatorNameVariations, CDesC16ArrayFlat& aAPTypeNameVariations)
:iAPTypeNameVariations(aAPTypeNameVariations)
	{




	}

void TAccessPointChooser::ChooseAccessPointL(TAccessPointNameMatchSet& aAPMatchSet, TCommDbConnPref& aPrefRet, TDes& aErrStr)
	{


	TBuf<64> name;
	TBuf<64> nameori;

	CCommsDatabase* commDB = CCommsDatabase::NewL(EDatabaseTypeIAP);
	CleanupStack::PushL(commDB);




	CCommsDbTableView* view = commDB->OpenIAPTableViewMatchingBearerSetLC( KCommDbBearerCSD|KCommDbBearerWcdma|KCommDbBearerCdma2000|ECommDbBearerGPRS, ECommDbConnectionDirectionOutgoing);

	// go to the first record in the view
	TInt ret = view->GotoFirstRecord();
	TUint32 id;
	TBool found = EFalse;

		while ( ret == KErrNone )
		    {

			    name.Zero();
			    nameori.Zero();

			    view->ReadUintL( TPtrC( COMMDB_ID ), id );
			    view->ReadTextL( TPtrC( COMMDB_NAME ), name );
			    nameori = name;
			    name.LowerCase();



			    for(TInt i=0;i<aAPMatchSet.iAPTypeNameVariations.Count(); i++)
			    	{
								if(name.Find(aAPMatchSet.iAPTypeNameVariations[i]) >= 0 )
									{
										found = ETrue;
										break;
									}

						if(found)
							break;
			    	}


			    if(found)
					break;

			    ret = view->GotoNextRecord();
		    }


	if(!found)
		{
			ret = view->GotoFirstRecord();

			if(ret != KErrNone)
				{
					_LIT(KNoAPFoundStr,"Can't find any compatible GSM Access Points");
					aErrStr = KNoAPFoundStr;
					User::Leave(KErrNotFound);
				}
			else
				{
					view->ReadUintL( TPtrC( COMMDB_ID ), id );
					view->ReadTextL( TPtrC( COMMDB_NAME ), name );
					nameori = name;
				}
		}

	aErrStr = nameori;//return the chosen access point name


	CleanupStack::PopAndDestroy( view );
	CleanupStack::PopAndDestroy(commDB);



/*	// Lets first check are we already connected.

	TUint connectionCount;
	User::LeaveIfError(iConnect.EnumerateConnections(connectionCount));
	TPckgBuf<TConnectionInfo> connectionInfo;
	for (TUint i = 1; i <= connectionCount; i++)
	{
	User::LeaveIfError(iConnect.GetConnectionInfo(i, connectionInfo));
	if (connectionInfo().iIapId == id)
		{

		TBuf<128> buf;
		buf = nameori;
		buf=_L(" already in use");
		aErrStr = buf;

		User::Leave(KErrInUse);

		}
	}*/





	// Now we have the iap Id. Use it to connect for the connection.
	// setup preferences
	aPrefRet.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
	aPrefRet.SetIapId(id);

}
