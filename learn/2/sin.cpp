#include<windows.h>
#include<cstdio>
#include<CMATH>
//sin.cpp
//LiuLiYuan 2024.8.7
#define NUM INT_MAX-30000
#define POINTNUM 1000
#define TWOPI (2*3.14159)

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//CALLBACK������Ϊ__stdcall

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR    szAppName[] = TEXT("Hello,Windows API!");

    //ע�ᴰ����
    WNDCLASS        wndclass;
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
    HWND            hWnd;
    hWnd = CreateWindow(
        szAppName,TEXT("Hello World"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,
        CW_USEDEFAULT,CW_USEDEFAULT,
        NULL,NULL,
        hInstance,NULL);
    
    //���ƴ���
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    
    //��Ϣѭ��
    MSG                msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static int cxClient, cyClient;
    int i;
    POINT apt[POINTNUM];
    TCHAR szBuffer[32]=TEXT("Hello World!");

    switch (message)
    {
    case WM_SIZE:
    
        //��ȡ����x,y�ߴ�
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
    
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);//BeginPaint �������ڻ��ƿ�ʼ
    
        //����x��,y��
        MoveToEx(hdc, 0, cyClient / 2, NULL);
        LineTo(hdc, cxClient, cyClient / 2);
        MoveToEx(hdc, cxClient / 2, 0, NULL);
        LineTo(hdc, cxClient / 2, cyClient);
        TextOut(hdc, cxClient / 1.01, cyClient / 2, TEXT("x"), 1);
        TextOut(hdc, cxClient / 2, 0, TEXT("y"), 1);
    
        //�����Ͻǻ���"Hello World!"
        TextOut(hdc, 0, 0, szBuffer, lstrlen(szBuffer));
    
        //������������
        for (i = 0; i < POINTNUM; i++)
        {
            apt[i].x = i * cxClient / POINTNUM;
            apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI * i / POINTNUM)));
        }
        Polyline(hdc, apt, POINTNUM);
    
        EndPaint(hWnd, &ps);//EndPaint �������ڻ���
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);//PostQuitMessage ���˳���Ϣ������Ϣ����
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);//DefWindowProc ִ��Ĭ�ϵ���Ϣ����

}

//������ʶ��ǰ׺
    //CS            ����ѡ��
    //CW            ��������ѡ��
    //DT            �ı�����ѡ��
    //IDI            ͼ���ID��
    //IDC            ����ID��
    //MB            ��Ϣ��ѡ��
    //SND            ����ѡ��
    //WM            ������Ϣ
    //WS            ���ڷ��

//������ʹ�õ����ݽṹ
    //MSG            ��Ϣ�ṹ
    //WNDCLASS        ������ṹ
    //PAINTSTRUCT    ���ƽṹ
    //RECT            ���νṹ

//���
    //HINSTANCE        ʵ�����(������)
    //HWND            ���ھ��
    //HDC            �豸�������
    //HICON            ͼ����
    //HCURSOR        �α���
    //HBRUSH        ͼ�λ�ˢ���