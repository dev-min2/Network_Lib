#pragma once
#include <string>
#include <cstdint>
using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;
using uInt64 = std::uint64_t;
using uInt32 = std::uint32_t;
using uInt16 = std::uint16_t;
using uInt8 = std::uint8_t;

enum class ChatProtocol
{
	CS_REQU_CHAT = 1000,
	SC_REQU_CHAT
};
#pragma pack(push,1)
/// <summary>
/// 클라 - 서버
/// 클라 : 입력한 채팅내용과 채팅을 친 유저의 아이디와 함께 패킷을 전송한다.
/// 서버 : 클라에서 보낸 데이터를 그대로 BroadCast처리한다.
/// </summary>
struct CS_REQU_CHAT_PACKET
{
	std::wstring name;
	std::wstring chatMsg;
};
struct SC_REQU_CHAT_PACKET
{
	std::wstring name;
	std::wstring chatMsg;
};
#pragma pack(pop)