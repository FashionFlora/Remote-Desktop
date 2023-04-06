#define HEAD_MARK "TEA"
#define CRT_SECURE_NO_WARNINGS
#include "includes.h"
#include <string>
#define MAX_RECV_BUF 120000



class ICreateRemoteDesktop
{

private:

	enum MSGTYPE
	{
		MSGTYPE_CMD,	
		MSGTYPE_IMG,	
		MSGTYPE_LAN,	
		MSGTYPE_WP,		
		MSGTYPE_OK,		
		MSGTYPE_BYE		
	};

	typedef struct _LanMsgHeader
	{
		unsigned int nPackageSize;	
		unsigned int nHeaderSize;	
		unsigned int nDataSize;		
		unsigned int nImageSize;	
		MSGTYPE MsgType;			
	}LanMsgHeader;

	SOCKET client;
	int realsiz;
	BYTE m_byImageData[MAX_RECV_BUF];
	char buffer[45000];
	char buffer2[45000];
	char  arr[90000];
	char sizearr[100];
	char m_szRecvBufClient[MAX_RECV_BUF];
	size_t m_nImageSize;


	size_t m_nCurrentSize = 0;

	
public:


	HWND currproc =0;

	HWND window123 = 0;
	bool init_server(HWND hwnd);
	 LVITEM LvItem_p = { 0 };
	 int PutOnStream_p = 0;
	 std::string commandlist_p;
	 HWND infoLog_p;
	  HWND hList_p;


	int width = 0;
	int height = 0;
	int x = 0;
	int y = 0;

	int ProcessClientData(SOCKET socket, const char* lpszData, int nLen);
	void AddText1(HWND edit, LPCTSTR Text);

	void on_client_connect();
	void ProcessImageData(SOCKET  m_socketClient);
	void  printimage(BYTE array[MAX_RECV_BUF], int size , int x , int y , int width ,  int height);
	bool GetClientIsEnd();





};
