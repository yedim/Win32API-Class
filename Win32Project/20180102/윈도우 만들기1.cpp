#include <Windows.h>
#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL, TEXT("Hello World"), TEXT("App"), MB_OKCANCEL);//MB_OK,MB_ABORTRETRYIGNOR...
	return 0;
}
//�밡���� ǥ���
//lpCmdLine : ���α׷��� �̸��� ������ ���� ���α׷��� �����. lp:long pointer, ln:long int
