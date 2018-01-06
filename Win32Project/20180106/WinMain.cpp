#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;//Def
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDC_ICON);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("App");

	RegisterClass(&wc);

	HWND hWnd;
	
	hWnd = CreateWindow(TEXT("App"), TEXT("Sample Application"),//TEXT("__")은 unicode로 변환. "App"를 반복사용하는 이유는 어떤 프로그램을 사용한다는 뜻.
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //그림을 그릴 수 있는 영역
	PAINTSTRUCT ps;
	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, _T("Test"), _tcslen(_T("Test")));
		TextOut(hdc, 200, 200, _T("Yerim"), _tcslen(_T("Yerim")));//_tsclen 문자열개수
		TextOut(hdc, 800, 400, _T("Lee"), _tcslen(_T("Lee")));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(hWnd, _T("LButton Down"), _T("Msg"), MB_OK);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);//화면에 대한 모든 기능 시행. 그러나 행위는 동작.
}