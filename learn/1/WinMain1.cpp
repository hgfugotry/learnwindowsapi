#include<windows.h>
#include<cstdio>
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//CALLBACK被定义为__stdcall

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR	szAppName[] = TEXT("Hello,Windows API!");
	HWND			hWnd;
	MSG				msg;
	WNDCLASS		wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);//LoadImage 加载图标以供程序使用 参阅 https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-loadimagea
	wndclass.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//GetStockObject 获取一个图形对象 在这里用于对窗口的背景进行重绘 参阅 https://learn.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-setdcbrushcolor
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	RegisterClass(&wndclass);//RegisterClass 注册窗口类

	hWnd = CreateWindow(
		szAppName,
		TEXT("Hello World"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);//CreateWindow 创建一个窗口(基于窗口类)

	ShowWindow(hWnd, iCmdShow);//ShowWindow 在屏幕中显示窗口
	UpdateWindow(hWnd);//UpdateWindow 指示窗口对其自身进行重绘

	while (GetMessage(&msg, NULL, 0, 0))//GetMessage 从消息队列获取消息
	{
		TranslateMessage(&msg);//TranslateMessage 翻译一些键盘消息
		DispatchMessage(&msg);//DispatchMessage 将消息发送给窗口过程
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static int cxChar, cxCaps, cyChar, cxClient, cyClient, iVscrollPos = 0;
	TEXTMETRIC tm;
	TCHAR szBuffer[100];
	static SCROLLINFO scrollinfo = { sizeof(SCROLLINFO),SIF_RANGE,0,1000,0,0 };
	int i = 0;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hwnd, hdc);
		SetScrollInfo(hwnd, SB_VERT, &scrollinfo, TRUE);
		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollPos -= 1;
			break;
		case SB_LINEDOWN:
			iVscrollPos += 1;
			break;
		case SB_PAGEUP:
			iVscrollPos -= cyClient / cyChar;
			break;
		case SB_PAGEDOWN:
			iVscrollPos += cyClient / cyChar;
			break;
		case SB_THUMBPOSITION:
			iVscrollPos = HIWORD(wParam);
			break;
		default:
			break;
		}
		if (iVscrollPos > 1000)
			iVscrollPos = 1000;
		if (iVscrollPos < 0)
			iVscrollPos = 0;
		scrollinfo.fMask = SIF_POS;
		scrollinfo.nPos = iVscrollPos;
		SetScrollInfo(hwnd, SB_VERT, &scrollinfo, TRUE);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);//BeginPaint 标明窗口绘制开始
		GetClientRect(hwnd, &rect);//GetClientRect 获取窗口客户区的尺寸
		for (i = 0; i < 10000; i++)
		{
			int temp = wsprintf(szBuffer, TEXT("%d"), i);
			TextOut(hdc, 0, cyChar * (i - iVscrollPos), szBuffer, temp);//DrawText 显示一个文本字符串
		}
		EndPaint(hwnd, &ps);//EndPaint 结束窗口绘制
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);//PostQuitMessage 将退出消息插入消息队列
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);//DefWindowProc 执行默认的消息处理
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