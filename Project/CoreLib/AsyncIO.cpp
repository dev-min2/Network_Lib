#include "AsyncIO.h"

AsyncIO* AsyncIO::m_instance = nullptr;

bool AsyncIO::Connect()
{
    SOCKADDR_IN serverAddr;
    ::ZeroMemory(&serverAddr, sizeof(SOCKADDR_IN));

    // 소켓이벤트를 알려줄 윈도우 메시지와 감시할 네트워크 이벤트 등록.
    // 등록한 소켓은 자동으로 논블록킹 소켓이 된다.
    if (::WSAAsyncSelect(m_clientSock, g_hWindowHandle, NETWORK_MSG,
        FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
    {
        ::MessageBoxA(g_hWindowHandle, "AsyncSelect 실패", "경고", 0);
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
    serverAddr.sin_port = ::htons(9199); // 네트워크 바이트 순서

    int ret = ::connect(m_clientSock, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));
    if (ret == SOCKET_ERROR)
    {
        int errorCode = ::WSAGetLastError();
        if (errorCode == WSAEWOULDBLOCK)
        {
            while (true)
            {
                if (::WSAGetLastError() == WSAEISCONN)
                    break;
                ::connect(m_clientSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
            }
        }
        else
        {
            std::string s1 = "Connect 실패 사유 : ";
            s1 += std::to_string(errorCode);
            ::MessageBoxA(g_hWindowHandle, s1.c_str(), "경고", 0);
            return false;
        }
    }


    return true;
}

LRESULT AsyncIO::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) // 네트워크 메시지 처리. 여기서 굳이 안해도 되긴함
    {
    case NETWORK_MSG:
    {
        WORD wRet = WSAGETSELECTEVENT(lParam);
        switch (wRet)
        {
        case FD_CONNECT:
        {
            m_isConnected = true;
        }break;
        case FD_CLOSE:
        {
            m_isConnected = false;
        }break;
        case FD_READ:
        {
            // Recv처리
            Net::RecvPacket();
        }break;
        case FD_WRITE:
        {

        }break;
        default:
            break;
        }
    }
    }

    return 0;
}

AsyncIO::AsyncIO() : m_isConnected(false)
{

}

AsyncIO::~AsyncIO()
{
}
