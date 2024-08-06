#include<windows.h>
#include<cstdio>
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//CALLBACK������Ϊ__stdcall

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
	wndclass.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);//LoadImage ����ͼ���Թ�����ʹ�� ���� https://learn.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-loadimagea
	wndclass.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//GetStockObject ��ȡһ��ͼ�ζ��� ���������ڶԴ��ڵı��������ػ� ���� https://learn.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-setdcbrushcolor
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	RegisterClass(&wndclass);//RegisterClass ע�ᴰ����

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
	);//CreateWindow ����һ������(���ڴ�����)

	ShowWindow(hWnd, iCmdShow);//ShowWindow ����Ļ����ʾ����
	UpdateWindow(hWnd);//UpdateWindow ָʾ���ڶ�����������ػ�

	while (GetMessage(&msg, NULL, 0, 0))//GetMessage ����Ϣ���л�ȡ��Ϣ
	{
		TranslateMessage(&msg);//TranslateMessage ����һЩ������Ϣ
		DispatchMessage(&msg);//DispatchMessage ����Ϣ���͸����ڹ���
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
		hdc = BeginPaint(hwnd, &ps);//BeginPaint �������ڻ��ƿ�ʼ
		GetClientRect(hwnd, &rect);//GetClientRect ��ȡ���ڿͻ����ĳߴ�
		for (i = 0; i < 10000; i++)
		{
			int temp = wsprintf(szBuffer, TEXT("%d"), i);
			TextOut(hdc, 0, cyChar * (i - iVscrollPos), szBuffer, temp);//DrawText ��ʾһ���ı��ַ���
		}
		EndPaint(hwnd, &ps);//EndPaint �������ڻ���
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);//PostQuitMessage ���˳���Ϣ������Ϣ����
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);//DefWindowProc ִ��Ĭ�ϵ���Ϣ����
}

//������ʶ��ǰ׺
	//CS			����ѡ��
	//CW			��������ѡ��
	//DT			�ı�����ѡ��
	//IDI			ͼ���ID��
	//IDC			����ID��
	//MB			��Ϣ��ѡ��
	//SND			����ѡ��
	//WM			������Ϣ
	//WS			���ڷ��

//������ʹ�õ����ݽṹ
	//MSG			��Ϣ�ṹ
	//WNDCLASS		������ṹ
	//PAINTSTRUCT	���ƽṹ
	//RECT			���νṹ

//���
	//HINSTANCE		ʵ�����(������)
	//HWND			���ھ��
	//HDC			�豸�������
	//HICON			ͼ����
	//HCURSOR		�α���
	//HBRUSH		ͼ�λ�ˢ���