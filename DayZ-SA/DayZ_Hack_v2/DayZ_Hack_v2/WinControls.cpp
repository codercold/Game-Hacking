#include "stdafx.h"

bool gameover = false;
HWND hWndList;
HWND hWndList2;
HWND hwndListWindow;


LV_COLUMN lvC;				// List View Column structure	
LV_ITEM lvI;				// List view item structure


HOUSEINFO rgHouseInfo[] = 
{
	{ "100 Main Street", "Redmond", 175000, 3, 2 },
	{ "523 Pine Lake Road", "Redmond", 125000, 4, 2},
	{ "1212 112th Place SE", "Redmond", 200000, 4, 3},
	{ "22 Lake Washington Blvd", "Bellevue", 2500000, 4, 4},
	{ "33542 116th Ave. NE", "Bellevue", 180000, 3, 2},
	{ "64134 Nicholas Lane", "Bellevue", 250000, 4, 3},
	{ "33 Queen Anne Hill", "Seattle", 350000, 3, 2},
	{ "555 SE Fifth St", "Seattle", 140000, 3, 2},
	{ "446 Mariners Way", "Seattle", 225000, 4, 3}
};


DWORD WINAPI Thread2( LPVOID lpParameter )
{	
	Sleep(3000);
	listBoxWindow();
	return 0;
}


// --- WindowProc2 ---
// Return type - The return value is the result of the message processing and depends on the message sent.
// Callback - _stdcall
// 1st arg - A handle to the window. 
// 2nd arg - The message -> http://msdn.microsoft.com/en-us/library/ms644927(v=VS.85).aspx#system_defined
// 3rd arg - Additional message-specific information. The contents depend on the value of uMsg.
// 4th arg - Additional message-specific information. The contencts depend on the value of uMsg.
LRESULT CALLBACK WindowProc2( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//HWND hButton, hCombo, hEdit, hList, hScroll, hStatic,  hListView ;
	static int cxChar, cxCaps, cyChar ;
	HDC hdc ;
	PAINTSTRUCT ps ;
	RECT rect ;
	TEXTMETRIC tm ;
	char* heading = "DayZ SA Online Map Information" ;
	char* url = "http:\/\/dayzdb.com\/map\/chernarusplus" ;
	char* cpSecondListHeader = "Entity Coordinates" ;
	char* cpFirstListHeader = "Local Player Coordinates" ;

	switch( uMsg )
    {
		case WM_CREATE:
		{
			hdc = GetDC( hwnd ) ;
			GetTextMetrics ( hdc,  &tm ) ;
			cxChar = tm.tmAveCharWidth ;
			cxCaps = ( tm.tmPitchAndFamily & 1 ? 3 : 2 ) * cxChar / 2 ;
			cyChar = tm.tmHeight + tm.tmExternalLeading ;			

			hListView = CreateListView( hwnd ) ;
			hListView2 = CreateListViewTwo ( hwnd ) ;

			//hButton = CreateWindowEx( NULL,
			//						  "Button",
			//						  "Button Example",
			//						  WS_BORDER | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			//						  10, 150,
			//						  150, 30,
			//						  hwnd, NULL,
			//						  NULL,
			//						  NULL ) ;

			//hCombo  = CreateWindowEx( NULL,
			//						  "ComboBox",
			//						  "darkblue",
			//						  WS_BORDER | WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
			//						  10, 170,
			//						  150, 100,
			//						  hwnd, NULL,
			//						  NULL,
			//						  NULL ) ;

			//hEdit   = CreateWindowEx( NULL,
			//						  "Edit",
			//						  "edit box example",
			//						  WS_BORDER | WS_CHILD | WS_VISIBLE,
			//						  10, 200,
			//						  150, 30,
			//						  hwnd, NULL,
			//						  NULL,
			//						  NULL ) ;

			ReleaseDC( hwnd, hdc ) ;
			break;
		}

		case WM_NOTIFY:
		{
			return ( NotifyHandler( hwnd, uMsg, wParam, lParam ) ) ;
			break ;
		}

		case WM_PAINT:
		{
			hdc = BeginPaint ( hwnd, &ps ) ;
			GetClientRect ( hwnd, &rect ) ;

			//DrawText ( hdc, TEXT ("Testing!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER ) ;
			SetTextColor( hdc, 0x00000067 ) ;
			SetBkMode( hdc, TRANSPARENT ) ;

			TextOut( hdc, 10, (cyChar * 0 ) + 10, heading, lstrlen( heading ) ) ;
			//ExtTextOut( hdc, 10, (cyChar * 0 ) + 10, ETO_OPAQUE, NULL, heading, lstrlen( heading ), NULL ) ;

			TextOut( hdc, 20, (cyChar * 1 ) + 10, url, lstrlen( url ) ) ;
			TextOut( hdc, 10, (cyChar * 2 ) + 20, cpFirstListHeader, lstrlen( cpFirstListHeader ) ) ;
			TextOut( hdc, 10, 141, cpSecondListHeader, lstrlen( cpSecondListHeader ) ) ;
			
			EndPaint ( hwnd, &ps ) ;
			return 0 ;
		}

		case WM_DESTROY:
		{
			gameover = true;
			PostQuitMessage ( 0 ) ;
			break ;
		}

		default:
			// Calls the default window procedure to provide default processing for any window messages that an application does not process.
			// Ensures that every message is processed.
			return DefWindowProc ( hwnd, uMsg, wParam, lParam ) ;
    }

	return 0;
}// --- End of WindowProc2 ---



