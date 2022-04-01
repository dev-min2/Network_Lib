#include "User.h"
#include "ChatProtocol.h"
User::User(ClientInfo& client) : Session(client)
{
	m_executeFunc.insert
	(
		std::make_pair(static_cast<uInt16>(ChatProtocol::CS_REQU_CHAT), &User::ParsingChatMsg)
	);
}

User::~User()
{
}

void User::PacketParsing(Packet& packet)
{
	auto iter = m_executeFunc.find(packet.GetProtocolID());
	if (iter != m_executeFunc.end())
		iter->second(*this, packet);
}

void User::EventBeforeDelete()
{

}

void User::ParsingChatMsg(Packet& packet)
{
	CS_REQU_CHAT_PACKET chat;

	packet >> chat.name >> chat.chatMsg;
	Packet sendPacket;
	sendPacket.SetProtocolID(static_cast<uInt16>(ChatProtocol::SC_REQU_CHAT));
	sendPacket << chat.name << chat.chatMsg;

	BroadCast(sendPacket);
}
