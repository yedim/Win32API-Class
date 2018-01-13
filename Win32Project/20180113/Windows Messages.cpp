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
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[256];//[프로젝트]-[속성]-[구성속성]-[일반]-[문자집합]-멀티바이트
	int len;
	static int x = 100;
	static int y = 100;

	static RECT rt;
	static BOOL bnowDraw = FALSE;

	switch (iMsg) {
	case WM_CREATE://윈도우 생성되면 크기와 위치 변경
		MoveWindow(hWnd, 200, 100, 500, 500, FALSE);
		return 0;

	case WM_CHAR://WM_CHAR로 전달되는 문자들을 문자열 버퍼에 모아 출력
		len = strlen(str);
		str[len] = (TCHAR)wParam;//wParam으로 전달된 키 코드를 str 문자배열에 저장.
		str[len + 1] = 0;//문자열의 제일 끝 부분에 wParam값을 써 넣고 바로 뒤쪽의 0을 써 넣어 문자열 끝을 표시.
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
			MessageBox(hWnd, "First Button Clicked", "Button", MB_OK);
			break;
		case 1:
			MessageBox(hWnd, "Second Button Clicked", "Button", MB_OK);
			break;
		}
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			x -= 8;
			break;
		case VK_RIGHT:
			x += 8;
			break;
		case VK_UP:
			y -= 8;
			break;
		case VK_DOWN:
			y += 8;
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);//화면을 다시 그리기 위해 호출. TRUE면 다시 그리고, FALSE면 남아있다.
		return 0;

	case WM_LBUTTONDOWN://마우스 버튼을 누를 때의 좌표를 x,y에 기억시켜 놓고 마우스를 캡처.이 상태에서 마우스를 움직이면 이동 거리만큼 직선 그림
		x = (int)(short)LOWORD(lParam);
		y = (int)(short)HIWORD(lParam);
		bnowDraw = TRUE;
		SetCapture(hWnd);
		return 0;
	case WM_MOUSEMOVE://마우스로 자유 곡선
		if (bnowDraw == TRUE) {
			hdc = GetDC(hWnd);
			MoveToEx(hdc, x, y, NULL);
			x = (int)(short)LOWORD(lParam);
			y = (int)(short)HIWORD(lParam);
			LineTo(hdc, x, y);
			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bnowDraw = FALSE;
		ReleaseCapture();
		return 0;

	case WM_SIZE: //윈도우 크기가 변경될 때마다 작업 영역의 크기를 다시 계산하고 화면을 다시 그리므로 문자열은 윈도우의 크기에 상관없이 항상 같은 위치
		GetClientRect(hWnd, &rt);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, str, strlen(str));
		TextOut(hdc, x, y, "A", 1);
		SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc, rt.right / 2, rt.bottom / 2, "WM_SIZE", 7);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