void listBoxWindow()
{
	MSG messages;
	WNDCLASSEX wc = { sizeof ( WNDCLASSEX ), CS_CLASSDC, WindowProc2, 0, 0, GetModuleHandle(NULL), NULL, NULL, (HBRUSH)( GetStockObject ( GRAY_BRUSH ) ), NULL, "OLW", NULL } ;
	
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb775507(v=vs.85).aspx
	// INITCOMMONCONTROLSEX structure carries information used to load common control classes from the dynamic-link library (DLL).
	// This structure is used with the InitCommonControlsEx function
	// ICC_WIN95_CLASSES -- Load animate control, header, hot key, list-view, progress bar, status bar, tab, tooltip, toolbar, trackbar, tree-view, and up-down control classes.
	// InitCommonControlsEx() -- Ensures that the common control DLL(Comctl32.dll) is loaded, and registers specific common control
	// classes from the DLL. An application must call this function before creating a common control.
	// make sure to load Comctl32.lib

	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx( &icex );
	
   // api - RegisterClassEx - Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function. 
   // 1st arg - A pointer to a WNDCLASSEX structure. You must fill the structure with the appropriate class attributes before passing it to the function.
   // return type - If the function succeeds, the return value is a class atom that uniquely identifies the class being registered. This atom can 
   //               only be used by the CreateWindow, CreateWindowEx, GetClassInfo, GetClassInfoEx, FindWindow, FindWindowEx, and 
   //               UnregisterClass functions and the IActiveIMMap::FilterClientWindows method. 
	RegisterClassEx ( &wc ) ; 

	int desktopwidth  = GetSystemMetrics( SM_CXSCREEN ) ;
	int desktopheight = GetSystemMetrics( SM_CYSCREEN ) ;
	
	// create unowned window
	/*Sends WM_GETMINMAXINFO, WM_NCCREATE, WM_NCCALCSIZE, WM_CREATE */
	hwndListWindow = CreateWindow( "OLW",														//name: wnd 'class'
							  "DayZ Standalone Information",									//wnd title
							  WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUPWINDOW | WS_CAPTION,	//wnd style
							  desktopwidth / 4,													//position: left
							  desktopheight / 4,												//position: top
							  425,													            //width
							  400,												                //height
							  NULL,														        //parent wnd handle
							  0,																//menu handle/wnd id
							  wc.hInstance,													    //app instance
							  0 ) ;																//user defined info
	
	/* Sends WM_SHOWWINDOW, WM_WINDOWPOSCHANGING, WM_ACTIVATEAPP, WM_NCACTIVATE, */
	/* Sends WM_GETTEXT, WM_ACTIVATE,  WM_SETFOCUS, WM_NCPAINT, WM_ERASEBKGND, */
	/* WM_WINDOWPOSCHANGED, WM_SIZE, WM_MOVE */
	ShowWindow ( hwndListWindow, SW_NORMAL ) ;  
    UpdateWindow ( hwndListWindow ) ;          /* Sends a WM_PAINT message */

	//while ( GetMessage( &messages, NULL, 0, 0 ) )   // This is the loop that processes all the messages 
	//{
	//	TranslateMessage ( &messages ) ;
    //  DispatchMessage ( &messages ) ;
	//}
	
	//==========================================================================================================
	// Initialize the game
	while( !gameover )
	{
		if( PeekMessage( &messages, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage ( &messages ) ;
			DispatchMessage ( &messages ) ;
		}

		// Process New Game Loop		
		Game_Run( hwndListWindow );
	}

	// Shutdown Game Loop
	// Release all objects here.
	//==========================================================================================================
}



/****************************************************************************
* 
*    FUNCTION: CreateListViewTwo(HWND)
*
*    PURPOSE:  Creates the list view window and initializes it
*
****************************************************************************/
HWND CreateListViewTwo (HWND hWndParent)                                     
{	
	// ========================================================================================================================================================
	// ====================== LIST VIEW WINDOW CONTROL CREATION ===============================================================================================
	// ========================================================================================================================================================

	// Ensure that the common control DLL is loaded.
	InitCommonControls() ;

	// Create the list view window that starts out in report view and allows label editing.
	hWndList2 = CreateWindowEx(  0L,
								WC_LISTVIEW,                // list view class
								"",                         // no default text
								WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE,
								10, 70,
								400, 60,
								hWndParent,
								NULL,
								NULL,
								NULL ) ;

	if (hWndList2 == NULL )
		return NULL ;
	
	// ========================================================================================================================================================
	// ====================== HEADER TITLES OF EACH COLUMN ====================================================================================================
	// ========================================================================================================================================================

	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvC.fmt  = LVCFMT_LEFT ;  // text left align the column
	lvC.cx   = 75 ;           // width of the column, in pixels

	char * szTestText[] = { "Entity Index", "Entity Type", "X Coord", "Y Coord", "Z Coord" } ;	
	for( int index = 0; index <= 4; index++ )
	{	
		// Log( "szText: %s\n", szTestText[ index ] ) ;
		lvC.pszText = szTestText[ index ] ;
		SendMessage ( hWndList2, LVM_INSERTCOLUMN, index, (LPARAM)&lvC ) ;
	}
		
	// ========================================================================================================================================================
	// ====================== SET ITEMS IN EACH ROW ===========================================================================================================
	// ========================================================================================================================================================

	memset ( &lvI, 0, sizeof ( lvI ) ) ;  // Reset Item Struct

	lvI.mask       = LVIF_TEXT ;
	lvI.cchTextMax = 256 ; 
	lvI.iItem      = 0 ;
	lvI.iSubItem   = 0 ;
	lvI.pszText = "Default" ;
	//SendMessage ( hWndList, LVM_INSERTITEM, 0, (LPARAM)&lvI ) ;
	if( ListView_InsertItem ( hWndList2, &lvI ) == -1 )
		Log ( "ListView_Insert Failed\n" ) ;

	lvI.iSubItem = 1 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList2, LVM_SETITEM, 0, (LPARAM)&lvI ) ;

	lvI.iSubItem = 2 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList2, LVM_SETITEM, 0, (LPARAM)&lvI ) ;

	lvI.iSubItem = 3 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList2, LVM_SETITEM, 0, (LPARAM)&lvI ) ;

	lvI.iSubItem = 4 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList2, LVM_SETITEM, 0, (LPARAM)&lvI ) ;
	
	return( hWndList2 );
}


















