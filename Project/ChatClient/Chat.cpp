#include "Chat.h"
#include "AsyncIO.h"
#include "ChatProtocol.h"
#include "Packet.h"
#include <string.h>
Chat::~Chat()
{
}
bool Chat::GameInit()
{
    Init();
    return true;
}

bool Chat::GameRun()
{
    if (!Update()) return false;
    if (!Render()) return false;

    return true;
}

bool Chat::GameRelease()
{
    Release();
    return true;
}

LRESULT Chat::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 200:
        {
            int index;

            wchar_t buffer[1024] = { 0, };
            ::SendMessage(m_hEdit, WM_GETTEXT, 1024, (LPARAM)buffer);
            if (lstrcmp(buffer, L"") == 0)
                break;
            // 패킷 전송.
            Packet packet(static_cast<uInt16>(ChatProtocol::CS_REQU_CHAT));
            
            packet << m_name << buffer;
            AsyncIO::GetInstance()->SendPacket(packet);
            SendMessage(m_hEdit, WM_SETTEXT, 0, (LPARAM)""); // 에디트 박스 비우기.
        }break;
        default:
            break;
        }
    }break;
    default:
        break;
    }
    return 0;
}

bool Chat::Init()
{
    m_name.reserve(10);
    m_name += L"Test";
    DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;

    m_hEdit = CreateWindow(L"edit", NULL, style,
        180, g_clientRect.bottom - 50, 300, 50,
        m_hWindowHandle, (HMENU)100, m_hInst, NULL);
    style = WS_CHILD | WS_VISIBLE;
    m_hButton = CreateWindow(L"button", L"전송", style,
        500, g_clientRect.bottom - 50, 50, 50,
        m_hWindowHandle, (HMENU)200, m_hInst, NULL);
    m_hListBox = CreateWindow(L"listbox", NULL, WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, //| WS_VSCROLL,
        0, 0, g_clientRect.right, g_clientRect.bottom - 70,
        m_hWindowHandle, (HMENU)300, m_hInst, NULL);


    AsyncIO::GetInstance()->InitClientNet();
    AsyncIO::GetInstance()->Connect();
    return true;
}

bool Chat::Update()
{
    auto iter = AsyncIO::GetInstance()->m_packetPool.begin();
    for (; iter != AsyncIO::GetInstance()->m_packetPool.end(); )
    {
        Packet* packet = *iter;
        switch(packet->GetProtocolID())
        {
        case static_cast<uInt16>(ChatProtocol::SC_REQU_CHAT):
        {
            SC_REQU_CHAT_PACKET chat;
            *packet >> chat.name >> chat.chatMsg;

            std::wstring msg;
            msg += L"[";
            msg += chat.name;
            msg += L"] ";
            msg += chat.chatMsg;

            int index = ::SendMessage(m_hListBox, LB_ADDSTRING, 0, (LPARAM)msg.c_str());
            ::SendMessage(m_hListBox, LB_SETCURSEL, index, 0);
            ::SendMessage(m_hListBox, LB_SETCURSEL, -1, 0);
        }break;
        }
        // 처리 완료했으니 삭제.
        delete* iter;
        iter = AsyncIO::GetInstance()->m_packetPool.erase(iter);
    }
    return true;
}

bool Chat::Render()
{   
    return true;
}

bool Chat::Release()
{
    AsyncIO::GetInstance()->DeleteInstance();
    return true;
}