/*
＊WM_CHAR 
키보드로부터 문자입력 받을 때
키보드로부터 입력이 발생했을 경우 윈도우즈는 포커스를 가진 프로그램에게 키보드 메시지(WM_CHAR)를 보내주며 프로그램은 이 메시지를 받아 키보드 입력을 처리.
입력된 문자의 아스키 코드를 wParam으로 전달하도록 되어 있으며 우리는 wParam의 값을 읽어 사용자가 어떤 키를 눌렀는지를 알아내게 된다

＊WM_COMMAND
프로그램 실행중에 사용자가 메뉴 항목을 선택할 때 발생.
각종 컨트롤로부터 값이 전달되며 각 컨트롤은 다양한 통지 메시지를 보내므로 이 메시지는 일반적으로 다음과 같은 이중 switch문으로 작성된다.

＊WM_CREATE
CreateWindow(Ex) 함수에 의해 윈도우가 생성될 때 보내진다. 
메모리에 윈도우를 생성한 후 화면에 보이기 전에 보내지며 주로 윈도우에 관련된 초기화 작업을 할 때 사용된다. 
윈도우 동작을 위한 메모리 할당, 리소스 생성, 차일드 컨트롤 생성, 윈도우 속성 초기화 작업에 이 메시지가 사용된다
윈도우를 성공적으로 생성했으면 0을 리턴

＊WM_DESTROY
윈도우가 파괴될 때 이 메시지가 전달
DefWindowProc은 DestroyWindow 함수를 호출하여 윈도우를 파괴
윈도우가 메인 윈도우일 경우 PostQuitMessage 함수를 반드시 호출하여 프로세스의 메시지 루프를 종료하도록

＊WM_LBUTTONDOWN
마우스 왼쪽 버튼을 누를 때
＊WM_LBUTTONUP
마우스 왼쪽 버튼을 놓을 때

＊WM_MOUSEMOVE
마우스가 움직일 때
마우스가 계속 작업 영역 위에서 움직이고 있으면 이 메시지는 반복적으로 계속 전달
MK_CONTROL
MK_LBUTTON
MK_MBUTTON
MK_RBUTTON
MK_SHIFT

＊WM_PAINT
윈도우의 작업 영역중 일부가 무효화되어 있을 때
무효 영역이 설정되며 이때마다 WM_PAINT메시지가 전달된다.
① 윈도우가 처음 생성되었을 때
② 윈도우의 위치가 이동되었을 때
③ 윈도우의 크기가 변경되었을 때. 최대, 최소화되었을 때
④ 다른 윈도우에 가려져 있다가 드러날 때
⑤ 스크롤 될 때
응용 프로그램 내부에서 InvalidateRect 함수 호출에 의해 강제로 무효 영역을 설정

WM_PAINT 메시지는 모든 메시지 중에 우선 순위가 가장 늦다. 
GetMessage 함수는 메시지 큐에 WM_PAINT가 있더라도 다른 메시지가 대기중이면 이 메시지를 먼저 처리한다.

WM_PAINT 메시지에서 그리기를 할 때는 BeginPaint 함수와 EndPaint 함수를 사용해야 한다. 
이 함수는 다시 그려야 할 영역에 대한 정확한 좌표를 조사하며 무효영역을 유효화하고 캐럿을 숨기거나 배경을 지우는 등의 꼭 필요한 동작을 한다.

＊WM_QUIT
응용 프로그램을 종료하라는 신호이다. PostQuitMessage 함수 호출에 의해 발생하며 GetMessage 함수가 0을 리턴하도록 함으로써 메시지 루프를 종료시키는 역할
GetMessage 함수는 WM_QUIT 이외의 모든 메시지에 대해 0이 아닌 값을 리턴하므로 계속 루프를 돌지만 WM_QUIT에 대해서만 0을 리턴
GetMessage 함수가 0이 아닌 값을 리턴하는 동안 무한히 이 루프를 도는데 단 WM_QUIT가 전달될 때는 while문이 종료되며 따라서 WinMain이 종료된다. 
메인 윈도우의 WM_DESTROY에서는 반드시 PostQuitMessage 함수를 호출하여 메시지 루프가 종료될 수 있도록 해 주어야 한다

＊WM_SIZE
문자열을 계속 작업 영역 중앙에 두고 싶으면 윈도우의 크기가 변경될 때마다 다시 출력해 주어야 하는데 이때 사용되는 메시지.
이 메시지는 윈도우의 크기가 변경되었을 때 보내진다. 
이때 lParam의 하위 워드에는 변경된 후의 윈도우 폭이, 상위 워드에서는 높이가 전달되며 wParam에는 이 메시지가 발생한 이유를 나타내는 플레그가 전달.
SIZE_MAXHIDE   다른 윈도우 최대화 되어 이 윈도우 가려짐
SIZE_MAXIMIZED 최대화
SIZE_MAXSHOW   다른 윈도우가 원래 크기로 되어 이 윈도우 보여짐.
SIZE_MINIMIZED 최소화
SIZE_RESTORED  크기 변경
*/