/****************************************************************************
* 
*    FUNCTION: CreateListView(HWND)
*
*    PURPOSE:  Creates the list view window and initializes it
*
****************************************************************************/
HWND CreateListView (HWND hWndParent)                                     
{	
	// ========================================================================================================================================================
	// ====================== LIST VIEW WINDOW CONTROL CREATION ===============================================================================================
	// ========================================================================================================================================================

	// Ensure that the common control DLL is loaded.
	InitCommonControls() ;

	// Create the list view window that starts out in report view and allows label editing.
	hWndList = CreateWindowEx(  0L,
								WC_LISTVIEW,                // list view class
								"",                         // no default text
								WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE,
								10, 160,
								400, 200,
								hWndParent,
								NULL,
								NULL,
								NULL ) ;

	if (hWndList == NULL )
		return NULL ;
	
	// ========================================================================================================================================================
	// ====================== HEADER TITLES OF EACH COLUMN ====================================================================================================
	// ========================================================================================================================================================

	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvC.fmt  = LVCFMT_LEFT ;  // text left align the column
	lvC.cx   = 75 ;           // width of the column, in pixels

	char * szTestText[] = { "Entity Index", "Entity Type", "X Coord", "Y Coord", "Z Coord" } ;	
	for( int index = 0; index <= 4; index++ )
	{	
		// Log( "szText: %s\n", szTestText[ index ] ) ;
		lvC.pszText = szTestText[ index ] ;
		SendMessage ( hWndList, LVM_INSERTCOLUMN, index, (LPARAM)&lvC ) ;
	}
		
	// ========================================================================================================================================================
	// ====================== SET ITEMS IN EACH ROW ===========================================================================================================
	// ========================================================================================================================================================

	memset ( &lvI, 0, sizeof ( lvI ) ) ;  // Reset Item Struct

	lvI.mask       = LVIF_TEXT ;
	lvI.cchTextMax = 256 ; 
	lvI.iItem      = 0 ;
	lvI.iSubItem   = 0 ;
	lvI.pszText = "Default" ;
	//SendMessage ( hWndList, LVM_INSERTITEM, 0, (LPARAM)&lvI ) ;
	if( ListView_InsertItem ( hWndList, &lvI ) == -1 )
		Log ( "ListView_Insert Failed\n" ) ;

	lvI.iSubItem = 1 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;

	lvI.iSubItem = 2 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;

	lvI.iSubItem = 3 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;

	lvI.iSubItem = 4 ; 
	lvI.pszText  = "Default" ;
	SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;
	
	return( hWndList );
}





