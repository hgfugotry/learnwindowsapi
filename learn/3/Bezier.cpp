#include<windows.h>
//Bezier.cpp
//hgfugotry 2024.8.7
#define POINTNUM 4

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//CALLBACK被定义为__stdcall

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR	szAppName[] = TEXT("Hello,Windows API!");

	//注册窗口类
	WNDCLASS		wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	wndclass.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	RegisterClass(&wndclass);

	//创建窗口
	HWND			hWnd;
	hWnd = CreateWindow(
		szAppName, TEXT("Bezier"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL,
		hInstance, NULL);

	//绘制窗口
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	//消息循环
	MSG				msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void DrawBezier(HDC hdc, POINT apt[])
{
	//绘制贝塞尔曲线
	PolyBezier(hdc, apt, 4);

	//绘制端点与控制点的连线
	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);
	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[3].x, apt[3].y);
	MoveToEx(hdc, apt[3].x, apt[3].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);
	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[0].x, apt[0].y);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int cxClient, cyClient;
	static POINT apt[POINTNUM];

	switch (message)
	{
	case WM_SIZE:

		//获取窗口x,y尺寸
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//重设贝塞尔曲线端点位置
		apt[0] = { cxClient / 4,cyClient / 2 };
		apt[3] = { 3 * cxClient / 4,cyClient / 2 };
		//重设贝塞尔曲线控制点位置
		apt[1] = { cxClient / 2,cyClient / 4 };
		apt[2] = { cxClient / 2, 3 * cyClient / 4 };

		return 0;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON || wParam & MK_RBUTTON)
		{
			hdc = GetDC(hWnd);
			
			//清理上次绘制的轨迹
			SelectObject(hdc, GetStockObject(WHITE_PEN));
			DrawBezier(hdc, apt);

			//选择改变的控制点
			if (wParam & MK_LBUTTON)
				apt[1] = { LOWORD(lParam),HIWORD(lParam) };
			if (wParam & MK_RBUTTON)
				apt[2] = { LOWORD(lParam),HIWORD(lParam) };

			//绘制新曲线和端点与控制点的连线
			SelectObject(hdc, GetStockObject(BLACK_PEN));
			DrawBezier(hdc, apt);
			
			ReleaseDC(hWnd, hdc);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawBezier(hdc, apt);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//常量标识符前缀
	//CS			类风格选项
	//CW			创建窗口选项
	//DT			文本绘制选项
	//IDI			图标的ID号
	//IDC			光标的ID号
	//MB			消息框选项
	//SND			声音选项
	//WM			窗口消息
	//WS			窗口风格

//程序中使用的数据结构
	//MSG			消息结构
	//WNDCLASS		窗口类结构
	//PAINTSTRUCT	绘制结构
	//RECT			矩形结构

//句柄
	//HINSTANCE		实例句柄(程序本身)
	//HWND			窗口句柄
	//HDC			设备环境句柄
	//HICON			图标句柄
	//HCURSOR		游标句柄
	//HBRUSH		图形画刷句柄