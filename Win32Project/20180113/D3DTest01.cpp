#include <Windows.h>
#include <tchar.h>
#include <d3d9.h>

LPDIRECT3D9 pD3D; 
LPDIRECT3DDEVICE9 pD3DDevice; 

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
bool InitDirect3D(HWND hWnd);
void Render(void);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDC_ICON);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("MyApp");//클래스

	RegisterClass(&wc);

	HWND hWnd;
	hWnd = CreateWindow(TEXT("MyApp"), TEXT("Sample Application"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	if (InitDirect3D(hWnd) == FALSE)
	{
		MessageBox(NULL, _T("Error Init Direct3D"), _T("Error"), MB_OK);
		return 0;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();//렌더링
		}
	}

	//Direct3D Device 해제
	if (pD3DDevice != NULL)
		pD3DDevice->Release();

	//Direct3D Object 해제
	if (pD3D != NULL)
		pD3D->Release();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

bool InitDirect3D(HWND hWnd) 
{
	pD3D = NULL;
	pD3DDevice = NULL;

	//DirectX Object 생성
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (pD3D == NULL)
		return false;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp)); //구조체를 0으로 초기화.

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;//백버퍼 개수 1개.
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;
	d3dpp.hDeviceWindow = hWnd;

	//기본 DirectX Device 생성
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, 
		&pD3DDevice)))
	{
		return false;
	}
	return true;
}

void Render(void)
{
	if (pD3DDevice == NULL)
		return;

	//백버퍼 지우기
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(250, 210, 230), 1.0f, 0);

	//백버퍼 데이터 화면에 전송
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
