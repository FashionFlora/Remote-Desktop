#include "Bitmap.h"
#include "Networking.h"
using namespace std;

void compress_str(string str)
{


	int j = 0;
	for (int i = 0; i < str.length(); i++) {

		int count = 0;
		while (i < str.length() - 1 && str[i] == str[i + 1]) {
			count++;
			i++;
		}

		count == 0 ? cout << str[i] : cout << str[i] << count;
	}
}



int main()
{

   

   



	

	HDC hdc = GetDC(0);
	POINT p;
	GetCursorPos(&p);
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, width, height);
	HGDIOBJ oldbmp = SelectObject(memdc, hbitmap);
	BitBlt(memdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY | CAPTUREBLT);
	SelectObject(memdc, oldbmp);
	DeleteDC(memdc);
		 vector<BYTE> buf;
		 IStream* stream = NULL;
		 HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
		 CImage image;
		 ULARGE_INTEGER liSize;

		//zapisanie screena w vectorze
		 image.Attach(hbitmap);
		 image.Save(stream, Gdiplus::ImageFormatJPEG);
		 IStream_Size(stream, &liSize);
		 DWORD len = liSize.LowPart;
		 IStream_Reset(stream);
		 buf.resize(len);
		 IStream_Read(stream, &buf[0], len);
		 stream->Release();


		 int c = 200000 - len;

		 std::cout << c + len;

		// buf.resize(c + len);
		
			
		 fstream fi;
		 fi.open("SomePng1.png", fstream::binary | fstream::out);
		 fi.write(reinterpret_cast<const char*>(&buf[0]), buf.size());
		 fi.close();
		 ICreateNetwork123 po;
		 po.initsock();

		 

		//set.printBmp(bmp);
		 _getch();


	return 0;
}