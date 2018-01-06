#include <Windows.h>
#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MessageBox(NULL, TEXT("Hello World"), TEXT("App"), MB_OKCANCEL);//MB_OK,MB_ABORTRETRYIGNOR...
	return 0;
}
//헝가리안 표기법
//lpCmdLine : 프로그램을 이름을 제외한 응용 프로그램의 명령줄. lp:long pointer, ln:long int
