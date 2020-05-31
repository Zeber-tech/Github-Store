// 分形.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "分形.h"
#include <corecrt_math.h>


#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

const int iXmax = 640;
const int iYmax = 550;
const double zXmin = -1.5;
const double zXmax = 1.5;
const double zYmin = -1.3;
const double zYmax = 1.3;
const int IterationMax = 255;
const double EscapeRadius = 2;

double cX = 0.35;//可变量
double cY = 0.09;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    //case WM_CHAR:
        char hit;
        char num[4];
        num[4] = wParam;
        hit = wParam;
        if (hit == ' ')
        {
            cX = num[4];
            num[4] = 0.37;
            InvalidateRect(hWnd, NULL, TRUE);
        }
            /*hit = wParam;
        num = wParam;
        if (hit == 'x')
            cX = num;
        if (hit == 'y')
            cY = num;
        else if (hit == ' ')
        {
            num = 0;
            InvalidateRect(hWnd, NULL, TRUE);
        }*/
        
        break;
         
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            int iX, iY;
            double PixelWidth = (zXmax - zXmin) / iXmax;
            double PixelHeigh = (zYmax - zYmin) / iYmax;
            COLORREF color;
            double zX, zY,zX2,zY2;
            int Iteration;
            double ER2=EscapeRadius*EscapeRadius;
            for (iY = 0; iY < iYmax; iY++)
            {
                zY = zYmin + iY * PixelHeigh;
                //if (fabs(zY) < PixelHeigh )zY = 0.0;
                for (iX = 0; iX < iXmax; iX++)
                {
                    zX = zXmin + iX * PixelWidth;
                    
                    zX2 = zX * zX;
                    zY2 = zY * zY;
                    Iteration = 0;
                    while (Iteration < IterationMax && (zX2 + zY2) < ER2)
                    {
                        zY = 2 * zX * zY + cY;
                        zX = zX2 - zY2 + cX;
                        zX2 = zX * zX;
                        zY2 = zY * zY;
                        Iteration++;
                    }
                    if (Iteration == IterationMax)
                        color = RGB(255, 0, 0);//收敛，前景色黑色
                    else if (Iteration > 50)
                        color = RGB(255, 185, 15);//逃逸，背景色白色
                    else if (Iteration > 10)
                        color = RGB(255, 255, 0);
                    else if (Iteration > 6)
                        color = RGB(0, 255, 0);
                    else if (Iteration > 3)
                        color = RGB(0, 0, 255);
                    else if (Iteration >1)
                        color = RGB(0, 206, 209);
                    else
                        color = RGB(200, 200, 255);


                    SetPixel(hdc, iX, iY, color);
                    zY = zYmin + iY * PixelHeigh;//将被百般摧残的zY复回原样，非常重要

                }
            }
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
