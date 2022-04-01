#include "Window.h"
#include "AsyncIO.h"
HWND g_hWindowHandle;
RECT g_clientRect;
Window* g_pWindow;
Window::Window() : m_windowInit(false),m_hInst(NULL),m_hWindowHandle(NULL),m_gameRun(false)
{
    ::ZeroMemory(&m_clientRect, sizeof(RECT));
}

Window::~Window()
{
}

bool Window::InitWindow(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
    g_pWindow = this;
    m_hInst = hInstance;
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;//WS_CAPTION | WS_SYSMENU
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, NULL);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"testWindow";
    wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

    RegisterClassExW(&wcex);


    // 윈도우 생성
    RECT rt = { 0,0,width,height };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE); // 클라이언트 영역이 우리가 원하는 크기가 되게끔 만들어준다.(RT의 크기조정)

   /* m_hWindowHandle = CreateWindow(wcex.lpszClassName, L"오목", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        rt.right - rt.left, rt.bottom - rt.top,
        nullptr, nullptr, m_hInst, nullptr);
    */
    m_hWindowHandle = CreateWindow(
        wcex.lpszClassName,
        L"Window",
        WS_CAPTION | WS_SYSMENU, //
        CW_USEDEFAULT, CW_USEDEFAULT,
        rt.right - rt.left,
        rt.bottom - rt.top,
        nullptr,
        nullptr,
        m_hInst,
        nullptr);

    if (!m_hWindowHandle)
    {
        return FALSE;
    }
    g_hWindowHandle = m_hWindowHandle;


    ShowWindow(m_hWindowHandle, SW_SHOW);
    UpdateWindow(m_hWindowHandle);
    m_gameRun = true;
    m_windowInit = true;



    ::GetClientRect(m_hWindowHandle, &m_clientRect);
    g_clientRect = m_clientRect;
	return true;
}

bool Window::WindowRun()
{
    if (!m_windowInit)
        return false;

    MSG msg;
    GameInit();

    // 기본 메시지 루프입니다:
    while (m_gameRun)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_gameRun = GameRun();
        }
    }
    GameRelease();
	return true;
}

LRESULT Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    AsyncIO::GetInstance()->MsgProc(hWnd, message, wParam, lParam); // 네트워크 메시지를 받기 위해
    g_pWindow->MsgProc(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