/****************************************************************************
* 
*    FUNCTION: NotifyHandler(HWND, UINT, UINT, LONG)
*
*    PURPOSE: This function is the handler for the WM_NOTIFY that is 
*    sent to the parent of the list view window.
*
****************************************************************************/
LRESULT NotifyHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//printf( "NotifyHandler\n\n" ) ;

	LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam ;
	NM_LISTVIEW *pNm   = (NM_LISTVIEW *)lParam ;
	HOUSEINFO *pHouse  = (HOUSEINFO *)( pLvdi->item.lParam ) ;
	static TCHAR szText[10] ;

	//printf( "lParam Test: %u\n", pLvdi->item.lParam ) ;

	//if( wParam != 1000 )
		//return 0L;

	//printf("pLvdi->hdr.code: %d\n\n", pLvdi->hdr.code );

	// Notification Code
	// List view notifications - http://msdn.microsoft.com/en-us/library/windows/desktop/ff485962(v=vs.85).aspx
	switch( pLvdi->hdr.code )
	{
		// # 01
		case LVN_GETDISPINFO:
			// iSubItem - One-based index of the subitem to which this structure refers, 
			//            or zero if this structure refers to an item rather than a subitem.
			switch( pLvdi->item.iSubItem )
			{
				case 0:     // Address
					//pLvdi->item.pszText = pHouse->szAddress;
					//sprintf( szText, "%u", pCords[0].x );
					//pLvdi->item.pszText = szText;
					break;

				case 1:     // City
					//pLvdi->item.pszText = pHouse->szCity;
					break;

				case 2:     // Price
					//sprintf( szText, "$%u", pHouse->iPrice );
					//pLvdi->item.pszText = szText;
					break;

				case 3:     // Number of bedrooms
					//sprintf( szText, "%u", pHouse->iBeds );
					//pLvdi->item.pszText = szText;
					break;

				case 4:     // Number of bathrooms
					//sprintf( szText, "%u", pHouse->iBaths );
					//pLvdi->item.pszText = szText;
					break;

				default:
					break;
			}
			break;

		// # 02
        case LVN_BEGINLABELEDIT:
            {
                //HWND hWndEdit;
                //// Get the handle to the edit box.
                //hWndEdit = (HWND)SendMessage( hWnd, LVM_GETEDITCONTROL, 0, 0 );
                //// Limit the amount of text that can be entered.
                //SendMessage( hWndEdit, EM_SETLIMITTEXT, (WPARAM)20, 0 );
            }
            break;

		// # 03
        case LVN_ENDLABELEDIT:
            // Save the new label information
       //     if( (pLvdi->item.iItem != -1) && (pLvdi->item.pszText != NULL) )
			    //lstrcpy( pHouse->szAddress, pLvdi->item.pszText );
            break;

		// # 04
		case LVN_COLUMNCLICK:
			// The user clicked on one of the column headings - sort by this column.
			//ListView_SortItems( pNm->hdr.hwndFrom, ListViewCompareProc, (LPARAM)( pNm->iSubItem ) );
			break;

		default:
			//printf("Inside Default: pLvdi->hdr.code\n\n");
			break;
	}
	return 0L;
}






