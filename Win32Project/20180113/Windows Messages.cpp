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

	hWnd = CreateWindow(TEXT("App"), TEXT("Sample Application"),//TEXT("__")�� unicode�� ��ȯ. "App"�� �ݺ�����ϴ� ������ � ���α׷��� ����Ѵٴ� ��.
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
	static char str[256];//[������Ʈ]-[�Ӽ�]-[�����Ӽ�]-[�Ϲ�]-[��������]-��Ƽ����Ʈ
	int len;
	static int x = 100;
	static int y = 100;

	static RECT rt;
	static BOOL bnowDraw = FALSE;

	switch (iMsg) {
	case WM_CREATE://������ �����Ǹ� ũ��� ��ġ ����
		MoveWindow(hWnd, 200, 100, 500, 500, FALSE);
		return 0;

	case WM_CHAR://WM_CHAR�� ���޵Ǵ� ���ڵ��� ���ڿ� ���ۿ� ��� ���
		len = strlen(str);
		str[len] = (TCHAR)wParam;//wParam���� ���޵� Ű �ڵ带 str ���ڹ迭�� ����.
		str[len + 1] = 0;//���ڿ��� ���� �� �κп� wParam���� �� �ְ� �ٷ� ������ 0�� �� �־� ���ڿ� ���� ǥ��.
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
		InvalidateRect(hWnd, NULL, TRUE);//ȭ���� �ٽ� �׸��� ���� ȣ��. TRUE�� �ٽ� �׸���, FALSE�� �����ִ�.
		return 0;

	case WM_LBUTTONDOWN://���콺 ��ư�� ���� ���� ��ǥ�� x,y�� ������ ���� ���콺�� ĸó.�� ���¿��� ���콺�� �����̸� �̵� �Ÿ���ŭ ���� �׸�
		x = (int)(short)LOWORD(lParam);
		y = (int)(short)HIWORD(lParam);
		bnowDraw = TRUE;
		SetCapture(hWnd);
		return 0;
	case WM_MOUSEMOVE://���콺�� ���� �
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

	case WM_SIZE: //������ ũ�Ⱑ ����� ������ �۾� ������ ũ�⸦ �ٽ� ����ϰ� ȭ���� �ٽ� �׸��Ƿ� ���ڿ��� �������� ũ�⿡ ������� �׻� ���� ��ġ
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
��WM_CHAR 
Ű����κ��� �����Է� ���� ��
Ű����κ��� �Է��� �߻����� ��� ��������� ��Ŀ���� ���� ���α׷����� Ű���� �޽���(WM_CHAR)�� �����ָ� ���α׷��� �� �޽����� �޾� Ű���� �Է��� ó��.
�Էµ� ������ �ƽ�Ű �ڵ带 wParam���� �����ϵ��� �Ǿ� ������ �츮�� wParam�� ���� �о� ����ڰ� � Ű�� ���������� �˾Ƴ��� �ȴ�

��WM_COMMAND
���α׷� �����߿� ����ڰ� �޴� �׸��� ������ �� �߻�.
���� ��Ʈ�ѷκ��� ���� ���޵Ǹ� �� ��Ʈ���� �پ��� ���� �޽����� �����Ƿ� �� �޽����� �Ϲ������� ������ ���� ���� switch������ �ۼ��ȴ�.

��WM_CREATE
CreateWindow(Ex) �Լ��� ���� �����찡 ������ �� ��������. 
�޸𸮿� �����츦 ������ �� ȭ�鿡 ���̱� ���� �������� �ַ� �����쿡 ���õ� �ʱ�ȭ �۾��� �� �� ���ȴ�. 
������ ������ ���� �޸� �Ҵ�, ���ҽ� ����, ���ϵ� ��Ʈ�� ����, ������ �Ӽ� �ʱ�ȭ �۾��� �� �޽����� ���ȴ�
�����츦 ���������� ���������� 0�� ����

��WM_DESTROY
�����찡 �ı��� �� �� �޽����� ����
DefWindowProc�� DestroyWindow �Լ��� ȣ���Ͽ� �����츦 �ı�
�����찡 ���� �������� ��� PostQuitMessage �Լ��� �ݵ�� ȣ���Ͽ� ���μ����� �޽��� ������ �����ϵ���

��WM_LBUTTONDOWN
���콺 ���� ��ư�� ���� ��
��WM_LBUTTONUP
���콺 ���� ��ư�� ���� ��

��WM_MOUSEMOVE
���콺�� ������ ��
���콺�� ��� �۾� ���� ������ �����̰� ������ �� �޽����� �ݺ������� ��� ����
MK_CONTROL
MK_LBUTTON
MK_MBUTTON
MK_RBUTTON
MK_SHIFT

��WM_PAINT
�������� �۾� ������ �Ϻΰ� ��ȿȭ�Ǿ� ���� ��
��ȿ ������ �����Ǹ� �̶����� WM_PAINT�޽����� ���޵ȴ�.
�� �����찡 ó�� �����Ǿ��� ��
�� �������� ��ġ�� �̵��Ǿ��� ��
�� �������� ũ�Ⱑ ����Ǿ��� ��. �ִ�, �ּ�ȭ�Ǿ��� ��
�� �ٸ� �����쿡 ������ �ִٰ� �巯�� ��
�� ��ũ�� �� ��
���� ���α׷� ���ο��� InvalidateRect �Լ� ȣ�⿡ ���� ������ ��ȿ ������ ����

WM_PAINT �޽����� ��� �޽��� �߿� �켱 ������ ���� �ʴ�. 
GetMessage �Լ��� �޽��� ť�� WM_PAINT�� �ִ��� �ٸ� �޽����� ������̸� �� �޽����� ���� ó���Ѵ�.

WM_PAINT �޽������� �׸��⸦ �� ���� BeginPaint �Լ��� EndPaint �Լ��� ����ؾ� �Ѵ�. 
�� �Լ��� �ٽ� �׷��� �� ������ ���� ��Ȯ�� ��ǥ�� �����ϸ� ��ȿ������ ��ȿȭ�ϰ� ĳ���� ����ų� ����� ����� ���� �� �ʿ��� ������ �Ѵ�.

��WM_QUIT
���� ���α׷��� �����϶�� ��ȣ�̴�. PostQuitMessage �Լ� ȣ�⿡ ���� �߻��ϸ� GetMessage �Լ��� 0�� �����ϵ��� �����ν� �޽��� ������ �����Ű�� ����
GetMessage �Լ��� WM_QUIT �̿��� ��� �޽����� ���� 0�� �ƴ� ���� �����ϹǷ� ��� ������ ������ WM_QUIT�� ���ؼ��� 0�� ����
GetMessage �Լ��� 0�� �ƴ� ���� �����ϴ� ���� ������ �� ������ ���µ� �� WM_QUIT�� ���޵� ���� while���� ����Ǹ� ���� WinMain�� ����ȴ�. 
���� �������� WM_DESTROY������ �ݵ�� PostQuitMessage �Լ��� ȣ���Ͽ� �޽��� ������ ����� �� �ֵ��� �� �־�� �Ѵ�

��WM_SIZE
���ڿ��� ��� �۾� ���� �߾ӿ� �ΰ� ������ �������� ũ�Ⱑ ����� ������ �ٽ� ����� �־�� �ϴµ� �̶� ���Ǵ� �޽���.
�� �޽����� �������� ũ�Ⱑ ����Ǿ��� �� ��������. 
�̶� lParam�� ���� ���忡�� ����� ���� ������ ����, ���� ���忡���� ���̰� ���޵Ǹ� wParam���� �� �޽����� �߻��� ������ ��Ÿ���� �÷��װ� ����.
SIZE_MAXHIDE   �ٸ� ������ �ִ�ȭ �Ǿ� �� ������ ������
SIZE_MAXIMIZED �ִ�ȭ
SIZE_MAXSHOW   �ٸ� �����찡 ���� ũ��� �Ǿ� �� ������ ������.
SIZE_MINIMIZED �ּ�ȭ
SIZE_RESTORED  ũ�� ����
*/