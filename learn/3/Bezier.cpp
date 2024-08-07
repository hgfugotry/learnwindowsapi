#include<windows.h>
//Bezier.cpp
//hgfugotry 2024.8.7
#define POINTNUM 4

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//CALLBACK������Ϊ__stdcall

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR	szAppName[] = TEXT("Hello,Windows API!");

	//ע�ᴰ����
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

	//��������
	HWND			hWnd;
	hWnd = CreateWindow(
		szAppName, TEXT("Bezier"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL,
		hInstance, NULL);

	//���ƴ���
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	//��Ϣѭ��
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
	//���Ʊ���������
	PolyBezier(hdc, apt, 4);

	//���ƶ˵�����Ƶ������
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

		//��ȡ����x,y�ߴ�
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//���豴�������߶˵�λ��
		apt[0] = { cxClient / 4,cyClient / 2 };
		apt[3] = { 3 * cxClient / 4,cyClient / 2 };
		//���豴�������߿��Ƶ�λ��
		apt[1] = { cxClient / 2,cyClient / 4 };
		apt[2] = { cxClient / 2, 3 * cyClient / 4 };

		return 0;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON || wParam & MK_RBUTTON)
		{
			hdc = GetDC(hWnd);
			
			//�����ϴλ��ƵĹ켣
			SelectObject(hdc, GetStockObject(WHITE_PEN));
			DrawBezier(hdc, apt);

			//ѡ��ı�Ŀ��Ƶ�
			if (wParam & MK_LBUTTON)
				apt[1] = { LOWORD(lParam),HIWORD(lParam) };
			if (wParam & MK_RBUTTON)
				apt[2] = { LOWORD(lParam),HIWORD(lParam) };

			//���������ߺͶ˵�����Ƶ������
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