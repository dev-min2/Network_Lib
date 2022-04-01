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


    // ������ ����
    RECT rt = { 0,0,width,height };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE); // Ŭ���̾�Ʈ ������ �츮�� ���ϴ� ũ�Ⱑ �ǰԲ� ������ش�.(RT�� ũ������)

   /* m_hWindowHandle = CreateWindow(wcex.lpszClassName, L"����", WS_OVERLAPPEDWINDOW,
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

    // �⺻ �޽��� �����Դϴ�:
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

    AsyncIO::GetInstance()->MsgProc(hWnd, message, wParam, lParam); // ��Ʈ��ũ �޽����� �ޱ� ����
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
