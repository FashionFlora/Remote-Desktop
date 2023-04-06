//#include "Networking.h"
#include "Includes.h"
#include <windowsx.h>
#include "Networking.h"
#include "MainDlg.h"
#include <	uxtheme.h>
#pragma comment (lib ,"UxTheme.lib")
LRESULT CALLBACK MainDlgProcedure(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow2(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindow3(HWND & handleforwindow1 , HINSTANCE& hInst, int nShowCmd);

LRESULT CALLBACK ProcedureForThird(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HINSTANCE  hInst1;
#define createwindowbuttoninwindow1 101
#define createwindowbuttoninwindow2 201
#define createwindowbuttoninwindow3 301
#define ID_BUTTON_RD 1
#define   IDB_ANIMMAIN_1                501 

bool window1open, window2open, window3open, window4open = false;
bool windowclass1registeredbefore, windowclass2registeredbefore,
windowclass3registeredbefore, windowclass4registeredbefore = false;

enum windowtoopenenumt { none, window2, window3, window4 };

windowtoopenenumt windowtoopenenum = none;



HWND  Connect;
HWND  Disconnect;
HWND infoLog;
void createwindow2(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd);



ICreateRemoteDesktop sesion1;




HMENU hMenu;



HINSTANCE kurwa12;
HINSTANCE siemano(HINSTANCE& inst, int p)
{

    if (p == 1)
    {

        kurwa12 = inst;
    }

    if (p == 0)
    {

        return kurwa12;
    }


    return 0;
}


VOID APIENTRY HandlePopupMenu(HWND &hwnd, POINT pt)
{
    HMENU hmenu;            // menu template          
    HMENU hmenuTrackPopup;  // shortcut menu   



    hmenu = LoadMenu(hInst1, "PopUp");
    if (hmenu == NULL)
        return;

    hmenuTrackPopup = GetSubMenu(hmenu, 0);

    ClientToScreen(hwnd, (LPPOINT)&pt);

    // Draw and track the shortcut menu.  

    TrackPopupMenu(hmenuTrackPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON,
        pt.x, pt.y, 0, hwnd, NULL);

    // Destroy the menu. 

    DestroyMenu(hmenu);
}


void __cdecl ThreadProc(void* Args)
{

 
    sesion1.init_server(sesion1.currproc);

    _endthread();
}




void GDIPLUS(HDC hdc, int drvIndex = -1) {
    Graphics graphics(hdc);
    const wchar_t* text = L"P-C Name: ";
    const wchar_t* author = L"IP Addy: ";
    const wchar_t* text1 = L"Windows: ";
    const wchar_t* author2 = L"AntiVs: ";

    FontFamily family(L"Verdana");
    Font font(&family, 15, FontStyleRegular, UnitPixel);


  
    SolidBrush bBrush(Color(255, 196, 196, 196));
    // Fill the rectangle.
    graphics.FillRectangle(&bBrush, 20, 20, 1200, 50);

    SolidBrush sbrush(Color::Black);
    graphics.DrawString(text, wcslen(text), &font, PointF(250, 20), &sbrush);
    graphics.DrawString(author, wcslen(author), &font, PointF(500, 20), &sbrush);
    graphics.DrawString(text1, wcslen(text), &font, PointF(750, 20), &sbrush);
    graphics.DrawString(author2, wcslen(author), &font, PointF(1000, 20), &sbrush);

}







HWND CreateButton(LPCSTR lpButtonName, HWND hWnd, int x, int y) {
    return CreateWindow("button", lpButtonName, WS_EX_TRANSPARENT | BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, x, y, 150, 50, hWnd, (HMENU)ID_BUTTON_RD, (HINSTANCE)hWnd, 0);
}


int WINAPI MainDlg::Initialize(HINSTANCE hInst, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int   nShowCmd)
{

    AllocConsole();
    freopen("CONOUT$", "w", stdout);


    siemano(hInst, 1);

    std::cout << hInst << "   1 " << std::endl;
    bool endprogram = false;
    WNDCLASSEX windowclassforwindow2;
    HWND handleforwindow2;

   


    
    MSG msg;
    WNDCLASSEX windowclassforwindow1;
    ZeroMemory(&windowclassforwindow1, sizeof(WNDCLASSEX));
    windowclassforwindow1.cbClsExtra = NULL;
    windowclassforwindow1.cbSize = sizeof(WNDCLASSEX);
    windowclassforwindow1.cbWndExtra = NULL;
    windowclassforwindow1.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowclassforwindow1.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowclassforwindow1.hIcon = NULL;
    windowclassforwindow1.hIconSm = NULL;
    windowclassforwindow1.hInstance = hInst;
    windowclassforwindow1.lpfnWndProc = (WNDPROC)MainDlgProcedure;
    windowclassforwindow1.lpszClassName = "window class 1";
    windowclassforwindow1.lpszMenuName = NULL;
    windowclassforwindow1.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&windowclassforwindow1))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            "Window class creation failed",
            "Window Class Failed",
            MB_ICONERROR);
    }

    HWND handleforwindow1 = CreateWindowEx(NULL,
        windowclassforwindow1.lpszClassName,
        "Window 1",
        WS_OVERLAPPEDWINDOW,
        200,
        150,
        1280,
        720,
        NULL,
        NULL,
        hInst,
        NULL                /* No Window Creation data */
    );

    if (!handleforwindow1)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            "Window creation failed",
            "Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(handleforwindow1, nShowCmd);
    bool endloop = false;
    while (endloop == false) {
        if (GetMessage(&msg, NULL, 0, 0));
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (windowtoopenenum != none) {
            switch (windowtoopenenum) {
            case window2:
                if (window2open == false) {
                    createwindow2(windowclassforwindow2, handleforwindow2, hInst, nShowCmd);
                }
                break;
            case window3:
                {
                    windowprocessforwindow3(handleforwindow2, hInst, nShowCmd);
                }
                break;
            }
            windowtoopenenum = none;
        }
        if (window1open == false && window2open == false )
            endloop = true;

    }



    return 0;
}


