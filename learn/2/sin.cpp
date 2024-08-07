#include<windows.h>
#include<cstdio>
#include<CMATH>
//sin.cpp
//LiuLiYuan 2024.8.7
#define NUM INT_MAX-30000
#define POINTNUM 1000
#define TWOPI (2*3.14159)

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//CALLBACK被定义为__stdcall

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR    szAppName[] = TEXT("Hello,Windows API!");

    //注册窗口类
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
    
    //创建窗口
    HWND            hWnd;
    hWnd = CreateWindow(
        szAppName,TEXT("Hello World"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,
        CW_USEDEFAULT,CW_USEDEFAULT,
        NULL,NULL,
        hInstance,NULL);
    
    //绘制窗口
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    
    //消息循环
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
    
        //获取窗口x,y尺寸
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
    
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);//BeginPaint 标明窗口绘制开始
    
        //绘制x轴,y轴
        MoveToEx(hdc, 0, cyClient / 2, NULL);
        LineTo(hdc, cxClient, cyClient / 2);
        MoveToEx(hdc, cxClient / 2, 0, NULL);
        LineTo(hdc, cxClient / 2, cyClient);
        TextOut(hdc, cxClient / 1.01, cyClient / 2, TEXT("x"), 1);
        TextOut(hdc, cxClient / 2, 0, TEXT("y"), 1);
    
        //在左上角绘制"Hello World!"
        TextOut(hdc, 0, 0, szBuffer, lstrlen(szBuffer));
    
        //绘制正弦曲线
        for (i = 0; i < POINTNUM; i++)
        {
            apt[i].x = i * cxClient / POINTNUM;
            apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI * i / POINTNUM)));
        }
        Polyline(hdc, apt, POINTNUM);
    
        EndPaint(hWnd, &ps);//EndPaint 结束窗口绘制
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);//PostQuitMessage 将退出消息插入消息队列
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);//DefWindowProc 执行默认的消息处理

}

//常量标识符前缀
    //CS            类风格选项
    //CW            创建窗口选项
    //DT            文本绘制选项
    //IDI            图标的ID号
    //IDC            光标的ID号
    //MB            消息框选项
    //SND            声音选项
    //WM            窗口消息
    //WS            窗口风格

//程序中使用的数据结构
    //MSG            消息结构
    //WNDCLASS        窗口类结构
    //PAINTSTRUCT    绘制结构
    //RECT            矩形结构

//句柄
    //HINSTANCE        实例句柄(程序本身)
    //HWND            窗口句柄
    //HDC            设备环境句柄
    //HICON            图标句柄
    //HCURSOR        游标句柄
    //HBRUSH        图形画刷句柄