#include "includes.h"

class ICreateNetwork123
{

private:
	

	struct sockaddr_in serv_addr;
	PCSTR addy = "127.0.0.1";
	int port = 8080;
	int  server = socket(AF_INET, SOCK_STREAM, 0);
public:


	bool initsock();
	ICreateNetwork123();
 	bool sendPacket(const char FAR* buf , size_t size);



};