/****************************************************************************
* 
*    FUNCTION: ListViewCompareProc(LPARAM, LPARAM, LPARAM)
*
*    PURPOSE: Callback function that sorts depending on the column click 
*
****************************************************************************/
int CALLBACK ListViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	HOUSEINFO *pHouse1 = (HOUSEINFO *)lParam1;
	HOUSEINFO *pHouse2 = (HOUSEINFO *)lParam2;
	LPSTR lpStr1, lpStr2;
	int iResult;


	if (pHouse1 && pHouse2)
	{
		switch( lParamSort)
		{
			case 0:     // sort by Address
				lpStr1 = pHouse1->szAddress;
				lpStr2 = pHouse2->szAddress;
				iResult = strcmpi(lpStr1, lpStr2);
				break;

			case 1:     // sort by City
				lpStr1 = pHouse1->szCity;
				lpStr2 = pHouse2->szCity;
				iResult = lstrcmpi(lpStr1, lpStr2);
				break;

			case 2:     // sort by Price
				iResult = pHouse1->iPrice - pHouse2->iPrice;
				break;

			case 3:     // sort by the number of bedrooms
				iResult = pHouse1->iBeds - pHouse2->iBeds;
				break;

			case 4:     // sort by the number of bathrooms
				iResult = pHouse1->iBaths - pHouse2->iBaths;
				break;

			default:
				iResult = 0;
				break;

		}

	}
	return(iResult);
}







void Game_Run ( HWND window )
{
	Sleep ( 100 ) ;
	
	char cListItemIndex [ MAX_PATH ] = {0} ;
	char cXCoord [ MAX_PATH ]        = {0} ;
	char cYCoord [ MAX_PATH ]        = {0} ;
	char cZCoord [ MAX_PATH ]        = {0} ;

	D3DXVECTOR3 temp ;
	int itemCount = 0 ;	
	int mapSize   = 0 ;
	char* cpEntityString ;

	lvI.mask       = LVIF_TEXT ;
	lvI.cchTextMax = 256 ; 
	lvI.iSubItem   = 0 ;
		
	mapSize = mapCoords.size() ;
	Log ( "Inside Game_Run() mapSize: %d\n", mapSize ) ;
	
	// Fill ListView Box
	itemCount = ListView_GetItemCount( hWndList ) ;
	Log ( "itemCount: %d\n", itemCount ) ;

	Log( "EnititySize: %d\n", EntitySize ) ;
	if ( itemCount > EntitySize )
	{
		ListView_DeleteAllItems( hWndList ) ;
	}

	if( itemCount < mapSize )
	{
		ListView_DeleteAllItems( hWndList ) ;

		for( int k = 0; k < mapSize; k++ )
		{
			// Index					
			lvI.mask       = LVIF_TEXT ;
			lvI.cchTextMax = 256 ; 
			lvI.iItem      = k ;
			lvI.iSubItem   = 0 ;			
			sprintf ( cListItemIndex, "%d",  k ) ;
			//printf ( "cListItemIndex: %s\n", cListItemIndex ) ;
			lvI.pszText = cListItemIndex ;
			//SendMessage ( hWndList, LVM_INSERTITEM, 0, (LPARAM)&lvI ) ;
			if( ListView_InsertItem ( hWndList, &lvI ) == -1 )
				printf ( "ListView_Insert Failed\n" ) ;					
		}
	}

	// Update Listview Box
	if ( itemCount == mapSize )
	{
		for( int k = 0; k < mapSize; k++ )
		{
			temp           = mapCoords[ k ] ;
			cpEntityString = mapEntityName[ k ] ;
			
			lvI.mask       = LVIF_TEXT ;
			lvI.cchTextMax = 256 ; 
			lvI.iItem      = k ;

			// Entity Column
			lvI.iSubItem   = 1 ;
			lvI.pszText = cpEntityString ;		
			SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;
						
			// X Column
			lvI.iSubItem   = 2 ;
			sprintf ( cXCoord, "%7.3f",  temp.x ) ;
			//printf ( "cXCoord: %s\n", cXCoord ) ;
			lvI.pszText = cXCoord ;		
			SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;
			
			// Y Column
			lvI.iSubItem   = 3 ;
			sprintf ( cYCoord, "%7.3f",  temp.y ) ;
			//printf ( "cYCoord: %s\n", cYCoord ) ;
			lvI.pszText = cYCoord ;		
			SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;
		
			// Z Column
			lvI.iSubItem   = 4 ;
			sprintf ( cZCoord, "%7.3f",  temp.z ) ;
			//printf ( "cZCoord: %s\n", cZCoord ) ;
			lvI.pszText = cZCoord ;		
			SendMessage ( hWndList, LVM_SETITEM, 0, (LPARAM)&lvI ) ;	
		}	
	}


} // End of Game_Run()
