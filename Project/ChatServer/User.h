#pragma once
#include "Session.h"
class User : public Session
{
public:
	User(ClientInfo& client);
	~User();
public:
	//Session�� ���� �����Լ�
	void PacketParsing(Packet& packet) override; // ��Ŷ�ؼ�(����(������) �������ݿ� �°� parsing)
	void EventBeforeDelete()		   override; // Session�� delete�Ǳ��� �������������� ó���Ǿ�� �� ��������.
private:
	void ParsingChatMsg(Packet& packet);
private:
	// ����Լ��� �����ϱ� ���� this�� ������ �˾ƾ��Ѵ�.
	using CallbackFunc = std::function<void(User&, Packet& packet)>;
	std::unordered_map<uInt16, CallbackFunc> m_executeFunc;
};

