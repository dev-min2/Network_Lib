#pragma once
#include "Session.h"
class User : public Session
{
public:
	User(ClientInfo& client);
	~User();
public:
	//Session의 순수 가상함수
	void PacketParsing(Packet& packet) override; // 패킷해석(게임(콘텐츠) 프로토콜에 맞게 parsing)
	void EventBeforeDelete()		   override; // Session이 delete되기전 콘텐츠로직에서 처리되어야 할 로직실행.
private:
	void ParsingChatMsg(Packet& packet);
private:
	// 멤버함수를 저장하기 위해 this의 정보를 알아야한다.
	using CallbackFunc = std::function<void(User&, Packet& packet)>;
	std::unordered_map<uInt16, CallbackFunc> m_executeFunc;
};

