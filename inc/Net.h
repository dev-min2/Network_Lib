#pragma once
#include "std.h"
class Packet;
class Net
{
public:
	std::list<Packet*>	m_packetPool; // recvó���Ǿ ���� ��Ŷ Ǯ. �����ͼ� ����Ʈ �ؾ��Ѵ�.
	//std::list<DEFAULT_PACKET*>	m_packetPool; // recvó���Ǿ ���� ��Ŷ Ǯ. �����ͼ� ����Ʈ �ؾ��Ѵ�.
protected:
	Net() = default;
	virtual ~Net();
public:
	bool	InitClientNet();
	bool	CloseClientNet();
	int		SendPacket(Packet& packet);
	int		RecvPacket();
private:
	int		RecvProcess(int recvBytes);
protected:
	bool	m_initCheck = false;
	SOCKET  m_clientSock = INVALID_SOCKET;
private:
	char	m_recvBuffer[PACKET_BUFFER_SIZE] = { 0, };
	int		m_beginPos = 0;
	int		m_endPos = 0;
	int		m_writePos = 0;
	int		m_readPos = 0;
};

