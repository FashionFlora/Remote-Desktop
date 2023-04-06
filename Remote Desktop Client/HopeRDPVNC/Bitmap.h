#include "includes.h"


class ICreateBitmap123
{

private:

	HDC hdc;
	HDC hScreenDC;
	HDC hMemoryDC;
	int width;
	int height;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	HDC hdc_x;

public:


	HBITMAP CreateBmp();
	std::vector<BYTE> Bmptobuf(HBITMAP bmp);
	bool save_to_file(char* name , std::vector<BYTE>);
	HDC printBmp(HDC bmp);
	HDC  operator  =(HDC bmp);




};