void AddText(HWND edit, LPCTSTR Text)
{
    int len = GetWindowTextLength(edit);
    SendMessage(edit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
    SendMessage(edit, EM_REPLACESEL, 0, (LPARAM)Text);
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
HBITMAP hBitmap = NULL;

LRESULT CALLBACK  MainDlgProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {


    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    CoInitialize(0);
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;    // client area             
    POINT pt;   // location of mouse click
    
    switch (message) {


    case WM_PAINT:
    {

        hdc = BeginPaint(hwnd, &ps);
        // TODO:  在此添加任意绘图代码...

        HINSTANCE halo=0;
        HINSTANCE hInst = siemano(halo, 0);
        HBITMAP hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        HDC hMemDc = CreateCompatibleDC(hdc);
        SelectObject(hMemDc, hBitMap);
        BitBlt(hdc, 0,40, 1280, 720, hMemDc, 20, 0, SRCCOPY);
        DeleteDC(hMemDc);
        DeleteObject(hBitMap);
        //DrawUserInfo();
        EndPaint(hwnd, &ps);







       /* PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);


        HBITMAP hBitMap = LoadBitmap(MainDlg::hInstance1, MAKEINTRESOURCE(IDB_BITMAP2));
        HDC hMemDc = CreateCompatibleDC(hdc);
        SelectObject(hMemDc, hBitMap);
        BitBlt(hdc, 0, 0, 400, 400, hMemDc, 0, 0, SRCCOPY);
        DeleteDC(hMemDc);
        DeleteObject(hBitMap);




        GDIPLUS(hdc);

        EndPaint(hwnd, &ps);
        */
    }
    break;




    case WM_RBUTTONDOWN:

        // Get the bounding rectangle of the client area. 

        GetClientRect(hwnd, (LPRECT)&rc);

        // Get the client coordinates for the mouse click.  

        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);

        // If the mouse click took place inside the client 
        // area, execute the application-defined function 
        // that displays the shortcut menu. 

        if (PtInRect((LPRECT)&rc, pt))
            HandlePopupMenu(hwnd, pt);
        break;

    case WM_MENUCOMMAND:
    {

        std::cout << "yea boy " << std::endl;

        HMENU menu = (HMENU)lParam;
        int idx = wParam;

        MessageBox(hwnd, "Rd boys ", "Error", MB_OK | MB_ICONINFORMATION);



    }
    break;










    case WM_CREATE:




        SetWindowTheme(hwnd, L"Explorer", NULL);
        window1open = true;
        CreateWindowEx(NULL,
            "BUTTON",
            "Open Remote Desktop",
            WS_TABSTOP | WS_VISIBLE |
            WS_CHILD | BS_DEFPUSHBUTTON,
            20,
            20,
            150,
            50,
            hwnd,
            (HMENU)createwindowbuttoninwindow1,
            GetModuleHandle(NULL),
            NULL);
        break;
    case WM_DESTROY:
        window1open = false;
        break;
    case WM_COMMAND:
        std::cout << "yea boy " << std::endl;
        switch LOWORD(wParam) {
        case createwindowbuttoninwindow1:
            windowtoopenenum = window2;
            break;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);

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
            AddText(infoLog, " Beggining thread... \n");
            sesion1.currproc = infoLog;




            _beginthread(ThreadProc,0,0);
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

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;


    }
    return (INT_PTR)FALSE;
}


LRESULT CALLBACK windowprocessforwindow3(HWND& handleforwindow1, HINSTANCE& hInst ,int nShowCmd)
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

    HWND handleforwindow2 = CreateWindowEx(NULL,
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


    sesion1.window123 = handleforwindow2;

    if (!handleforwindow2)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            "Window creation failed",
            "Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(handleforwindow2, nShowCmd);
    SetParent(handleforwindow2, handleforwindow1);

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

