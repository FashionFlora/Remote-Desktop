#include "Includes.h"
#include "Networking.h"
using namespace Gdiplus;
using namespace std;



void ICreateRemoteDesktop::AddText1(HWND edit, LPCTSTR Text)
{
	int len = GetWindowTextLength(edit);
	SendMessage(edit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
	SendMessage(edit, EM_REPLACESEL, 0, (LPARAM)Text);
}



void  ICreateRemoteDesktop::printimage(BYTE array[MAX_RECV_BUF], int size , int x, int y, int width, int height )
{

	CxImage px((BYTE*)array, size, CXIMAGE_FORMAT_JPG);


	HDC hdc;
	HDC hMemDc;
	HBITMAP hMemBmp;
	HBITMAP hOldBmp;
	//int width = GetSystemMetrics(SM_CXSCREEN);
	//int height = GetSystemMetrics(SM_CYSCREEN);


	hdc = GetDC(window123);
	hMemDc = CreateCompatibleDC(hdc);
	hMemBmp = CreateCompatibleBitmap(hdc, 1152, 860);
	hOldBmp = (HBITMAP)SelectObject(hMemDc, hMemBmp);
	px.Draw(hMemDc);


	int m_nRemoteScreenWidth = 1152;
	int m_nRemoteScreenHeight = 860;
	RECT rect;
	GetClientRect(window123, &rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	int x2 = rect.left;
	int y2 = rect.top;
	int width2 = rect.right - rect.left;
	int height2 = rect.bottom - rect.top;
	int m_nDrawOriginX = m_nRemoteScreenWidth < w ? (w - m_nRemoteScreenWidth) / 2 : 0;
	int m_nDrawOriginY = m_nRemoteScreenHeight < h ? (h - m_nRemoteScreenHeight) / 2 : 0;

	SetStretchBltMode(hdc, HALFTONE);
	StretchBlt(hdc,x2, y2, width2, height2, hMemDc , m_nDrawOriginX, m_nDrawOriginY, m_nRemoteScreenWidth, m_nRemoteScreenHeight,  SRCCOPY);


	//StretchBlt(hdc, 40, 40, 1200, 720, hMemDc, 0, 0, height ,width, SRCCOPY);
	SelectObject(hMemDc, hOldBmp);


	DeleteDC(hMemDc);
	DeleteObject(hMemBmp);
	DeleteObject(hOldBmp);

}
bool ICreateRemoteDesktop::GetClientIsEnd()
{

	return false;
}
void ICreateRemoteDesktop::ProcessImageData(SOCKET  m_socketClient)
{
	static bool bIsShouldExit = false;
	const size_t nHeadMarkSize = sizeof(HEAD_MARK);
	char szTmpDataBuf[nHeadMarkSize];
	char szHeadMark[nHeadMarkSize];
	int nLen = recv(m_socketClient, szTmpDataBuf, nHeadMarkSize, 0);
	if (nLen == SOCKET_ERROR)
	{
		return;
	}
	if (nLen == SOCKET_ERROR)
	{

		return;
	}
	memcpy_s(szHeadMark, sizeof(szHeadMark), szTmpDataBuf, nHeadMarkSize);

	if (strcmp(szHeadMark, HEAD_MARK) == 0)
	{


		while (GetClientIsEnd() == false)
		{
			int nRealRecv = 0;
			int nHeaderSize = sizeof(LanMsgHeader);
			char szHeaderBuf[sizeof(LanMsgHeader) + 1];

			while (GetClientIsEnd() == false)
			{


				int nRecvLen = recv(m_socketClient, m_szRecvBufClient, nHeaderSize - nRealRecv, 0);

				if (nRecvLen == SOCKET_ERROR)
				{

					std::cout << "third";

					return;
				}
				memcpy_s(szHeaderBuf + nRealRecv, nHeaderSize, m_szRecvBufClient, nRecvLen);
				nRealRecv += nRecvLen;
				if (nRealRecv == nHeaderSize)
				{

					nRealRecv = 0;
					break;
				}
			}
			LanMsgHeader tmpLanHeader;
			memcpy_s(&tmpLanHeader, nHeaderSize, szHeaderBuf, nHeaderSize);
			m_nImageSize = tmpLanHeader.nImageSize;
			std::cout << m_nImageSize << endl;
			memset(m_szRecvBufClient, 0, sizeof(m_szRecvBufClient));
			nRealRecv = 0;
			while (GetClientIsEnd() == false)
			{

				int nRecvLen = recv(m_socketClient, m_szRecvBufClient, m_nImageSize - nRealRecv, 0);
				memcpy_s(m_byImageData + m_nCurrentSize, MAX_RECV_BUF - m_nCurrentSize, m_szRecvBufClient, nRecvLen);
				m_nCurrentSize += nRecvLen;
				nRealRecv += nRecvLen;
				if (nRealRecv == m_nImageSize)
				{

					break;
				}
			}


			ICreateRemoteDesktop::printimage(m_byImageData, m_nImageSize, x,y,width,height);
			m_nCurrentSize = 0;

			// 得到下张图片的标识头
			memset(m_szRecvBufClient, 0, sizeof(m_szRecvBufClient));
			nRealRecv = 0;
			while (GetClientIsEnd() == false)
			{
				int nRecvLen = recv(m_socketClient, m_szRecvBufClient, nHeadMarkSize - nRealRecv, 0);
				if (nRecvLen == SOCKET_ERROR)
				{
					std::cout << "five";
					return;
				}
				memcpy_s(szHeadMark + nRealRecv, nHeadMarkSize, m_szRecvBufClient, nRecvLen);
				nRealRecv += nRecvLen;
				if (nRealRecv == nHeadMarkSize)
				{
					nRealRecv = 0;
					if (strcmp(szHeadMark, HEAD_MARK) == 0)
					{
						break;
					}
				}
			}
		}
		return;
	}
}
void ICreateRemoteDesktop::on_client_connect()
{


	while (true)
	{
		char buffer[512];
		memset(buffer, 0x0, sizeof(buffer) / sizeof(char));
		recv(client, buffer, sizeof(buffer) / sizeof(char), 0);
		string MainInfo = buffer;
		size_t found = MainInfo.find("192.168");
		string code = "999";

		int subitemcounter =1;
		
		if (found != string::npos)
		{
			int p = 1;

			std::cout << MainInfo << std::endl;
			int  entry[15];
			int s = 0;
			memset(entry, 0x0, sizeof(entry) / sizeof(int));
			int temporary = 0;
			std::cout << MainInfo.length() << std::endl;
			for (int i = 0; i < MainInfo.length(); i++)
			{


				if (MainInfo[i] == code[0] && MainInfo[i + 1] == code[1] && MainInfo[i + 2] == code[2])
				{
					string currentdata=  MainInfo.substr(entry[p - 1], i - entry[p - 1]);
					LPSTR LPSTRdata = const_cast<char*>(currentdata.c_str());


					
					if (temporary == 0)
					{
						
						LvItem_p.mask = LVIF_TEXT | LVCFMT_CENTER;   // Text Style
						LvItem_p.cchTextMax = 256; // Max size of test

						LvItem_p.iItem = 0;          // choose item  
						LvItem_p.iSubItem = 0;       // Put in first coluom
						LvItem_p.pszText = LPSTRdata; // Text to display (can be from a char variable) (Items)
		

						SendMessage(hList_p, LVM_INSERTITEM, 0, (LPARAM)&LvItem_p); // Send to the Listview
						temporary = 1;
					}
					else
					{

						LvItem_p.iSubItem = subitemcounter;
						//sprintf(Temp, "SubItem %d", i);
						LvItem_p.pszText = LPSTRdata;
						SendMessage(hList_p, LVM_SETITEM, 0, (LPARAM)&LvItem_p); // Enter text to SubItems
						subitemcounter++;

					}

					entry[p] = i + 3;
					p++;


				}
			}
			while (true)
			{


				while (true)
				{

					string temp = commandlist_p;
					 size_t  command = temp.find("remotedesktop");

					 

					 if (command != std::string::npos)
					 {
						 send(client, reinterpret_cast<const char*>(&temp[0]), temp.length(),0);
						 temp.clear();

						 Sleep(10);
						 while (true)
						 {

							 std::cout << "reciving" << endl;

							 

							 ICreateRemoteDesktop::ProcessImageData(client);
							 Sleep(2);

						 }
					 }
					 command = 0;
					 temp.clear();
					 Sleep(2);
				}

			}
		}

	}

	
}
bool ICreateRemoteDesktop::init_server(HWND hwnd)
{
	WSADATA wsa_data;
	SOCKADDR_IN server_addr, client_addr;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
	const auto server = socket(AF_INET, SOCK_STREAM, 0);


	AddText1(infoLog_p, " Initialize Winsock Struct...\n");
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5555);
	::bind(server, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr));
	listen(server, 0);

	Sleep(100);

	AddText1(infoLog_p, "\n Listening for incoming connections...\n");

	/*

	char Temp[255];
	LvItem_p.mask=LVIF_TEXT | LVCFMT_CENTER;   // Text Style
	LvItem_p.cchTextMax = 256; // Max size of test

	LvItem_p.iItem=0;
	LvItem_p.iSubItem=0;
	LvItem_p.pszText="Item 0";

	SendMessage(hList_p,LVM_INSERTITEM,0,(LPARAM)&LvItem_p); // Send to the Listview

	for(int i=0;i<=7;i++) // Add SubItems in a loop
	{
		LvItem_p.iSubItem=i;
		sprintf(Temp,"SubItem %d",i);
		LvItem_p.pszText=Temp;
		SendMessage(hList_p,LVM_SETITEM,0,(LPARAM)&LvItem_p); // Enter text to SubItems
	}
	*/




	int client_addr_size = sizeof(client_addr);

	std::cout << "waiting for accept" << std::endl;
	for (;;)
	{

		
		if ((client = accept(server, reinterpret_cast<SOCKADDR*>(&client_addr), &client_addr_size)) != INVALID_SOCKET)
		{
			
			for (;;)
			{
				std::cout << "accepted" << endl;
				ICreateRemoteDesktop::on_client_connect();
			}

		}

		const auto last_error = WSAGetLastError();

		if (last_error > 0)
		{
			cout << "Error: " << last_error << endl;
			break;
		}
	}
	return true;
}