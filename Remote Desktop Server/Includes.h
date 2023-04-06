#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define CRT_SECURE_NO_WARNINGS
#ifndef CRT_SECURE_NO_WARNINGS
#define CRT_SECURE_NO_WARNINGS
#endif


#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <ObjIdl.h>
#include <atlimage.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <thread>
#include <gdiplus.h>
#include <future>
#include <commctrl.h>
#include <Windows.h>
#include <windowsx.h>
#include <Uxtheme.h>
#include <string>
#include "CxImage\include\ximage.h"
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib , "UxTheme.lib")


#pragma comment(lib, "CxImage/lib/cximage.lib")


using namespace Gdiplus;
using namespace std;