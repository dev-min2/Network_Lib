// DummyClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include "Packet.h"
#include "ChatProtocol.h"
#include "Types.h"
#include <thread>
#include <chrono>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")
#define MAX_SOCKET 1000
SOCKET socketTest[MAX_SOCKET];
int index = 0;

void CreateSocket()
{
    if (index >= MAX_SOCKET)
        return;
    socketTest[index++] = ::socket(AF_INET, SOCK_STREAM, 0);
}

void Connect()
{
    SOCKADDR_IN addr;
    int len = sizeof(SOCKADDR_IN);

    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(9199);
    addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
    for (int i = 0; i < MAX_SOCKET; ++i)
    {
        ::connect(socketTest[i], (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
        std::cout << "클라이언트 " << i << "접속.." << std::endl;
    }
}

void OneChat()
{
    Packet p1;
    p1.SetProtocolID(static_cast<uInt16>(ChatProtocol::CS_REQU_CHAT));


    p1 << (L"DummyClient") << L" Hello Server!!";
    char* pMsg = (char*)&p1.GetDefaultPacket();
    int sendSize;
   
    sendSize = 0;
    do
    {
        int sendBytes = ::send(socketTest[0], &pMsg[sendSize], p1.GetDefaultPacket().m_ph.m_len - sendSize, 0);
        sendSize += sendBytes;
    } while (sendSize < p1.GetDefaultPacket().m_ph.m_len);
    std::cout << "클라이언트 " << sendSize << " bytes 송신" << std::endl;
    
}
void OneRecvFunc()
{
    int recvSize;

   Packet p1;
   char buffer[100] = { 0, };
   recvSize = 0;
   do
   {
       int recvBytes = ::recv(socketTest[0], buffer, PACKET_HEADER_SIZE - recvSize, 0);
       recvSize += recvBytes;
   } while (recvSize < PACKET_HEADER_SIZE);

   p1.CopyToPacketBuffer(buffer, PACKET_HEADER_SIZE);
   int packetLen = p1.GetDefaultPacket().m_ph.m_len;
   DEFAULT_PACKET& packet = p1.GetDefaultPacket();
   recvSize = 0;
   do
   {
       int recvBytes = ::recv(socketTest[0], &packet.m_dataField[recvSize], packetLen - PACKET_HEADER_SIZE - recvSize, 0);
       recvSize += recvBytes;
   } while (recvSize < packetLen - PACKET_HEADER_SIZE);

   std::wstring name;
   std::wstring msg;
   p1 >> name >> msg;
   std::wcout << name << L":" << L" " << msg << std::endl;
    
}

void Chat()
{
    Packet p1;
    p1.SetProtocolID(static_cast<uInt16>(ChatProtocol::CS_REQU_CHAT));
    

    p1 << (L"DummyClient") << L" Hello Server!!";
    char* pMsg = (char*)&p1.GetDefaultPacket();
    int sendSize;
    for (int i = 0; i < MAX_SOCKET; ++i)
    {
        
        sendSize = 0;
        do
        {
            int sendBytes = ::send(socketTest[i], &pMsg[sendSize], p1.GetDefaultPacket().m_ph.m_len - sendSize, 0);
            sendSize += sendBytes;
        } while (sendSize < p1.GetDefaultPacket().m_ph.m_len);
        std::cout << "클라이언트 " << sendSize << " bytes 송신" << std::endl;
    }
}
void RecvFunc()
{
    int recvSize;
    for (int i = 0; i < MAX_SOCKET; ++i)
    {
        Packet p1;
        char buffer[100] = { 0, };
        recvSize = 0;
        do
        {
            int recvBytes = ::recv(socketTest[i], buffer, PACKET_HEADER_SIZE - recvSize, 0);
            recvSize += recvBytes;
        } while (recvSize < PACKET_HEADER_SIZE);

        p1.CopyToPacketBuffer(buffer, PACKET_HEADER_SIZE);
        int packetLen = p1.GetDefaultPacket().m_ph.m_len;
        DEFAULT_PACKET& packet = p1.GetDefaultPacket();
        recvSize = 0;
        do
        {
            int recvBytes = ::recv(socketTest[i], &packet.m_dataField[recvSize], packetLen - PACKET_HEADER_SIZE - recvSize, 0);
            recvSize += recvBytes;
        } while (recvSize < packetLen - PACKET_HEADER_SIZE);

        std::wstring name;
        std::wstring msg;
        p1 >> name >> msg;
        std::wcout << name << L":" << i << L" " << msg << std::endl;
    }
}

int main()
{
    WSADATA wsa;
    if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 0;

    for (int i = 0; i < MAX_SOCKET; ++i)
        CreateSocket();
    
    Connect();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    while (1)
    {
        OneChat();
        RecvFunc();
        //Chat();
        //RecvFunc();
    }
    
    

}