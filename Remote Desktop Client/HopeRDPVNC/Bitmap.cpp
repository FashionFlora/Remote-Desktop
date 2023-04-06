#include "includes.h"
#include "Bitmap.h"


HBITMAP ICreateBitmap123::CreateBmp()
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
	return hbitmap;
}
std::vector<BYTE> ICreateBitmap123::Bmptobuf(HBITMAP bmp)
{
	std::vector<BYTE> buf;
	IStream* stream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
	CImage image;
	ULARGE_INTEGER liSize;
	image.Attach(bmp);
	image.Save(stream, Gdiplus::ImageFormatJPEG);
	IStream_Size(stream, &liSize);
	DWORD len = liSize.LowPart;
	IStream_Reset(stream);
	buf.resize(len);
	IStream_Read(stream, &buf[0], len);
	stream->Release();
	return buf;
}
bool ICreateBitmap123::save_to_file(char* name, std::vector<BYTE> buf)
{

	std::fstream fi;
	fi.open(name, std::fstream::binary | std::fstream::out);
	fi.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(BYTE));
	fi.close();

	return true;
}

HDC  ICreateBitmap123:: operator=(HDC bmp)
{
	return 0;
}

