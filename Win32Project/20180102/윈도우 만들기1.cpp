#include <Windows.h>
#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL, TEXT("Hello World"), TEXT("App"), MB_OK);
	return 0;
}